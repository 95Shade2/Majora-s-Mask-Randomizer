using System;
using System.Collections.Generic;
using System.Text;

namespace Majora_s_Mask_Randomizer_GUI
{
    /// <summary>
    /// Debug helper: generate unrestricted boards and compare against known seeds.
    /// </summary>
    internal static class BingoParityHarness
    {
        public static string RunSeedCheck(int seed)
        {
            IList<BingoGoal> allGoals = BingoGoalList.LoadStandard();
            HashSet<string> obtainable = BuildUnrestrictedObtainable(allGoals);
            Dictionary<int, List<BingoGoal>> tiers =
                BingoGoalValidator.FilterGoalsByTier(allGoals, obtainable);

            Dictionary<int, List<BingoGoal>> safeTiers = BingoSrlGenerator.DeepClone(tiers);
            BingoCard card = BingoSrlGenerator.GenerateSrlBoard(safeTiers, seed, SrlProfile.Normal);

            if (card == null)
            {
                return "Generation failed for seed " + seed;
            }

            StringBuilder report = new StringBuilder();
            report.AppendLine("Seed: " + seed);
            for (int row = 0; row < 5; row++)
            {
                for (int col = 0; col < 5; col++)
                {
                    int index = row * 5 + col;
                    report.AppendLine("[" + row + "," + col + "] " + card.Goals[index]);
                }
            }

            return report.ToString();
        }

        private static HashSet<string> BuildUnrestrictedObtainable(IList<BingoGoal> allGoals)
        {
            HashSet<string> obtainable = new HashSet<string>();
            for (int i = 0; i < allGoals.Count; i++)
            {
                BingoGoal goal = allGoals[i];
                if (goal.RequiredItems != null)
                {
                    for (int j = 0; j < goal.RequiredItems.Length; j++)
                    {
                        obtainable.Add(goal.RequiredItems[j]);
                    }
                }

                if (goal.RequiredAnyOf != null)
                {
                    for (int j = 0; j < goal.RequiredAnyOf.Length; j++)
                    {
                        string[] group = goal.RequiredAnyOf[j];
                        if (group == null)
                        {
                            continue;
                        }

                        for (int k = 0; k < group.Length; k++)
                        {
                            obtainable.Add(group[k]);
                        }
                    }
                }
            }

            obtainable.Add("Deku Mask");
            obtainable.Add("Goron Mask");
            obtainable.Add("Zora Mask");
            obtainable.Add("Hookshot");
            obtainable.Add("Epona's Song");
            obtainable.Add("Powder Keg");
            obtainable.Add("Bomber's Notebook");
            obtainable.Add("Postman's Hat");
            obtainable.Add("Romani's Mask");
            obtainable.Add("Kafei's Mask");
            obtainable.Add("Gibdo Mask");
            obtainable.Add("Garo Mask");
            obtainable.Add("Captain's Hat");
            obtainable.Add("Pictograph Box");
            obtainable.Add("Mask of Truth");
            obtainable.Add("Fierce Deity Mask");
            obtainable.Add("Oath to Order");
            obtainable.Add("Land Title Deed");
            obtainable.Add("Mountain Title Deed");
            obtainable.Add("Swamp Title Deed");
            obtainable.Add("Ocean Title Deed");
            obtainable.Add("Woodfall Map");
            obtainable.Add("Magic Beans");
            obtainable.Add("Odolwa's Remains");
            obtainable.Add("Goht's Remains");
            obtainable.Add("Gyorg's Remains");
            obtainable.Add("Twinmold's Remains");
            return obtainable;
        }
    }
}
