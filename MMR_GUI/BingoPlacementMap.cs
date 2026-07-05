using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class BingoPlacementMap
    {
        internal const string Version = "MMBINGO_PLACEMENTS_V1";

        public static Dictionary<string, string> BuildFromWindow(Main_Window window)
        {
            Dictionary<string, string> map = new Dictionary<string, string>(StringComparer.Ordinal);

            if (window == null)
            {
                return map;
            }

            foreach (KeyValuePair<string, string> entry in LogicUsefulness.ParseSpoilerLog("./Spoiler Log.txt"))
            {
                map[entry.Key] = entry.Value;
            }

            if (window.Item_Names != null && window.ItemObjectsForBingo != null)
            {
                for (int i = 0; i < window.Item_Names.Length; i++)
                {
                    string slotName = window.Item_Names[i];
                    if (map.ContainsKey(slotName))
                    {
                        continue;
                    }

                    if (window.IsPlandoLocationForBingo(slotName))
                    {
                        continue;
                    }

                    Item row;
                    if (!window.ItemObjectsForBingo.TryGetValue(slotName, out row) || row == null)
                    {
                        continue;
                    }

                    CheckBox check = row.Get_Checkbox();
                    if (check != null && !check.Checked)
                    {
                        map[slotName] = slotName;
                    }
                }
            }

            if (window.PlandoItemsForBingo != null)
            {
                foreach (KeyValuePair<string, string> entry in window.PlandoItemsForBingo)
                {
                    if (!string.IsNullOrEmpty(entry.Key) && !string.IsNullOrEmpty(entry.Value))
                    {
                        map[entry.Key] = entry.Value;
                    }
                }
            }

            return map;
        }

        public static string BuildCanonicalPayload(Dictionary<string, string> placements)
        {
            if (placements == null || placements.Count == 0)
            {
                return Version + "\n";
            }

            StringBuilder text = new StringBuilder();
            text.Append(Version).Append('\n');
            foreach (KeyValuePair<string, string> entry in placements.OrderBy(p => p.Key, StringComparer.Ordinal))
            {
                text.Append(entry.Key).Append('=').Append(entry.Value ?? "").Append('\n');
            }

            return text.ToString();
        }

        public static string EncodeBase64(Dictionary<string, string> placements)
        {
            return SettingsPayload.EncodeBase64(BuildCanonicalPayload(placements));
        }

        public static bool TryDecodeBase64(string base64, out Dictionary<string, string> placements, out string error)
        {
            placements = new Dictionary<string, string>(StringComparer.Ordinal);
            error = null;

            if (string.IsNullOrWhiteSpace(base64))
            {
                return true;
            }

            string text;
            try
            {
                text = SettingsPayload.DecodeBase64(base64);
            }
            catch (FormatException)
            {
                error = "Placement payload is invalid base64.";
                return false;
            }

            return TryParseCanonical(text, out placements, out error);
        }

        public static bool TryParseCanonical(string text, out Dictionary<string, string> placements, out string error)
        {
            placements = new Dictionary<string, string>(StringComparer.Ordinal);
            error = null;

            if (string.IsNullOrEmpty(text))
            {
                return true;
            }

            string[] lines = text.Split(new string[] { "\r\n", "\n" }, StringSplitOptions.None);
            if (lines.Length == 0)
            {
                return true;
            }

            if (!string.Equals(lines[0].Trim(), Version, StringComparison.Ordinal))
            {
                error = "Unsupported placement payload version.";
                return false;
            }

            for (int i = 1; i < lines.Length; i++)
            {
                string line = lines[i].Trim();
                if (line.Length == 0)
                {
                    continue;
                }

                int separator = line.IndexOf('=');
                if (separator <= 0)
                {
                    continue;
                }

                string key = line.Substring(0, separator);
                string value = line.Substring(separator + 1);
                placements[key] = value;
            }

            return true;
        }

        public static string ComputeHash(Dictionary<string, string> placements)
        {
            return SettingsHash.Compute(BuildCanonicalPayload(placements));
        }
    }
}
