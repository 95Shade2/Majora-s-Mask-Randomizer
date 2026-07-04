using System.Drawing;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class BingoGoalMark
    {
        public const int StateCount = 6;

        public static int CycleForward(int state)
        {
            return (state + 1) % StateCount;
        }

        public static int CycleBackward(int state)
        {
            return (state + StateCount - 1) % StateCount;
        }

        public static bool IsMarked(int state)
        {
            return state > 0;
        }

        public static Color GetBackColor(int state)
        {
            switch (state)
            {
                case 1:
                    return Color.FromArgb(200, 230, 201);
                case 2:
                    return Color.FromArgb(255, 205, 210);
                case 3:
                    return Color.FromArgb(255, 224, 178);
                case 4:
                    return Color.FromArgb(187, 222, 251);
                case 5:
                    return Color.FromArgb(225, 190, 231);
                default:
                    return SystemColors.Control;
            }
        }

        public static string GetHtmlClass(int state)
        {
            switch (state)
            {
                case 1:
                    return "mark-green";
                case 2:
                    return "mark-red";
                case 3:
                    return "mark-orange";
                case 4:
                    return "mark-blue";
                case 5:
                    return "mark-purple";
                default:
                    return "";
            }
        }

        public static string GetStateName(int state)
        {
            switch (state)
            {
                case 1:
                    return "Green";
                case 2:
                    return "Red";
                case 3:
                    return "Orange";
                case 4:
                    return "Blue";
                case 5:
                    return "Purple";
                default:
                    return "None";
            }
        }
    }
}
