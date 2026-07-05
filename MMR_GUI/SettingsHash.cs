using System;
using System.Security.Cryptography;
using System.Text;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class SettingsHash
    {
        internal const string Version = "MMSETTINGS_V1";

        public static string Compute(string canonicalPayload)
        {
            string payload = canonicalPayload + Version;
            using (SHA256 sha = SHA256.Create())
            {
                byte[] hash = sha.ComputeHash(Encoding.UTF8.GetBytes(payload));
                StringBuilder hex = new StringBuilder(hash.Length * 2);
                for (int i = 0; i < hash.Length; i++)
                {
                    hex.Append(hash[i].ToString("x2"));
                }

                return hex.ToString().Substring(0, 16);
            }
        }
    }
}
