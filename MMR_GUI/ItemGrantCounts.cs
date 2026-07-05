using System;
using System.Collections.Generic;
using System.IO;
using System.Web.Script.Serialization;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class ItemGrantCounts
    {
        private static Dictionary<string, int> _slotCounts;
        private static bool _loaded;

        public static int GetSlotCount(string slotName)
        {
            EnsureLoaded();
            if (slotName == "" || _slotCounts == null)
            {
                return 1;
            }

            int count;
            if (_slotCounts.TryGetValue(slotName, out count))
            {
                return count;
            }

            return 1;
        }

        public static int GetParentheticalMultiplier(string placementName)
        {
            if (placementName == "")
            {
                return 1;
            }

            int open = placementName.LastIndexOf('(');
            int close = placementName.LastIndexOf(')');
            if (open < 0 || close <= open)
            {
                return 1;
            }

            int mult;
            if (int.TryParse(placementName.Substring(open + 1, close - open - 1).Trim(), out mult)
                && mult > 0)
            {
                return mult;
            }

            return 1;
        }

        public static string GetBaseItemName(string itemName)
        {
            if (itemName == "")
            {
                return "";
            }

            string stripped = LogicUsefulnessResult.StripCountPrefix(itemName);
            int open = stripped.LastIndexOf('(');
            int close = stripped.LastIndexOf(')');
            if (open > 0 && close > open)
            {
                return stripped.Substring(0, open).Trim();
            }

            return stripped;
        }

        public static bool ItemsMatch(string logicItemName, string placementName)
        {
            if (logicItemName == "" || placementName == "")
            {
                return false;
            }

            if (string.Equals(logicItemName, placementName, StringComparison.Ordinal))
            {
                return true;
            }

            string logicBase = GetBaseItemName(logicItemName);
            string placementBase = GetBaseItemName(placementName);
            return string.Equals(logicBase, placementBase, StringComparison.Ordinal);
        }

        public static int GetGrantQuantity(string slotName, string placementName)
        {
            if (placementName == "")
            {
                return 0;
            }

            int slotCount = GetSlotCount(slotName);
            if (slotCount <= 0)
            {
                slotCount = 1;
            }

            return slotCount * GetParentheticalMultiplier(placementName);
        }

        private static void EnsureLoaded()
        {
            if (_loaded)
            {
                return;
            }

            _loaded = true;
            _slotCounts = new Dictionary<string, int>(StringComparer.Ordinal);

            string path = FindDataFile("item_slot_counts.json");
            if (path == null || !File.Exists(path))
            {
                return;
            }

            try
            {
                string json = File.ReadAllText(path);
                var serializer = new JavaScriptSerializer();
                Dictionary<string, int> parsed =
                    serializer.Deserialize<Dictionary<string, int>>(json);
                if (parsed != null)
                {
                    foreach (KeyValuePair<string, int> entry in parsed)
                    {
                        _slotCounts[entry.Key] = entry.Value;
                    }
                }
            }
            catch
            {
                _slotCounts = new Dictionary<string, int>(StringComparer.Ordinal);
            }
        }

        private static string FindDataFile(string fileName)
        {
            string[] candidates =
            {
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Data", fileName),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, fileName),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "Data", fileName),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "Data", fileName),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "MMR_GUI", "Data", fileName),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "..", "..", "MMR_GUI", "Data", fileName)
            };

            foreach (string candidate in candidates)
            {
                string full = Path.GetFullPath(candidate);
                if (File.Exists(full))
                {
                    return full;
                }
            }

            return null;
        }
    }
}
