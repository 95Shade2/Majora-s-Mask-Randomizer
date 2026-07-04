using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal enum PoolIssueType
    {
        StrandedItem,
        ConsumedReward,
        StalePoolLocation
    }

    internal sealed class PoolIssue
    {
        public PoolIssueType Type { get; set; }
        public string Key { get; set; }
        public string Message { get; set; }
        public string Location { get; set; }
        public string Item { get; set; }
    }

    internal static class PlandoUx
    {
        public static string MakeIssueKey(PoolIssueType type, string location, string item)
        {
            return type + "|" + location + "|" + item;
        }

        public static List<PoolIssue> DetectIssues(
            Dictionary<string, string> plandoItems,
            Dictionary<string, Item> itemObjects,
            Dictionary<string, Dictionary<int, string>> itemPools,
            string[] itemNames,
            HashSet<string> ignored)
        {
            List<PoolIssue> issues = new List<PoolIssue>();

            if (plandoItems == null || plandoItems.Count == 0)
            {
                return issues;
            }

            if (ignored == null)
            {
                ignored = new HashSet<string>();
            }

            foreach (KeyValuePair<string, string> entry in plandoItems)
            {
                string location = entry.Key;
                string reward = entry.Value;

                string strandedKey = MakeIssueKey(PoolIssueType.StrandedItem, location, location);
                if (!ignored.Contains(strandedKey) &&
                    !IsItemPlacedElsewhere(location, plandoItems, itemObjects, itemNames))
                {
                    issues.Add(new PoolIssue
                    {
                        Type = PoolIssueType.StrandedItem,
                        Key = strandedKey,
                        Location = location,
                        Item = location,
                        Message = "Stranded: " + location + " is plando'd away and not placed elsewhere"
                    });
                }

                string staleKey = MakeIssueKey(PoolIssueType.StalePoolLocation, location, location);
                if (!ignored.Contains(staleKey) && IsInAnyPool(location, itemPools))
                {
                    issues.Add(new PoolIssue
                    {
                        Type = PoolIssueType.StalePoolLocation,
                        Key = staleKey,
                        Location = location,
                        Item = location,
                        Message = "Stale pool: " + location + " is plando'd but still listed in a pool"
                    });
                }

                string consumedKey = MakeIssueKey(PoolIssueType.ConsumedReward, reward, reward);
                if (!ignored.Contains(consumedKey) && IsInAnyPool(reward, itemPools))
                {
                    issues.Add(new PoolIssue
                    {
                        Type = PoolIssueType.ConsumedReward,
                        Key = consumedKey,
                        Location = location,
                        Item = reward,
                        Message = "Consumed: " + reward + " is used by plando but still in a pool"
                    });
                }
            }

            return issues;
        }

        public static bool IsInAnyPool(string item, Dictionary<string, Dictionary<int, string>> pools)
        {
            if (pools == null || item == "")
            {
                return false;
            }

            foreach (KeyValuePair<string, Dictionary<int, string>> pool in pools)
            {
                foreach (KeyValuePair<int, string> member in pool.Value)
                {
                    if (member.Value == item)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        public static bool IsItemPlacedElsewhere(
            string item,
            Dictionary<string, string> plandoItems,
            Dictionary<string, Item> itemObjects,
            string[] itemNames)
        {
            if (plandoItems != null)
            {
                foreach (KeyValuePair<string, string> entry in plandoItems)
                {
                    if (entry.Value == item)
                    {
                        return true;
                    }
                }
            }

            if (itemObjects == null || itemNames == null)
            {
                return false;
            }

            for (int i = 0; i < itemNames.Length; i++)
            {
                string rowName = itemNames[i];
                CheckBox check = itemObjects[rowName].Get_Checkbox();
                ComboBox gives = itemObjects[rowName].Get_Gives();

                if (check.Checked && gives.SelectedIndex != -1 && gives.Text == item)
                {
                    return true;
                }
            }

            return false;
        }
    }
}
