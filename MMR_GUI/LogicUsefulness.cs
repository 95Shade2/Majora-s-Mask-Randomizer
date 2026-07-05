using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class LogicUsefulnessResult
    {
        public Dictionary<string, int> UnlockCounts { get; private set; }
        public Dictionary<string, List<string>> ReverseMap { get; private set; }
        public Dictionary<string, List<List<string>>> LocationPrereqs { get; private set; }

        public LogicUsefulnessResult(
            Dictionary<string, int> unlockCounts,
            Dictionary<string, List<string>> reverseMap,
            Dictionary<string, List<List<string>>> locationPrereqs)
        {
            UnlockCounts = unlockCounts ?? new Dictionary<string, int>();
            ReverseMap = reverseMap ?? new Dictionary<string, List<string>>();
            LocationPrereqs = locationPrereqs ?? new Dictionary<string, List<List<string>>>();
        }

        public bool IsLocationReachable(
            string location,
            HashSet<string> acquiredItems,
            Dictionary<string, int> grantCounts = null)
        {
            List<List<string>> sets;
            if (!LocationPrereqs.TryGetValue(location, out sets) || sets.Count == 0)
            {
                return true;
            }

            foreach (List<string> set in sets)
            {
                if (IsPrereqSetSatisfied(set, acquiredItems, grantCounts))
                {
                    return true;
                }
            }

            return false;
        }

        public bool LocationRequiresItem(string location, string itemName)
        {
            List<List<string>> sets;
            if (itemName == "" || !LocationPrereqs.TryGetValue(location, out sets))
            {
                return false;
            }

            foreach (List<string> set in sets)
            {
                foreach (string rawPrereq in set)
                {
                    int countNeeded;
                    string parsedName;
                    if (TryParsePrereq(rawPrereq, out countNeeded, out parsedName) && parsedName == itemName)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        public bool ParentItemUnlocksLocation(
            string location,
            string parentGrantedItem,
            HashSet<string> acquiredItems,
            Dictionary<string, int> grantCounts = null)
        {
            if (parentGrantedItem == "")
            {
                return false;
            }

            List<List<string>> sets;
            if (!LocationPrereqs.TryGetValue(location, out sets))
            {
                return false;
            }

            foreach (List<string> set in sets)
            {
                if (ParentItemUnlocksLocationForSet(
                    location,
                    parentGrantedItem,
                    set,
                    acquiredItems,
                    grantCounts))
                {
                    return true;
                }
            }

            return false;
        }

        public bool ParentItemUnlocksLocationForSet(
            string location,
            string parentGrantedItem,
            List<string> set,
            HashSet<string> acquiredItems,
            Dictionary<string, int> grantCounts = null)
        {
            if (parentGrantedItem == "" || set == null || set.Count == 0)
            {
                return false;
            }

            if (!IsPrereqSetSatisfied(set, acquiredItems, grantCounts))
            {
                return false;
            }

            foreach (string rawPrereq in set)
            {
                int countNeeded;
                string itemName;
                if (TryParsePrereq(rawPrereq, out countNeeded, out itemName)
                    && ItemGrantCounts.ItemsMatch(itemName, parentGrantedItem))
                {
                    return true;
                }
            }

            if (IsRupeesOnlySet(set))
            {
                return WalletItemEnablesRupees(parentGrantedItem, GetMaxRupeesInSet(set));
            }

            return false;
        }

        public static int GetMaxRupeesInSet(List<string> set)
        {
            int maxRupees = 0;

            if (set == null)
            {
                return maxRupees;
            }

            foreach (string rawPrereq in set)
            {
                int countNeeded;
                string itemName;
                if (TryParsePrereq(rawPrereq, out countNeeded, out itemName) && itemName == "Rupees")
                {
                    maxRupees = Math.Max(maxRupees, countNeeded);
                }
            }

            return maxRupees;
        }

        public static bool WalletItemEnablesRupees(string walletItem, int rupeesRequired)
        {
            if (rupeesRequired <= 99)
            {
                return false;
            }

            if (walletItem == "Giant Wallet")
            {
                return rupeesRequired <= 500;
            }

            if (walletItem == "Adult Wallet")
            {
                return rupeesRequired <= 200;
            }

            return false;
        }

        public bool IsFreeLocation(string location)
        {
            List<List<string>> sets;
            if (!LocationPrereqs.TryGetValue(location, out sets) || sets.Count == 0)
            {
                return true;
            }

            foreach (List<string> set in sets)
            {
                if (set == null || set.Count == 0)
                {
                    return true;
                }
            }

            return false;
        }

        public bool IsRupeesOnlySet(List<string> set)
        {
            if (set == null || set.Count == 0)
            {
                return false;
            }

            foreach (string rawPrereq in set)
            {
                int countNeeded;
                string itemName;
                if (!TryParsePrereq(rawPrereq, out countNeeded, out itemName) || itemName != "Rupees")
                {
                    return false;
                }
            }

            return true;
        }

        public bool IsWalletOnlyReachableAtStart(string location)
        {
            List<List<string>> sets;
            if (!LocationPrereqs.TryGetValue(location, out sets) || sets.Count == 0)
            {
                return false;
            }

            HashSet<string> emptyAcquired = new HashSet<string>();

            foreach (List<string> set in sets)
            {
                if (!IsRupeesOnlySet(set))
                {
                    continue;
                }

                if (IsPrereqSetSatisfied(set, emptyAcquired, null))
                {
                    return true;
                }
            }

            return false;
        }

        public List<string> GetFreeLocations()
        {
            return LocationPrereqs.Keys
                .Where(IsFreeLocation)
                .OrderBy(name => name)
                .ToList();
        }

        public bool IsStartingColumnLocation(string location)
        {
            return IsFreeLocation(location) || IsWalletOnlyReachableAtStart(location);
        }

        public List<string> GetStartingColumnLocations()
        {
            return LocationPrereqs.Keys
                .Where(IsStartingColumnLocation)
                .OrderBy(name => name)
                .ToList();
        }

        public bool TryGetSatisfiedPrereqItem(
            string location,
            HashSet<string> acquiredItems,
            out string prereqItem,
            Dictionary<string, int> grantCounts = null)
        {
            prereqItem = "";
            List<List<string>> sets;
            if (!LocationPrereqs.TryGetValue(location, out sets))
            {
                return false;
            }

            foreach (List<string> set in sets)
            {
                if (!IsPrereqSetSatisfied(set, acquiredItems, grantCounts))
                {
                    continue;
                }

                if (IsRupeesOnlySet(set))
                {
                    int rupeesRequired = GetMaxRupeesInSet(set);
                    if (WalletItemEnablesRupees("Giant Wallet", rupeesRequired)
                        && GetOwnedCount("Giant Wallet", acquiredItems, grantCounts) > 0)
                    {
                        prereqItem = "Giant Wallet";
                        return true;
                    }

                    if (WalletItemEnablesRupees("Adult Wallet", rupeesRequired)
                        && GetOwnedCount("Adult Wallet", acquiredItems, grantCounts) > 0)
                    {
                        prereqItem = "Adult Wallet";
                        return true;
                    }

                    continue;
                }

                foreach (string rawPrereq in set)
                {
                    int countNeeded;
                    string itemName;
                    if (!TryParsePrereq(rawPrereq, out countNeeded, out itemName))
                    {
                        continue;
                    }

                    if (itemName == "Rupees")
                    {
                        continue;
                    }

                    if (GetOwnedCount(itemName, acquiredItems, grantCounts) >= countNeeded)
                    {
                        prereqItem = itemName;
                        return true;
                    }
                }
            }

            return false;
        }

        public bool IsPrereqSetSatisfied(
            List<string> set,
            HashSet<string> acquiredItems,
            Dictionary<string, int> grantCounts = null)
        {
            if (set == null || set.Count == 0)
            {
                return true;
            }

            foreach (string rawPrereq in set)
            {
                if (!IsPrereqMet(rawPrereq, acquiredItems, grantCounts))
                {
                    return false;
                }
            }

            return true;
        }

        public static bool TryParsePrereq(string rawPrereq, out int countNeeded, out string itemName)
        {
            countNeeded = 1;
            itemName = rawPrereq != null ? rawPrereq.Trim() : "";
            if (itemName == "")
            {
                return false;
            }

            int space = itemName.IndexOf(' ');
            if (space > 0)
            {
                string prefix = itemName.Substring(0, space);
                int parsedCount;
                if (int.TryParse(prefix, out parsedCount))
                {
                    countNeeded = parsedCount;
                    itemName = itemName.Substring(space + 1).Trim();
                }
            }

            return itemName != "";
        }

        public static int GetWalletMax(HashSet<string> acquiredItems)
        {
            if (acquiredItems != null && acquiredItems.Contains("Giant Wallet"))
            {
                return 500;
            }

            if (acquiredItems != null && acquiredItems.Contains("Adult Wallet"))
            {
                return 200;
            }

            return 99;
        }

        public bool IsPrereqMet(
            string rawPrereq,
            HashSet<string> acquiredItems,
            Dictionary<string, int> grantCounts = null)
        {
            int countNeeded;
            string itemName;
            if (!TryParsePrereq(rawPrereq, out countNeeded, out itemName))
            {
                return true;
            }

            if (itemName == "Rupees")
            {
                return GetWalletMax(acquiredItems) >= countNeeded;
            }

            return GetOwnedCount(itemName, acquiredItems, grantCounts) >= countNeeded;
        }

        public static string FormatPrereqSetDisplay(List<string> set)
        {
            if (set == null || set.Count == 0)
            {
                return "(none)";
            }

            return string.Join(" + ", set);
        }

        public List<string> GetMissingPrereqsInSet(
            List<string> set,
            HashSet<string> acquiredItems,
            Dictionary<string, int> grantCounts = null)
        {
            List<string> missing = new List<string>();

            if (set == null || set.Count == 0)
            {
                return missing;
            }

            foreach (string rawPrereq in set)
            {
                int countNeeded;
                string itemName;
                if (!TryParsePrereq(rawPrereq, out countNeeded, out itemName))
                {
                    continue;
                }

                if (itemName == "Rupees")
                {
                    if (GetWalletMax(acquiredItems) < countNeeded)
                    {
                        missing.Add(countNeeded + " Rupees (wallet " + GetWalletMax(acquiredItems) + ")");
                    }

                    continue;
                }

                int owned = GetOwnedCount(itemName, acquiredItems, grantCounts);
                if (owned < countNeeded)
                {
                    if (countNeeded > 1)
                    {
                        missing.Add(countNeeded + " " + itemName + " (have " + owned + ")");
                    }
                    else
                    {
                        missing.Add(itemName);
                    }
                }
            }

            return missing;
        }

        public static int GetOwnedCount(
            string itemName,
            HashSet<string> acquiredItems,
            Dictionary<string, int> grantCounts)
        {
            if (itemName == "")
            {
                return 0;
            }

            if (grantCounts != null)
            {
                int total = 0;
                foreach (KeyValuePair<string, int> entry in grantCounts)
                {
                    if (ItemGrantCounts.ItemsMatch(itemName, entry.Key))
                    {
                        total += entry.Value;
                    }
                }

                if (total > 0)
                {
                    return total;
                }
            }

            if (acquiredItems != null)
            {
                int total = 0;
                foreach (string acquired in acquiredItems)
                {
                    if (ItemGrantCounts.ItemsMatch(itemName, acquired))
                    {
                        total += 1;
                    }
                }

                return total;
            }

            return 0;
        }

        public static string StripCountPrefix(string item)
        {
            int space = item.IndexOf(' ');
            if (space > 0)
            {
                string prefix = item.Substring(0, space);
                int count;
                if (int.TryParse(prefix, out count))
                {
                    return item.Substring(space + 1).Trim();
                }
            }

            return item.Trim();
        }
    }

    internal static class LogicUsefulness
    {
        public static LogicUsefulnessResult Compute(string logicName, IEnumerable<string> itemNames)
        {
            Dictionary<string, int> unlockCounts = new Dictionary<string, int>();
            Dictionary<string, HashSet<string>> reverseSets = new Dictionary<string, HashSet<string>>();

            if (itemNames != null)
            {
                foreach (string item in itemNames)
                {
                    unlockCounts[item] = 0;
                }
            }

            if (string.IsNullOrWhiteSpace(logicName))
            {
                return new LogicUsefulnessResult(
                    unlockCounts,
                    new Dictionary<string, List<string>>(),
                    new Dictionary<string, List<List<string>>>());
            }

            string logicPath = "./logic/" + logicName + ".txt";
            if (!File.Exists(logicPath))
            {
                return new LogicUsefulnessResult(
                    unlockCounts,
                    new Dictionary<string, List<string>>(),
                    new Dictionary<string, List<List<string>>>());
            }

            Dictionary<string, List<List<string>>> locationPrereqs = ParseLogicFile(logicPath);

            foreach (KeyValuePair<string, List<List<string>>> entry in locationPrereqs)
            {
                string location = entry.Key;
                HashSet<string> prereqsForLocation = new HashSet<string>();

                foreach (List<string> set in entry.Value)
                {
                    foreach (string rawPrereq in set)
                    {
                        string prereq = LogicUsefulnessResult.StripCountPrefix(rawPrereq);
                        if (prereq == "")
                        {
                            continue;
                        }

                        prereqsForLocation.Add(prereq);
                    }
                }

                foreach (string prereq in prereqsForLocation)
                {
                    if (!reverseSets.ContainsKey(prereq))
                    {
                        reverseSets[prereq] = new HashSet<string>();
                    }

                    reverseSets[prereq].Add(location);
                }
            }

            foreach (KeyValuePair<string, HashSet<string>> entry in reverseSets)
            {
                unlockCounts[entry.Key] = entry.Value.Count;
            }

            Dictionary<string, List<string>> reverseMap = reverseSets.ToDictionary(
                pair => pair.Key,
                pair => pair.Value.OrderBy(name => name).ToList());

            return new LogicUsefulnessResult(unlockCounts, reverseMap, locationPrereqs);
        }

        public static Dictionary<string, string> ParseSpoilerLog(string path)
        {
            Dictionary<string, string> placements = new Dictionary<string, string>();

            if (!File.Exists(path))
            {
                return placements;
            }

            foreach (string rawLine in File.ReadAllLines(path))
            {
                string line = rawLine.Trim();
                if (!line.Contains("=>"))
                {
                    continue;
                }

                int arrow = line.IndexOf("=>", StringComparison.Ordinal);
                if (arrow < 0)
                {
                    continue;
                }

                string slot = line.Substring(0, arrow).Trim();
                string placed = line.Substring(arrow + 2).Trim();
                if (slot == "" || placed == "" || slot == "Old Item" || slot.StartsWith("Seed"))
                {
                    continue;
                }

                placements[slot] = placed;
            }

            return placements;
        }

        public static string ParseSpoilerSeed(string path)
        {
            if (!File.Exists(path))
            {
                return "";
            }

            foreach (string rawLine in File.ReadAllLines(path))
            {
                string line = rawLine.Trim();
                if (line.StartsWith("Seed:", StringComparison.OrdinalIgnoreCase))
                {
                    return line.Substring("Seed:".Length).Trim();
                }

                if (line.Length > 0)
                {
                    break;
                }
            }

            return "";
        }

        private static Dictionary<string, List<List<string>>> ParseLogicFile(string logicPath)
        {
            Dictionary<string, List<List<string>>> locationPrereqs =
                new Dictionary<string, List<List<string>>>();
            string location = "";
            List<List<string>> currentSets = null;

            foreach (string rawLine in File.ReadAllLines(logicPath))
            {
                string line = rawLine;
                if (line.Contains("//"))
                {
                    line = line.Substring(0, line.IndexOf("//", StringComparison.Ordinal));
                }

                line = line.Replace("\t", "").Trim();
                if (line == "")
                {
                    continue;
                }

                if (location == "")
                {
                    int brace = line.IndexOf('{');
                    if (brace < 0)
                    {
                        continue;
                    }

                    location = line.Substring(0, brace).Trim();
                    currentSets = new List<List<string>>();
                    locationPrereqs[location] = currentSets;
                    continue;
                }

                if (line == "}")
                {
                    location = "";
                    currentSets = null;
                    continue;
                }

                if (line[0] == '#' || line[0] == '@')
                {
                    continue;
                }

                if (currentSets == null)
                {
                    continue;
                }

                List<string> set = SplitPrereqLine(line);
                if (set.Count > 0)
                {
                    currentSets.Add(set);
                }
            }

            return locationPrereqs;
        }

        private static List<string> SplitPrereqLine(string line)
        {
            if (line.Contains(", "))
            {
                return line.Split(new[] { ", " }, StringSplitOptions.None)
                    .Select(part => part.Trim())
                    .Where(part => part != "")
                    .ToList();
            }

            if (line != "")
            {
                return new List<string> { line };
            }

            return new List<string>();
        }

    }
}
