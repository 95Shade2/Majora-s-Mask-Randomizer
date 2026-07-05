using System;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class SettingsShareToken
    {
        internal const string Version = "MMSETTINGS2";

        public static string Format(string canonicalPayload)
        {
            if (canonicalPayload == null)
            {
                canonicalPayload = "";
            }

            string hash = SettingsHash.Compute(canonicalPayload);
            string payloadBase64 = SettingsPayload.EncodeBase64(canonicalPayload);
            return Version + "|" + hash + "|" + payloadBase64;
        }

        public static bool TryParse(string raw, out string hash, out string payload, out string error)
        {
            hash = "";
            payload = "";
            error = null;

            if (string.IsNullOrWhiteSpace(raw))
            {
                error = "Settings token is empty.";
                return false;
            }

            string line = raw.Trim();
            int newline = line.IndexOf('\n');
            if (newline >= 0)
            {
                line = line.Substring(0, newline).Trim();
            }

            string[] parts = line.Split('|');
            if (parts.Length != 3)
            {
                error = "Settings token must have three pipe-separated fields.";
                return false;
            }

            if (!string.Equals(parts[0], Version, StringComparison.Ordinal))
            {
                error = "Unsupported settings token version: " + parts[0];
                return false;
            }

            hash = parts[1].Trim().ToLowerInvariant();
            if (hash.Length == 0)
            {
                error = "Settings token hash is empty.";
                return false;
            }

            try
            {
                payload = SettingsPayload.DecodeBase64(parts[2]);
            }
            catch (FormatException)
            {
                error = "Settings token payload is invalid base64.";
                return false;
            }

            string computed = SettingsHash.Compute(payload);
            if (!string.Equals(computed, hash, StringComparison.OrdinalIgnoreCase))
            {
                error = "Settings token hash does not match its payload.";
                return false;
            }

            return true;
        }

        public static bool TryApply(Main_Window window, string raw, out string error)
        {
            error = null;
            if (window == null)
            {
                error = "Main window is unavailable.";
                return false;
            }

            string hash;
            string payload;
            if (!TryParse(raw, out hash, out payload, out error))
            {
                return false;
            }

            if (!window.ApplyRaceSettingsFromPayload(payload))
            {
                error = "Settings token payload could not be applied.";
                return false;
            }

            if (!string.Equals(window.ComputeSettingsHash(), hash, StringComparison.OrdinalIgnoreCase))
            {
                error = "Applied settings do not match the token hash.";
                return false;
            }

            return true;
        }
    }
}
