using System;
using System.Collections.Generic;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class BingoObtainabilityContext
    {
        public string LogicName;
        public Dictionary<string, string> Placements;
        public HashSet<string> Obtainable;
        public LogicUsefulnessResult LogicData;

        public static BingoObtainabilityContext Build(Main_Window window)
        {
            BingoObtainabilityContext context = new BingoObtainabilityContext();
            if (window == null)
            {
                context.Obtainable = new HashSet<string>(StringComparer.Ordinal);
                return context;
            }

            context.LogicName = window.GetBingoLogicName();
            context.Placements = window.GetBingoPlacementMap();
            context.Obtainable = BingoGoalValidator.BuildObtainableSet(window, context);
            return context;
        }
    }
}
