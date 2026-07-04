using System;
using System.Collections.Generic;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class BingoCardGenerator
    {
        private const int MaxRerolls = 10;
        private const int FallbackSeedAttempts = 128;

        public static BingoCard Generate(string userSeed, BingoWinMode mode, Main_Window state)
        {
            int baseSeed = ParseSeed(userSeed);
            HashSet<string> obtainable = BingoGoalValidator.GetObtainableItems(state);
            string poolHash = BingoGoalValidator.ComputePoolHashV1(obtainable);
            IList<BingoGoal> allGoals = BingoGoalList.LoadStandard();
            List<BingoGoal> validPool = BingoGoalValidator.GetValidGoals(allGoals, obtainable);
            List<RerollTraceEntry> trace = new List<RerollTraceEntry>();

            if (validPool.Count < 25)
            {
                throw new InvalidOperationException(
                    "Not enough feasible SRL goals for current item pool. Only "
                    + validPool.Count
                    + " of 25 tiers can be filled — enable more items or load FNB.");
            }

            Dictionary<int, List<BingoGoal>> filteredTiers =
                BingoGoalValidator.FilterGoalsByTier(allGoals, obtainable);

            List<string> heuristicFailures = BingoSrlGenerator.DescribeHeuristicFailures(filteredTiers);
            if (heuristicFailures.Count > 0)
            {
                throw new InvalidOperationException(
                    "Bingo tier pool failed pre-checks for the current item pool:\n- "
                    + string.Join("\n- ", heuristicFailures));
            }

            for (int attempt = 0; attempt <= MaxRerolls; attempt++)
            {
                int trySeed = ExpandSeed(baseSeed, attempt);

                if (!BingoSrlGenerator.SRLDryRunValid(filteredTiers, trySeed))
                {
                    trace.Add(new RerollTraceEntry
                    {
                        Attempt = attempt,
                        Seed = trySeed,
                        Reason = BingoRerollLog.DryRunFail,
                        Detail = BingoSrlGenerator.DescribeDryRunFailure(filteredTiers, trySeed)
                    });
                    continue;
                }

                Dictionary<int, List<BingoGoal>> safeTiers = BingoSrlGenerator.DeepClone(filteredTiers);
                BingoCard card = BingoSrlGenerator.GenerateSrlBoard(safeTiers, trySeed, SrlProfile.Normal);

                if (card != null && card.AllGoalsFeasible(obtainable))
                {
                    trace.Add(new RerollTraceEntry
                    {
                        Attempt = attempt,
                        Seed = trySeed,
                        Reason = BingoRerollLog.Success
                    });
                    FinalizeCard(card, userSeed, trySeed, poolHash, attempt, false, trace, mode);
                    return card;
                }

                string detail;
                if (card == null)
                {
                    detail = BingoSrlGenerator.DescribeDryRunFailure(filteredTiers, trySeed);
                }
                else
                {
                    detail = BingoGoalValidator.DescribeInfeasibleGoals(card, obtainable)
                        ?? "Board failed validation for an unknown reason.";
                }

                trace.Add(new RerollTraceEntry
                {
                    Attempt = attempt,
                    Seed = trySeed,
                    Reason = BingoRerollLog.SrlFail,
                    Detail = detail
                });
            }

            trace.Add(new RerollTraceEntry
            {
                Attempt = MaxRerolls,
                Seed = ExpandSeed(baseSeed, MaxRerolls),
                Reason = BingoRerollLog.Exhausted,
                Detail = "No seed in the reroll range produced a valid card."
            });

            BingoCard fallback = TryGenerateFallbackBoard(
                filteredTiers,
                obtainable,
                ExpandSeed(baseSeed, MaxRerolls),
                trace);

            if (fallback == null)
            {
                throw new InvalidOperationException("Unable to generate a bingo card after rerolls and substitution.");
            }

            List<string> substitutions = new List<string>();
            fallback = SubstituteInvalidGoalsTierSafe(
                fallback,
                validPool,
                allGoals,
                obtainable,
                ExpandSeed(baseSeed, MaxRerolls),
                substitutions);

            trace.Add(new RerollTraceEntry
            {
                Attempt = MaxRerolls,
                Seed = fallback.EffectiveSeed,
                Reason = BingoRerollLog.Substitution,
                Detail = substitutions.Count > 0
                    ? string.Join("; ", substitutions)
                    : "No goals required substitution."
            });

            if (!BingoSrlGenerator.ValidateTypeConstraints(fallback))
            {
                trace.Add(new RerollTraceEntry
                {
                    Attempt = MaxRerolls,
                    Seed = fallback.EffectiveSeed,
                    Reason = BingoRerollLog.TypeSubstitutionRetry,
                    Detail = BingoSrlGenerator.DescribeTypeConflicts(fallback.GoalObjects, false)
                });
            }

            FinalizeCard(
                fallback,
                userSeed,
                fallback.EffectiveSeed,
                poolHash,
                MaxRerolls,
                substitutions.Count > 0,
                trace,
                mode);
            return fallback;
        }

        private static BingoCard TryGenerateFallbackBoard(
            Dictionary<int, List<BingoGoal>> filteredTiers,
            HashSet<string> obtainable,
            int startSeed,
            List<RerollTraceEntry> trace)
        {
            for (int offset = 0; offset < FallbackSeedAttempts; offset++)
            {
                int seed = startSeed + offset;
                Dictionary<int, List<BingoGoal>> safeTiers = BingoSrlGenerator.DeepClone(filteredTiers);
                BingoCard card = BingoSrlGenerator.GenerateSrlBoard(safeTiers, seed, SrlProfile.Normal);
                if (card == null)
                {
                    continue;
                }

                if (!card.AllGoalsFeasible(obtainable))
                {
                    continue;
                }

                card.EffectiveSeed = seed;
                return card;
            }

            return null;
        }

        public static int ParseSeed(string userSeed)
        {
            if (string.IsNullOrWhiteSpace(userSeed))
            {
                return Environment.TickCount;
            }

            int parsed;
            if (int.TryParse(userSeed.Trim(), out parsed))
            {
                return parsed;
            }

            return userSeed.GetHashCode();
        }

        internal static int ExpandSeed(int baseSeed, int attempt)
        {
            unchecked
            {
                long h = baseSeed;
                h = h * 1103515245L + 12345L + (long)attempt * 2654435761L;
                return (int)h;
            }
        }

        private static void FinalizeCard(
            BingoCard card,
            string romSeed,
            int effectiveSeed,
            string poolHash,
            int rerollCount,
            bool substituted,
            List<RerollTraceEntry> trace,
            BingoWinMode mode)
        {
            card.RomSeed = romSeed;
            card.EffectiveSeed = effectiveSeed;
            card.PoolHash = poolHash;
            card.RerollCount = rerollCount;
            card.GoalsSubstituted = substituted;
            card.RerollTrace = trace;
            card.WinMode = mode;
            if (card.GoalStates == null || card.GoalStates.Length != 25)
            {
                card.GoalStates = new int[25];
            }
        }

        private static BingoCard SubstituteInvalidGoalsTierSafe(
            BingoCard card,
            List<BingoGoal> validPool,
            IList<BingoGoal> allGoals,
            HashSet<string> obtainable,
            int seed,
            List<string> substitutions)
        {
            Random rng = new Random(seed);
            HashSet<string> onCard = new HashSet<string>(card.Goals);
            Dictionary<int, List<BingoGoal>> byTier = new Dictionary<int, List<BingoGoal>>();

            for (int i = 0; i < validPool.Count; i++)
            {
                BingoGoal goal = validPool[i];
                if (!byTier.ContainsKey(goal.Difficulty))
                {
                    byTier[goal.Difficulty] = new List<BingoGoal>();
                }

                byTier[goal.Difficulty].Add(goal);
            }

            for (int i = 0; i < card.Goals.Length; i++)
            {
                BingoGoal goal = BingoGoalList.LookupByName(card.Goals[i], allGoals);
                if (goal != null && BingoGoalValidator.IsGoalFeasible(goal, obtainable))
                {
                    continue;
                }

                string oldName = card.Goals[i];
                string infeasibleDetail = goal != null
                    ? BingoGoalValidator.DescribeInfeasibility(goal, obtainable)
                    : "\"" + oldName + "\" is not in the goal list";

                int tier = goal != null ? goal.Difficulty : GuessTierFromIndex(i);
                List<BingoGoal> candidates;
                if (!byTier.TryGetValue(tier, out candidates) || candidates.Count == 0)
                {
                    throw new InvalidOperationException("No feasible substitute pool for tier " + tier + ".");
                }

                BingoGoal replacement = null;
                for (int tries = 0; tries < candidates.Count; tries++)
                {
                    int pick = rng.Next(candidates.Count);
                    BingoGoal candidate = candidates[pick];
                    if (onCard.Contains(candidate.Name))
                    {
                        continue;
                    }

                    replacement = candidate;
                    break;
                }

                if (replacement == null)
                {
                    throw new InvalidOperationException(
                        "No unique feasible substitute for tier " + tier + ".");
                }

                onCard.Remove(card.Goals[i]);
                card.Goals[i] = replacement.Name;
                card.GoalObjects[i] = replacement;
                onCard.Add(replacement.Name);

                if (substitutions != null)
                {
                    substitutions.Add(
                        "Replaced "
                        + (infeasibleDetail ?? ("\"" + oldName + "\""))
                        + " -> \""
                        + replacement.Name
                        + "\"");
                }
            }

            return card;
        }

        private static int GuessTierFromIndex(int index)
        {
            int row = index / 5;
            int col = index % 5;
            int[,] magic = new int[,]
            {
                { 15,  8,  1, 24, 17 },
                { 16, 14,  7,  5, 23 },
                { 22, 20, 13, 19, 11 },
                {  3,  4, 25, 12, 21 },
                { 10,  9, 18,  6,  2 }
            };

            return magic[row, col];
        }
    }
}
