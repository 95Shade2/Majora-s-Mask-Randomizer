using System;
using System.Collections.Generic;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class LogicObtainability
    {
        public static bool IsLogicEnabled(string logicName)
        {
            return !string.IsNullOrWhiteSpace(logicName)
                && !string.Equals(logicName, "None", StringComparison.OrdinalIgnoreCase);
        }

        public static HashSet<string> Compute(
            LogicUsefulnessResult logic,
            Dictionary<string, string> placements)
        {
            HashSet<string> acquired = new HashSet<string>(StringComparer.Ordinal);
            if (logic == null || logic.LocationPrereqs == null || logic.LocationPrereqs.Count == 0)
            {
                return acquired;
            }

            Dictionary<string, int> grantCounts = new Dictionary<string, int>(StringComparer.Ordinal);
            HashSet<string> collectedLocations = new HashSet<string>(StringComparer.Ordinal);
            bool changed = true;
            int safety = 0;

            while (changed && safety < 512)
            {
                safety++;
                changed = false;

                foreach (string location in logic.LocationPrereqs.Keys)
                {
                    if (collectedLocations.Contains(location))
                    {
                        continue;
                    }

                    if (!logic.IsLocationReachable(location, acquired, grantCounts))
                    {
                        continue;
                    }

                    string placed;
                    if (!placements.TryGetValue(location, out placed) || string.IsNullOrWhiteSpace(placed))
                    {
                        continue;
                    }

                    GrantPlacement(location, placed.Trim(), acquired, grantCounts);
                    collectedLocations.Add(location);
                    changed = true;
                }
            }

            return acquired;
        }

        private static void GrantPlacement(
            string location,
            string placed,
            HashSet<string> acquired,
            Dictionary<string, int> grantCounts)
        {
            string normalized = ItemGrantCounts.GetBaseItemName(placed);
            if (normalized == "")
            {
                normalized = placed;
            }

            acquired.Add(normalized);

            int quantity = ItemGrantCounts.GetGrantQuantity(location, placed);
            if (quantity <= 0)
            {
                quantity = 1;
            }

            int count;
            if (grantCounts.TryGetValue(normalized, out count))
            {
                grantCounts[normalized] = count + quantity;
            }
            else
            {
                grantCounts[normalized] = quantity;
            }
        }
    }
}
