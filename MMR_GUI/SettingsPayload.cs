using System;
using System.Collections.Generic;
using System.Text;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class SettingsPayload
    {
        public static string EncodeBase64(string canonicalPayload)
        {
            return Convert.ToBase64String(Encoding.UTF8.GetBytes(canonicalPayload));
        }

        public static string DecodeBase64(string base64)
        {
            if (string.IsNullOrWhiteSpace(base64))
            {
                return "";
            }

            return Encoding.UTF8.GetString(Convert.FromBase64String(base64.Trim()));
        }

        public static Dictionary<string, Dictionary<string, string>> ParseIni(string text)
        {
            Dictionary<string, Dictionary<string, string>> contents =
                new Dictionary<string, Dictionary<string, string>>();
            string section = null;

            if (string.IsNullOrEmpty(text))
            {
                return contents;
            }

            string[] lines = text.Split(new string[] { "\r\n", "\n" }, StringSplitOptions.None);
            for (int i = 0; i < lines.Length; i++)
            {
                string line = lines[i].Trim();
                if (line.Length == 0)
                {
                    continue;
                }

                if (line.StartsWith("[") && line.EndsWith("]"))
                {
                    section = line.Substring(1, line.Length - 2);
                    if (!contents.ContainsKey(section))
                    {
                        contents[section] = new Dictionary<string, string>();
                    }

                    continue;
                }

                int separator = line.IndexOf('=');
                if (separator < 0 || section == null)
                {
                    continue;
                }

                string key = line.Substring(0, separator);
                string value = line.Substring(separator + 1);
                contents[section][key] = value;
            }

            return contents;
        }
    }
}
