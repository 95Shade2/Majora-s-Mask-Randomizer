using System.Collections.Generic;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal enum BingoWinMode
    {
        Line,
        Blackout
    }

    internal enum SrlProfile
    {
        Normal,
        Relaxed,
        Simulation
    }

    internal sealed class BingoGoal
    {
        public string Name;
        public int Difficulty;
        public string[] Types;
        public string[] RequiredItems;
        public string[][] RequiredAnyOf;

        public BingoGoal Clone()
        {
            return new BingoGoal
            {
                Name = Name,
                Difficulty = Difficulty,
                Types = Types != null ? (string[])Types.Clone() : new string[0],
                RequiredItems = RequiredItems != null ? (string[])RequiredItems.Clone() : new string[0],
                RequiredAnyOf = RequiredAnyOf != null ? CloneAnyOf(RequiredAnyOf) : new string[0][]
            };
        }

        private static string[][] CloneAnyOf(string[][] source)
        {
            string[][] copy = new string[source.Length][];
            for (int i = 0; i < source.Length; i++)
            {
                copy[i] = (string[])source[i].Clone();
            }

            return copy;
        }
    }

    internal sealed class RerollTraceEntry
    {
        public int Attempt;
        public int Seed;
        public string Reason;
        public string Detail;
    }

    internal sealed class BingoCard
    {
        public string RomSeed;
        public int EffectiveSeed;
        public string PoolHash;
        public int RerollCount;
        public bool GoalsSubstituted;
        public List<RerollTraceEntry> RerollTrace;
        public BingoWinMode WinMode;
        public string[] Goals;
        public int[] GoalStates;
        public BingoGoal[] GoalObjects;

        public bool AllGoalsFeasible(HashSet<string> obtainable)
        {
            if (GoalObjects == null)
            {
                return false;
            }

            for (int i = 0; i < GoalObjects.Length; i++)
            {
                if (GoalObjects[i] != null && !BingoGoalValidator.IsGoalFeasible(GoalObjects[i], obtainable))
                {
                    return false;
                }
            }

            return true;
        }
    }
}
