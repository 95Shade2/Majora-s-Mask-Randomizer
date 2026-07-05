using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class RunStateFile
    {
        internal const string Version = "MMR_RUN_STATE_V1";
        internal const string DefaultFileName = "Run Hash.txt";

        public static string BuildContent(Main_Window window)
        {
            if (window == null)
            {
                return "";
            }

            string settingsPayload = window.BuildSettingsHashPayload();
            string settingsHash = SettingsHash.Compute(settingsPayload);
            string settingsToken = SettingsShareToken.Format(settingsPayload);
            Dictionary<string, string> placements = BingoPlacementMap.BuildFromWindow(window);
            string placementsBase64 = BingoPlacementMap.EncodeBase64(placements);
            string placementsHash = BingoPlacementMap.ComputeHash(placements);
            string logicName = window.GetSelectedLogicName();
            if (string.IsNullOrWhiteSpace(logicName))
            {
                logicName = "None";
            }

            StringBuilder text = new StringBuilder();
            text.AppendLine("# Majora's Mask Randomizer — Run State");
            text.AppendLine("Version=" + Version);
            text.AppendLine("RomSeed=" + (window.GetRomSeedText() ?? ""));
            text.AppendLine("Logic=" + logicName);
            text.AppendLine("SettingsHash=" + settingsHash);
            text.AppendLine("SettingsToken=" + settingsToken);
            text.AppendLine("PlacementsHash=" + placementsHash);
            text.AppendLine("PlacementsPayloadBase64=" + placementsBase64);
            return text.ToString();
        }

        public static bool TrySave(Main_Window window, string path, out string error)
        {
            error = null;
            if (window == null)
            {
                error = "Main window is unavailable.";
                return false;
            }

            if (string.IsNullOrWhiteSpace(path))
            {
                error = "Run hash file path is empty.";
                return false;
            }

            try
            {
                File.WriteAllText(path, BuildContent(window), Encoding.UTF8);
                return true;
            }
            catch (Exception ex)
            {
                error = "Could not write run hash file: " + ex.Message;
                return false;
            }
        }

        public static bool TrySaveDefault(Main_Window window, out string path, out string error)
        {
            path = Path.Combine(".", DefaultFileName);
            return TrySave(window, path, out error);
        }

        public static bool TryLoad(Main_Window window, string path, out string error)
        {
            error = null;
            if (window == null)
            {
                error = "Main window is unavailable.";
                return false;
            }

            if (string.IsNullOrWhiteSpace(path) || !File.Exists(path))
            {
                error = "Run hash file was not found.";
                return false;
            }

            string text;
            try
            {
                text = File.ReadAllText(path, Encoding.UTF8);
            }
            catch (Exception ex)
            {
                error = "Could not read run hash file: " + ex.Message;
                return false;
            }

            Dictionary<string, string> fields = ParseFields(text);
            string version;
            if (!fields.TryGetValue("Version", out version)
                || !string.Equals(version, Version, StringComparison.Ordinal))
            {
                error = "Unsupported or missing run hash file version.";
                return false;
            }

            string settingsToken;
            if (!fields.TryGetValue("SettingsToken", out settingsToken)
                || string.IsNullOrWhiteSpace(settingsToken))
            {
                error = "Run hash file is missing SettingsToken.";
                return false;
            }

            if (!SettingsShareToken.TryApply(window, settingsToken, out error))
            {
                return false;
            }

            string expectedSettingsHash;
            if (fields.TryGetValue("SettingsHash", out expectedSettingsHash)
                && !string.IsNullOrWhiteSpace(expectedSettingsHash)
                && !string.Equals(
                    window.ComputeSettingsHash(),
                    expectedSettingsHash.Trim(),
                    StringComparison.OrdinalIgnoreCase))
            {
                error = "Applied settings do not match SettingsHash in the run hash file.";
                return false;
            }

            string logicName;
            if (!fields.TryGetValue("Logic", out logicName) || string.IsNullOrWhiteSpace(logicName))
            {
                logicName = window.GetSelectedLogicName();
            }

            string placementsBase64;
            if (!fields.TryGetValue("PlacementsPayloadBase64", out placementsBase64))
            {
                placementsBase64 = "";
            }

            Dictionary<string, string> placements;
            string placementError;
            if (!BingoPlacementMap.TryDecodeBase64(placementsBase64, out placements, out placementError))
            {
                error = placementError ?? "Run hash file placements payload is invalid.";
                return false;
            }

            string expectedPlacementsHash;
            if (fields.TryGetValue("PlacementsHash", out expectedPlacementsHash)
                && !string.IsNullOrWhiteSpace(expectedPlacementsHash)
                && !string.Equals(
                    BingoPlacementMap.ComputeHash(placements),
                    expectedPlacementsHash.Trim(),
                    StringComparison.OrdinalIgnoreCase))
            {
                error = "Placements payload does not match PlacementsHash in the run hash file.";
                return false;
            }

            string romSeed;
            fields.TryGetValue("RomSeed", out romSeed);

            window.ApplyLoadedRunState(logicName, placements, romSeed);
            return true;
        }

        private static Dictionary<string, string> ParseFields(string text)
        {
            Dictionary<string, string> fields = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            if (string.IsNullOrEmpty(text))
            {
                return fields;
            }

            string[] lines = text.Split(new string[] { "\r\n", "\n" }, StringSplitOptions.None);
            for (int i = 0; i < lines.Length; i++)
            {
                string line = lines[i].Trim();
                if (line.Length == 0 || line.StartsWith("#"))
                {
                    continue;
                }

                int separator = line.IndexOf('=');
                if (separator <= 0)
                {
                    continue;
                }

                string key = line.Substring(0, separator).Trim();
                string value = line.Substring(separator + 1);
                fields[key] = value;
            }

            return fields;
        }
    }
}
