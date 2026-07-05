using System;

namespace Majora_s_Mask_Randomizer_GUI
{
    /// <summary>
    /// Matches C++ seed parsing in main.cpp (string_to_dec / to_ascii).
    /// </summary>
    internal static class RomSeedParser
    {
        public static int Parse(string userSeed)
        {
            if (string.IsNullOrWhiteSpace(userSeed))
            {
                return RandomBingoSeed();
            }

            string trimmed = userSeed.Trim();
            int parsed;
            if (int.TryParse(trimmed, out parsed))
            {
                return parsed;
            }

            return ToAscii(trimmed);
        }

        /// <summary>
        /// New seed on each call — used when bingo ROM seed is left blank.
        /// </summary>
        public static int RandomBingoSeed()
        {
            unchecked
            {
                long mix = DateTime.UtcNow.Ticks;
                mix ^= (long)Guid.NewGuid().GetHashCode() << 16;
                mix ^= Environment.TickCount;
                return (int)mix;
            }
        }

        internal static int ToAscii(string text)
        {
            double ascii = 0;
            for (int c = 0; c < text.Length; c++)
            {
                int chr = text[c];
                if (chr < 10)
                {
                    ascii *= 10;
                }
                else if (chr < 100)
                {
                    ascii *= 100;
                }
                else
                {
                    ascii *= 1000;
                }

                ascii += chr;
            }

            return unchecked((int)ascii);
        }
    }
}
