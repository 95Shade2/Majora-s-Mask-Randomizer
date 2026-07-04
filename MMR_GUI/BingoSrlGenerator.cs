using System;
using System.Collections.Generic;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class BingoSrlGenerator
    {
        private const double BorrowedTierWeight = 0.85;
        private const int MaxTypeConflictRetries = 256;

        private static readonly int[,] MagicSquare = new int[,]
        {
            { 15,  8,  1, 24, 17 },
            { 16, 14,  7,  5, 23 },
            { 22, 20, 13, 19, 11 },
            {  3,  4, 25, 12, 21 },
            { 10,  9, 18,  6,  2 }
        };

        public static Dictionary<int, List<BingoGoal>> DeepClone(Dictionary<int, List<BingoGoal>> tiers)
        {
            Dictionary<int, List<BingoGoal>> clone = new Dictionary<int, List<BingoGoal>>();
            foreach (KeyValuePair<int, List<BingoGoal>> entry in tiers)
            {
                List<BingoGoal> list = new List<BingoGoal>();
                for (int i = 0; i < entry.Value.Count; i++)
                {
                    list.Add(entry.Value[i].Clone());
                }

                clone[entry.Key] = list;
            }

            return clone;
        }

        public static bool PassesHeuristicPreChecks(Dictionary<int, List<BingoGoal>> tiers)
        {
            return DescribeHeuristicFailures(tiers).Count == 0;
        }

        public static List<string> DescribeHeuristicFailures(Dictionary<int, List<BingoGoal>> tiers)
        {
            List<string> failures = new List<string>();

            string duplicate = DescribeDuplicateGoals(tiers);
            if (duplicate != null)
            {
                failures.Add(duplicate);
            }

            for (int tier = 1; tier <= 25; tier++)
            {
                if (ResolveTier(tiers, tier).Count == 0)
                {
                    failures.Add("Tier " + tier + " has no feasible goals (including ±2 borrowed tiers).");
                }
            }

            return failures;
        }

        public static string DescribeDryRunFailure(Dictionary<int, List<BingoGoal>> tiers, int seed)
        {
            Dictionary<int, List<BingoGoal>> safeTiers = DeepClone(tiers);
            Random rng = new Random(seed);
            Dictionary<int, BingoGoal> picks = PickGoalsForTiers(safeTiers, rng);
            if (picks.Count < 25)
            {
                return "Could only pick " + picks.Count + " of 25 unique tier goals for this seed.";
            }

            return "Could not satisfy SRL row type diversity within "
                + MaxTypeConflictRetries
                + " attempts for this seed.";
        }

        public static string DescribeTypeConflicts(BingoGoal[] goals)
        {
            return DescribeTypeConflicts(goals, true);
        }

        public static string DescribeTypeConflicts(BingoGoal[] goals, bool includeColumnsAndDiagonals)
        {
            if (goals == null || goals.Length != 25)
            {
                return "Board is incomplete.";
            }

            List<string> conflicts = new List<string>();
            for (int row = 0; row < 5; row++)
            {
                string detail = DescribeLineTypeConflict(goals, "ROW-" + (row + 1), row * 5, 1, 5);
                if (detail != null)
                {
                    conflicts.Add(detail);
                }
            }

            if (includeColumnsAndDiagonals)
            {
                for (int col = 0; col < 5; col++)
                {
                    string detail = DescribeLineTypeConflict(goals, "COL-" + (col + 1), col, 5, 5);
                    if (detail != null)
                    {
                        conflicts.Add(detail);
                    }
                }

                string tlbr = DescribeLineTypeConflict(goals, "TL-BR", 0, 6, 5);
                if (tlbr != null)
                {
                    conflicts.Add(tlbr);
                }

                string bltr = DescribeLineTypeConflict(goals, "BL-TR", 4, 4, 5);
                if (bltr != null)
                {
                    conflicts.Add(bltr);
                }
            }

            if (conflicts.Count == 0)
            {
                return null;
            }

            return string.Join("; ", conflicts);
        }

        public static bool SRLDryRunValid(Dictionary<int, List<BingoGoal>> tiers, int seed)
        {
            try
            {
                Dictionary<int, List<BingoGoal>> safeTiers = DeepClone(tiers);
                BingoCard test = GenerateSrlBoard(safeTiers, seed, SrlProfile.Normal);
                return test != null && test.Goals != null && test.Goals.Length == 25;
            }
            catch
            {
                return false;
            }
        }

        public static BingoCard GenerateSrlBoard(
            Dictionary<int, List<BingoGoal>> filteredTiers,
            int seed,
            SrlProfile profile)
        {
            Random rng = new Random(seed);
            bool enforceRowTypes = profile != SrlProfile.Relaxed;

            for (int attempt = 0; attempt < MaxTypeConflictRetries; attempt++)
            {
                Dictionary<int, BingoGoal> picks = PickGoalsForTiers(filteredTiers, rng);
                if (picks.Count < 25)
                {
                    continue;
                }

                string[] grid = new string[25];
                BingoGoal[] goalObjects = new BingoGoal[25];
                for (int row = 0; row < 5; row++)
                {
                    for (int col = 0; col < 5; col++)
                    {
                        int tier = MagicSquare[row, col];
                        BingoGoal goal = picks[tier];
                        int index = row * 5 + col;
                        grid[index] = goal.Name;
                        goalObjects[index] = goal;
                    }
                }

                if (!enforceRowTypes || !HasRowTypeConflicts(goalObjects))
                {
                    return new BingoCard
                    {
                        Goals = grid,
                        GoalObjects = goalObjects,
                        GoalStates = new int[25],
                        RerollTrace = new List<RerollTraceEntry>()
                    };
                }
            }

            return null;
        }

        public static bool ValidateTypeConstraints(BingoCard card)
        {
            if (card == null || card.GoalObjects == null || card.GoalObjects.Length != 25)
            {
                return false;
            }

            return !HasRowTypeConflicts(card.GoalObjects);
        }

        private static string DescribeDuplicateGoals(Dictionary<int, List<BingoGoal>> tiers)
        {
            Dictionary<string, int> seen = new Dictionary<string, int>();
            foreach (KeyValuePair<int, List<BingoGoal>> entry in tiers)
            {
                for (int i = 0; i < entry.Value.Count; i++)
                {
                    string name = entry.Value[i].Name;
                    if (seen.ContainsKey(name))
                    {
                        return "Duplicate goal in tier pool: \"" + name + "\" (tiers "
                            + seen[name]
                            + " and "
                            + entry.Key
                            + ").";
                    }

                    seen[name] = entry.Key;
                }
            }

            return null;
        }

        private static string DescribeLineTypeConflict(
            BingoGoal[] goals,
            string lineName,
            int start,
            int step,
            int count)
        {
            Dictionary<string, string> seen = new Dictionary<string, string>();
            int index = start;
            for (int i = 0; i < count; i++)
            {
                BingoGoal goal = goals[index];
                if (goal != null && goal.Types != null)
                {
                    for (int t = 0; t < goal.Types.Length; t++)
                    {
                        string type = goal.Types[t];
                        if (type == "borrowed_tier")
                        {
                            continue;
                        }

                        if (seen.ContainsKey(type))
                        {
                            return lineName + " repeats type \"" + type + "\" on \""
                                + seen[type]
                                + "\" and \""
                                + goal.Name
                                + "\"";
                        }

                        seen[type] = goal.Name;
                    }
                }

                index += step;
            }

            return null;
        }

        public static List<BingoGoal> ResolveTier(Dictionary<int, List<BingoGoal>> tiers, int tier)
        {
            for (int offset = 0; offset <= 2; offset++)
            {
                List<BingoGoal> list;
                if (tiers.TryGetValue(tier + offset, out list) && list.Count > 0)
                {
                    return TagBorrowed(list, tier);
                }

                if (offset > 0 && tiers.TryGetValue(tier - offset, out list) && list.Count > 0)
                {
                    return TagBorrowed(list, tier);
                }
            }

            return new List<BingoGoal>();
        }

        private static List<BingoGoal> TagBorrowed(List<BingoGoal> source, int homeTier)
        {
            List<BingoGoal> result = new List<BingoGoal>();
            for (int i = 0; i < source.Count; i++)
            {
                BingoGoal goal = source[i];
                if (goal.Difficulty == homeTier)
                {
                    result.Add(goal);
                    continue;
                }

                BingoGoal clone = goal.Clone();
                List<string> types = new List<string>();
                if (clone.Types != null)
                {
                    types.AddRange(clone.Types);
                }

                if (!types.Contains("borrowed_tier"))
                {
                    types.Add("borrowed_tier");
                }

                clone.Types = types.ToArray();
                result.Add(clone);
            }

            return result;
        }

        private static Dictionary<int, BingoGoal> PickGoalsForTiers(
            Dictionary<int, List<BingoGoal>> tiers,
            Random rng)
        {
            Dictionary<int, BingoGoal> picks = new Dictionary<int, BingoGoal>();
            HashSet<string> usedNames = new HashSet<string>();
            if (TryPickGoalsRecursive(tiers, 1, usedNames, picks, rng))
            {
                return picks;
            }

            return picks;
        }

        private static bool TryPickGoalsRecursive(
            Dictionary<int, List<BingoGoal>> tiers,
            int tier,
            HashSet<string> usedNames,
            Dictionary<int, BingoGoal> picks,
            Random rng)
        {
            if (tier > 25)
            {
                return picks.Count == 25;
            }

            List<BingoGoal> candidates = ResolveTier(tiers, tier);
            if (candidates.Count == 0)
            {
                return false;
            }

            List<BingoGoal> shuffled = ShuffleCandidates(candidates, rng);
            for (int i = 0; i < shuffled.Count; i++)
            {
                BingoGoal candidate = shuffled[i];
                if (usedNames.Contains(candidate.Name))
                {
                    continue;
                }

                picks[tier] = candidate;
                usedNames.Add(candidate.Name);
                if (TryPickGoalsRecursive(tiers, tier + 1, usedNames, picks, rng))
                {
                    return true;
                }

                picks.Remove(tier);
                usedNames.Remove(candidate.Name);
            }

            return false;
        }

        private static List<BingoGoal> ShuffleCandidates(List<BingoGoal> candidates, Random rng)
        {
            List<BingoGoal> shuffled = new List<BingoGoal>(candidates);
            for (int i = shuffled.Count - 1; i > 0; i--)
            {
                int swap = rng.Next(i + 1);
                BingoGoal temp = shuffled[i];
                shuffled[i] = shuffled[swap];
                shuffled[swap] = temp;
            }

            return shuffled;
        }

        private static bool HasRowTypeConflicts(BingoGoal[] goals)
        {
            for (int row = 0; row < 5; row++)
            {
                if (LineHasTypeConflict(goals, row * 5, 1, 5))
                {
                    return true;
                }
            }

            return false;
        }

        private static bool LineHasTypeConflict(BingoGoal[] goals, int start, int step, int count)
        {
            HashSet<string> seen = new HashSet<string>();
            int index = start;
            for (int i = 0; i < count; i++)
            {
                BingoGoal goal = goals[index];
                if (goal != null && goal.Types != null)
                {
                    for (int t = 0; t < goal.Types.Length; t++)
                    {
                        string type = goal.Types[t];
                        if (type == "borrowed_tier")
                        {
                            continue;
                        }

                        if (seen.Contains(type))
                        {
                            return true;
                        }

                        seen.Add(type);
                    }
                }

                index += step;
            }

            return false;
        }
    }
}
