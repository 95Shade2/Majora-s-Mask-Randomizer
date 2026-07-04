using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class BingoGoalValidator
    {
        internal const string PoolHashVersion = "MMBINGO_V1";
        internal const string DerivedAccessVersion = "DERIVED_V1";

        public static HashSet<string> GetObtainableItems(Main_Window window)
        {
            HashSet<string> items = new HashSet<string>();
            AddSlotObtainableItems(items, window);
            AddPlandoRewards(items, window);
            ExpandDerivedAccess(items);
            return items;
        }

        public static bool IsGoalFeasible(BingoGoal goal, HashSet<string> obtainable)
        {
            if (goal == null)
            {
                return false;
            }

            if (goal.RequiredItems != null && goal.RequiredItems.Length > 0)
            {
                for (int i = 0; i < goal.RequiredItems.Length; i++)
                {
                    if (!obtainable.Contains(goal.RequiredItems[i]))
                    {
                        return false;
                    }
                }
            }

            if (goal.RequiredAnyOf != null && goal.RequiredAnyOf.Length > 0)
            {
                bool anyGroup = false;
                for (int i = 0; i < goal.RequiredAnyOf.Length; i++)
                {
                    string[] group = goal.RequiredAnyOf[i];
                    if (group == null || group.Length == 0)
                    {
                        continue;
                    }

                    bool groupOk = true;
                    for (int j = 0; j < group.Length; j++)
                    {
                        if (!obtainable.Contains(group[j]))
                        {
                            groupOk = false;
                            break;
                        }
                    }

                    if (groupOk)
                    {
                        anyGroup = true;
                        break;
                    }
                }

                if (!anyGroup)
                {
                    return false;
                }
            }

            return true;
        }

        public static string DescribeInfeasibility(BingoGoal goal, HashSet<string> obtainable)
        {
            if (goal == null)
            {
                return "Unknown goal";
            }

            List<string> missing = new List<string>();

            if (goal.RequiredItems != null)
            {
                for (int i = 0; i < goal.RequiredItems.Length; i++)
                {
                    if (!obtainable.Contains(goal.RequiredItems[i]))
                    {
                        missing.Add(goal.RequiredItems[i]);
                    }
                }
            }

            if (goal.RequiredAnyOf != null && goal.RequiredAnyOf.Length > 0)
            {
                bool anyGroup = false;
                List<string> groupSummaries = new List<string>();
                for (int i = 0; i < goal.RequiredAnyOf.Length; i++)
                {
                    string[] group = goal.RequiredAnyOf[i];
                    if (group == null || group.Length == 0)
                    {
                        continue;
                    }

                    List<string> groupMissing = new List<string>();
                    for (int j = 0; j < group.Length; j++)
                    {
                        if (!obtainable.Contains(group[j]))
                        {
                            groupMissing.Add(group[j]);
                        }
                    }

                    if (groupMissing.Count == 0)
                    {
                        anyGroup = true;
                        break;
                    }

                    groupSummaries.Add("(" + string.Join(" + ", group) + " missing: " + string.Join(", ", groupMissing) + ")");
                }

                if (!anyGroup)
                {
                    missing.Add("need any of: " + string.Join(" OR ", groupSummaries));
                }
            }

            if (missing.Count == 0)
            {
                return null;
            }

            return "\"" + goal.Name + "\" missing " + string.Join("; ", missing);
        }

        public static string DescribeInfeasibleGoals(BingoCard card, HashSet<string> obtainable)
        {
            if (card == null || card.GoalObjects == null)
            {
                return "Board is incomplete.";
            }

            List<string> issues = new List<string>();
            for (int i = 0; i < card.GoalObjects.Length; i++)
            {
                BingoGoal goal = card.GoalObjects[i];
                if (goal != null && !IsGoalFeasible(goal, obtainable))
                {
                    string detail = DescribeInfeasibility(goal, obtainable);
                    if (detail != null)
                    {
                        issues.Add(detail);
                    }
                }
            }

            if (issues.Count == 0)
            {
                return null;
            }

            return string.Join("; ", issues);
        }

        public static List<BingoGoal> GetValidGoals(IList<BingoGoal> allGoals, HashSet<string> obtainable)
        {
            List<BingoGoal> valid = new List<BingoGoal>();
            for (int i = 0; i < allGoals.Count; i++)
            {
                if (IsGoalFeasible(allGoals[i], obtainable))
                {
                    valid.Add(allGoals[i]);
                }
            }

            return valid;
        }

        public static Dictionary<int, List<BingoGoal>> FilterGoalsByTier(
            IList<BingoGoal> allGoals,
            HashSet<string> obtainable)
        {
            Dictionary<int, List<BingoGoal>> tiers = new Dictionary<int, List<BingoGoal>>();
            for (int i = 0; i < allGoals.Count; i++)
            {
                BingoGoal goal = allGoals[i];
                if (!IsGoalFeasible(goal, obtainable))
                {
                    continue;
                }

                if (!tiers.ContainsKey(goal.Difficulty))
                {
                    tiers[goal.Difficulty] = new List<BingoGoal>();
                }

                tiers[goal.Difficulty].Add(goal);
            }

            return tiers;
        }

        public static string ComputePoolHashV1(HashSet<string> obtainable)
        {
            string[] sorted = obtainable.OrderBy(x => x, StringComparer.Ordinal).ToArray();
            string payload = string.Join("\n", sorted) + DerivedAccessVersion + PoolHashVersion;
            using (SHA256 sha = SHA256.Create())
            {
                byte[] hash = sha.ComputeHash(Encoding.UTF8.GetBytes(payload));
                StringBuilder hex = new StringBuilder(hash.Length * 2);
                for (int i = 0; i < hash.Length; i++)
                {
                    hex.Append(hash[i].ToString("x2"));
                }

                return hex.ToString().Substring(0, 16);
            }
        }

        private static void AddSlotObtainableItems(HashSet<string> items, Main_Window window)
        {
            if (window.Item_Names == null || window.ItemObjectsForBingo == null)
            {
                return;
            }

            for (int i = 0; i < window.Item_Names.Length; i++)
            {
                string name = window.Item_Names[i];
                if (!window.ItemObjectsForBingo.ContainsKey(name))
                {
                    continue;
                }

                if (window.IsPlandoLocationForBingo(name))
                {
                    continue;
                }

                Item row = window.ItemObjectsForBingo[name];
                CheckBox check = row.Get_Checkbox();
                ComboBox pool = row.Get_Pool();
                ComboBox gives = row.Get_Gives();

                if (!check.Checked)
                {
                    items.Add(name);
                    continue;
                }

                if (gives.SelectedIndex >= 0 && !string.IsNullOrEmpty(gives.Text))
                {
                    items.Add(gives.Text);
                    continue;
                }

                if (pool.SelectedIndex >= 0 && !string.IsNullOrEmpty(pool.Text))
                {
                    int added = AddPoolMemberItems(items, window, pool.Text);
                    if (added == 0)
                    {
                        items.Add(name);
                    }
                }
            }
        }

        private static int AddPoolMemberItems(HashSet<string> items, Main_Window window, string poolName)
        {
            Dictionary<string, Dictionary<int, string>> pools = window.ItemPoolsForBingo;
            if (pools == null || !pools.ContainsKey(poolName))
            {
                return 0;
            }

            int added = 0;
            Dictionary<int, string> members = pools[poolName];
            foreach (KeyValuePair<int, string> entry in members)
            {
                if (!string.IsNullOrEmpty(entry.Value) && items.Add(entry.Value))
                {
                    added++;
                }
            }

            return added;
        }

        private static void AddPlandoRewards(HashSet<string> items, Main_Window window)
        {
            if (window.PlandoItemsForBingo == null)
            {
                return;
            }

            foreach (KeyValuePair<string, string> entry in window.PlandoItemsForBingo)
            {
                if (!string.IsNullOrEmpty(entry.Value))
                {
                    items.Add(entry.Value);
                }
            }
        }

        private static void ExpandDerivedAccess(HashSet<string> items)
        {
            ExpandLayer1_ItemEquivalence(items);
            ExpandLayer2_RegionUnlocks(items);
            ExpandLayer3_DungeonEligibility(items);
        }

        private static void ExpandLayer1_ItemEquivalence(HashSet<string> items)
        {
            if (items.Contains("Zora Mask"))
            {
                items.Add("underwater traversal");
            }

            if (items.Contains("Hookshot"))
            {
                items.Add("ledges access");
            }

            if (items.Contains("Deku Mask"))
            {
                items.Add("deku traversal");
            }

            if (items.Contains("Goron Mask"))
            {
                items.Add("goron traversal");
            }
        }

        private static void ExpandLayer2_RegionUnlocks(HashSet<string> items)
        {
            if (CanAddToken(items, "Hookshot", "Epona's Song"))
            {
                items.Add("ikana canyon access");
            }
        }

        private static void ExpandLayer3_DungeonEligibility(HashSet<string> items)
        {
            if (CanAddToken(items, "Hookshot", "Zora Mask"))
            {
                items.Add("great bay temple access");
            }

            if (CanAddToken(items, "Powder Keg", "Goron Mask"))
            {
                items.Add("snowhead temple access");
            }
        }

        private static bool CanAddToken(HashSet<string> items, string a, string b)
        {
            return items.Contains(a) && items.Contains(b);
        }
    }
}
