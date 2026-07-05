using System.Text;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class BingoRerollLog
    {
        public const string HeuristicFail = "HEURISTIC_FAIL";
        public const string DryRunFail = "DRY_RUN_FAIL";
        public const string ItemInfeasible = "ITEM_INFEASIBLE";
        public const string PoolExhausted = "POOL_EXHAUSTED";
        public const string SrlFail = "SRL_FAIL";
        public const string Success = "SUCCESS";
        public const string Exhausted = "EXHAUSTED";
        public const string Substitution = "SUBSTITUTION";
        public const string TypeSubstitutionRetry = "TYPE_SUBSTITUTION_RETRY";
        public const string TypeSubstitutionSuccess = "TYPE_SUBSTITUTION_SUCCESS";

        public static string Format(BingoCard card)
        {
            StringBuilder text = new StringBuilder();
            text.AppendLine("Bingo Generation Reroll Log");
            text.AppendLine("===========================");
            text.AppendLine("ROM seed: " + (card.RomSeed ?? ""));
            text.AppendLine("Generation seed: " + card.EffectiveSeed);
            text.AppendLine("Pool hash (" + BingoGoalValidator.PoolHashVersion + "): " + (card.PoolHash ?? ""));
            if (!string.IsNullOrEmpty(card.SettingsHash))
            {
                text.AppendLine("Settings hash (" + SettingsHash.Version + "): " + card.SettingsHash);
            }

            if (!string.IsNullOrEmpty(card.AsyncRaceCode))
            {
                text.AppendLine("Async race code: " + card.AsyncRaceCode);
            }

            text.AppendLine("Win mode: " + card.WinMode);
            if (!string.IsNullOrEmpty(card.LogicName))
            {
                text.AppendLine("Logic: " + card.LogicName);
            }

            if (!string.IsNullOrEmpty(card.PlacementsHash))
            {
                text.AppendLine("Placements hash (" + BingoPlacementMap.Version + "): " + card.PlacementsHash);
            }

            text.AppendLine("Rerolls: " + card.RerollCount + " | Goals substituted: " + card.GoalsSubstituted);
            text.AppendLine();

            if (card.RerollTrace == null || card.RerollTrace.Count == 0)
            {
                text.AppendLine("No reroll attempts were recorded.");
                return text.ToString();
            }

            text.AppendLine("Attempt  Seed         Result");
            text.AppendLine("-------  -----------  ------");
            for (int i = 0; i < card.RerollTrace.Count; i++)
            {
                RerollTraceEntry entry = card.RerollTrace[i];
                text.AppendLine(
                    PadRight(entry.Attempt.ToString(), 7)
                    + "  "
                    + PadRight(entry.Seed.ToString(), 11)
                    + "  "
                    + DescribeReason(entry.Reason));
                if (!string.IsNullOrEmpty(entry.Detail))
                {
                    text.AppendLine("                         -> " + entry.Detail);
                }
            }

            text.AppendLine();
            text.AppendLine("Reason key");
            text.AppendLine("----------");
            text.AppendLine(DescribeReason(HeuristicFail));
            text.AppendLine(DescribeReason(ItemInfeasible));
            text.AppendLine(DescribeReason(PoolExhausted));
            text.AppendLine(DescribeReason(DryRunFail));
            text.AppendLine(DescribeReason(SrlFail));
            text.AppendLine(DescribeReason(Success));
            text.AppendLine(DescribeReason(Exhausted));
            text.AppendLine(DescribeReason(Substitution));
            text.AppendLine(DescribeReason(TypeSubstitutionRetry));
            text.AppendLine(DescribeReason(TypeSubstitutionSuccess));

            return text.ToString();
        }

        public static string DescribeReason(string reason)
        {
            switch (reason)
            {
                case HeuristicFail:
                    return "Tier pool pre-check failed (duplicate goals or empty tier).";
                case ItemInfeasible:
                    return "Board contains goals not obtainable with current item placements and logic.";
                case PoolExhausted:
                    return "Could not assign 25 unique obtainable goals for this seed.";
                case DryRunFail:
                    return "SRL dry-run could not build a valid board with this seed. (legacy)";
                case SrlFail:
                    return "SRL board failed validation for this seed. (legacy)";
                case Success:
                    return "Valid card generated.";
                case Exhausted:
                    return "Maximum reroll attempts reached without a valid card.";
                case Substitution:
                    return "Invalid goals were replaced with tier-safe substitutes.";
                case TypeSubstitutionRetry:
                    return "Board still has row type conflicts after substitution (informational).";
                case TypeSubstitutionSuccess:
                    return "Board passed row type validation.";
                default:
                    return reason ?? "Unknown";
            }
        }

        private static string PadRight(string value, int width)
        {
            if (value.Length >= width)
            {
                return value;
            }

            return value.PadRight(width);
        }
    }
}
