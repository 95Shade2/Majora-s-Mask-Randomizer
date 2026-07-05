using System;
using System.Collections.Generic;
using System.IO;
using System.Web.Script.Serialization;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal enum PresetVerificationStatus
    {
        Custom,
        Verified,
        ModifiedStandard,
        HashOnly
    }

    internal sealed class PresetVerificationResult
    {
        public PresetVerificationStatus Status;
        public string PresetName;
        public string CurrentHash;
        public string ExpectedHash;
        public string Message;
    }

    internal sealed class StandardPresetEntry
    {
        public string Name;
        public string SettingsHash;
    }

    internal sealed class StandardPresetManifest
    {
        public string Version;
        public List<StandardPresetEntry> Presets;
    }

    internal static class StandardPresets
    {
        private static StandardPresetManifest _cached;

        public static bool IsStandardPreset(string presetName)
        {
            return TryGetExpectedHash(presetName, out _);
        }

        public static bool TryGetExpectedHash(string presetName, out string hash)
        {
            hash = null;
            if (string.IsNullOrWhiteSpace(presetName))
            {
                return false;
            }

            StandardPresetManifest manifest = LoadManifest();
            if (manifest?.Presets == null)
            {
                return false;
            }

            for (int i = 0; i < manifest.Presets.Count; i++)
            {
                StandardPresetEntry entry = manifest.Presets[i];
                if (string.Equals(entry.Name, presetName.Trim(), StringComparison.Ordinal))
                {
                    hash = entry.SettingsHash;
                    return !string.IsNullOrEmpty(hash);
                }
            }

            return false;
        }

        public static PresetVerificationResult ValidateCurrentSettings(Main_Window window)
        {
            PresetVerificationResult result = new PresetVerificationResult
            {
                CurrentHash = window.ComputeSettingsHash()
            };

            string presetName = window.GetActivePresetName();
            result.PresetName = presetName;

            if (string.IsNullOrWhiteSpace(presetName))
            {
                result.Status = PresetVerificationStatus.Custom;
                result.Message = "Custom settings (no standard preset selected).";
                return result;
            }

            if (!TryGetExpectedHash(presetName, out string expectedHash))
            {
                result.Status = PresetVerificationStatus.HashOnly;
                result.Message = "Non-standard preset — verify using settings hash only.";
                return result;
            }

            result.ExpectedHash = expectedHash;
            if (string.Equals(result.CurrentHash, expectedHash, StringComparison.OrdinalIgnoreCase))
            {
                result.Status = PresetVerificationStatus.Verified;
                result.Message = presetName + " — verified.";
                return result;
            }

            result.Status = PresetVerificationStatus.ModifiedStandard;
            result.Message = presetName + " — settings hash mismatch (preset may have been modified locally).";
            return result;
        }

        private static StandardPresetManifest LoadManifest()
        {
            if (_cached != null)
            {
                return _cached;
            }

            string path = ResolveJsonPath();
            string json = File.ReadAllText(path);
            JavaScriptSerializer serializer = new JavaScriptSerializer();
            _cached = serializer.Deserialize<StandardPresetManifest>(json);
            return _cached;
        }

        private static string ResolveJsonPath()
        {
            string[] candidates = new string[]
            {
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "standard_presets.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Data", "standard_presets.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "MMR_GUI", "Data", "standard_presets.json")
            };

            for (int i = 0; i < candidates.Length; i++)
            {
                string full = Path.GetFullPath(candidates[i]);
                if (File.Exists(full))
                {
                    return full;
                }
            }

            throw new FileNotFoundException("standard_presets.json not found.");
        }
    }
}
