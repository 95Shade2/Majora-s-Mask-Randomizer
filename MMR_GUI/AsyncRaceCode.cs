using System;
using System.Collections.Generic;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class AsyncRaceCodeData
    {
        public string Version;
        public string RomSeed;
        public string SettingsHash;
        public string SettingsPayloadBase64;
        public string PlacementsPayloadBase64;
        public string LogicName;
        public BingoWinMode WinMode;

        // Legacy MMBINGO1 preset token (import only).
        public string LegacyPresetName;
    }

    internal sealed class AsyncRaceApplyResult
    {
        public bool Success;
        public string Message;
        public PresetVerificationResult Verification;
        public AsyncRaceCodeData Code;
    }

    internal sealed class AsyncRaceBuildResult
    {
        public bool Success;
        public string Message;
        public string RaceCode;
        public AsyncRaceCodeData Code;
    }

    internal static class AsyncRaceCode
    {
        internal const string Version = "MMBINGO3";
        internal const string Version2 = "MMBINGO2";
        internal const string LegacyVersion = "MMBINGO1";

        public static AsyncRaceBuildResult Build(Main_Window window, BingoWinMode winMode)
        {
            AsyncRaceBuildResult result = new AsyncRaceBuildResult();
            string seed = window.GetRomSeedText();
            if (string.IsNullOrWhiteSpace(seed))
            {
                result.Message = "Enter a ROM seed before creating an async race code.";
                return result;
            }

            string settingsPayload = window.BuildSettingsHashPayload();
            string settingsHash = SettingsHash.Compute(settingsPayload);
            string payloadBase64 = SettingsPayload.EncodeBase64(settingsPayload);
            Dictionary<string, string> placements = BingoPlacementMap.BuildFromWindow(window);
            string placementsBase64 = BingoPlacementMap.EncodeBase64(placements);
            string logicName = window.GetSelectedLogicName();
            if (string.IsNullOrWhiteSpace(logicName))
            {
                logicName = "None";
            }

            AsyncRaceCodeData code = new AsyncRaceCodeData
            {
                Version = Version,
                RomSeed = seed.Trim(),
                SettingsHash = settingsHash,
                SettingsPayloadBase64 = payloadBase64,
                PlacementsPayloadBase64 = placementsBase64,
                LogicName = logicName,
                WinMode = winMode
            };

            result.Code = code;
            result.RaceCode = Format(code);
            result.Success = true;
            return result;
        }

        public static bool TryParse(string raw, out AsyncRaceCodeData code, out string error)
        {
            code = null;
            error = null;
            if (string.IsNullOrWhiteSpace(raw))
            {
                error = "Race code is empty.";
                return false;
            }

            string line = raw.Trim();
            int newline = line.IndexOf('\n');
            if (newline >= 0)
            {
                line = line.Substring(0, newline).Trim();
            }

            string[] parts = line.Split('|');
            if (parts.Length != 5 && parts.Length != 7)
            {
                error = "Race code must have five or seven pipe-separated fields.";
                return false;
            }

            if (string.Equals(parts[0], Version, StringComparison.Ordinal))
            {
                return TryParseV3(parts, out code, out error);
            }

            if (string.Equals(parts[0], Version2, StringComparison.Ordinal))
            {
                return TryParseV2(parts, out code, out error);
            }

            if (string.Equals(parts[0], LegacyVersion, StringComparison.Ordinal))
            {
                return TryParseV1(parts, out code, out error);
            }

            error = "Unsupported race code version: " + parts[0];
            return false;
        }

        public static AsyncRaceApplyResult Apply(Main_Window window, AsyncRaceCodeData code)
        {
            AsyncRaceApplyResult result = new AsyncRaceApplyResult { Code = code };

            if (code == null)
            {
                result.Message = "Invalid race code.";
                return result;
            }

            if (string.Equals(code.Version, Version, StringComparison.Ordinal))
            {
                return ApplyV3(window, code, result);
            }

            if (string.Equals(code.Version, Version2, StringComparison.Ordinal))
            {
                return ApplyV2(window, code, result);
            }

            if (string.Equals(code.Version, LegacyVersion, StringComparison.Ordinal))
            {
                return ApplyV1(window, code, result);
            }

            result.Message = "Unsupported race code version.";
            return result;
        }

        public static string Format(AsyncRaceCodeData code)
        {
            if (string.Equals(code.Version, LegacyVersion, StringComparison.Ordinal))
            {
                return string.Join("|", new string[]
                {
                    code.Version,
                    code.LegacyPresetName ?? "",
                    code.RomSeed,
                    code.SettingsHash,
                    WinModeToken(code.WinMode)
                });
            }

            if (string.Equals(code.Version, Version, StringComparison.Ordinal))
            {
                return string.Join("|", new string[]
                {
                    code.Version,
                    code.RomSeed,
                    code.SettingsHash,
                    WinModeToken(code.WinMode),
                    code.SettingsPayloadBase64 ?? "",
                    code.PlacementsPayloadBase64 ?? "",
                    code.LogicName ?? "None"
                });
            }

            if (string.Equals(code.Version, Version2, StringComparison.Ordinal))
            {
                return string.Join("|", new string[]
                {
                    code.Version,
                    code.RomSeed,
                    code.SettingsHash,
                    WinModeToken(code.WinMode),
                    code.SettingsPayloadBase64 ?? ""
                });
            }

            return string.Join("|", new string[]
            {
                code.Version,
                code.RomSeed,
                code.SettingsHash,
                WinModeToken(code.WinMode),
                code.SettingsPayloadBase64 ?? ""
            });
        }

        public static string WinModeToken(BingoWinMode mode)
        {
            return mode == BingoWinMode.Blackout ? "blackout" : "line";
        }

        private static AsyncRaceApplyResult ApplyV3(
            Main_Window window,
            AsyncRaceCodeData code,
            AsyncRaceApplyResult result)
        {
            AsyncRaceApplyResult applied = ApplyV2(window, code, result);
            if (!applied.Success)
            {
                return applied;
            }

            Dictionary<string, string> placements;
            string placementError;
            if (!BingoPlacementMap.TryDecodeBase64(code.PlacementsPayloadBase64, out placements, out placementError))
            {
                result.Success = false;
                result.Message = placementError ?? "Race code placements payload is invalid.";
                return result;
            }

            window.SetBingoRaceContext(code.LogicName, placements);
            return result;
        }

        private static AsyncRaceApplyResult ApplyV2(
            Main_Window window,
            AsyncRaceCodeData code,
            AsyncRaceApplyResult result)
        {
            if (string.IsNullOrWhiteSpace(code.SettingsPayloadBase64))
            {
                result.Message = "Race code is missing embedded settings.";
                return result;
            }

            string payload;
            try
            {
                payload = SettingsPayload.DecodeBase64(code.SettingsPayloadBase64);
            }
            catch (FormatException)
            {
                result.Message = "Race code settings payload is invalid.";
                return result;
            }

            if (!window.ApplyRaceSettingsFromPayload(payload))
            {
                result.Message = "Race code settings payload could not be applied.";
                return result;
            }

            window.SetRomSeedText(code.RomSeed);
            return FinishApply(window, code, result);
        }

        private static AsyncRaceApplyResult ApplyV1(
            Main_Window window,
            AsyncRaceCodeData code,
            AsyncRaceApplyResult result)
        {
            if (!string.IsNullOrWhiteSpace(code.LegacyPresetName)
                && !string.Equals(code.LegacyPresetName, "CUSTOM", StringComparison.Ordinal))
            {
                window.TryLoadStandardPreset(code.LegacyPresetName);
            }

            window.SetRomSeedText(code.RomSeed);
            return FinishApply(window, code, result);
        }

        private static AsyncRaceApplyResult FinishApply(
            Main_Window window,
            AsyncRaceCodeData code,
            AsyncRaceApplyResult result)
        {
            result.Verification = StandardPresets.ValidateCurrentSettings(window);

            bool hashMatches = string.Equals(
                window.ComputeSettingsHash(),
                code.SettingsHash,
                StringComparison.OrdinalIgnoreCase);

            if (!hashMatches)
            {
                result.Message = "Settings hash mismatch — deployed settings do not match the race code.";
                result.Success = false;
                return result;
            }

            result.Success = true;
            result.Message = "Race settings deployed and verified.";
            return result;
        }

        private static bool TryParseV3(string[] parts, out AsyncRaceCodeData code, out string error)
        {
            code = null;
            error = null;

            if (string.IsNullOrWhiteSpace(parts[1]))
            {
                error = "Race code seed cannot be empty.";
                return false;
            }

            BingoWinMode winMode;
            if (!TryParseWinMode(parts[3], out winMode))
            {
                error = "Win mode must be 'line' or 'blackout'.";
                return false;
            }

            code = new AsyncRaceCodeData
            {
                Version = Version,
                RomSeed = parts[1].Trim(),
                SettingsHash = parts[2].Trim().ToLowerInvariant(),
                WinMode = winMode,
                SettingsPayloadBase64 = parts[4].Trim(),
                PlacementsPayloadBase64 = parts[5].Trim(),
                LogicName = parts[6].Trim()
            };
            return true;
        }

        private static bool TryParseV2(string[] parts, out AsyncRaceCodeData code, out string error)
        {
            code = null;
            error = null;

            if (string.IsNullOrWhiteSpace(parts[1]))
            {
                error = "Race code seed cannot be empty.";
                return false;
            }

            BingoWinMode winMode;
            if (!TryParseWinMode(parts[3], out winMode))
            {
                error = "Win mode must be 'line' or 'blackout'.";
                return false;
            }

            code = new AsyncRaceCodeData
            {
                Version = Version2,
                RomSeed = parts[1].Trim(),
                SettingsHash = parts[2].Trim().ToLowerInvariant(),
                WinMode = winMode,
                SettingsPayloadBase64 = parts[4].Trim()
            };
            return true;
        }

        private static bool TryParseV1(string[] parts, out AsyncRaceCodeData code, out string error)
        {
            code = null;
            error = null;

            if (string.IsNullOrWhiteSpace(parts[2]))
            {
                error = "Race code seed cannot be empty.";
                return false;
            }

            BingoWinMode winMode;
            if (!TryParseWinMode(parts[4], out winMode))
            {
                error = "Win mode must be 'line' or 'blackout'.";
                return false;
            }

            code = new AsyncRaceCodeData
            {
                Version = LegacyVersion,
                LegacyPresetName = parts[1].Trim(),
                RomSeed = parts[2].Trim(),
                SettingsHash = parts[3].Trim().ToLowerInvariant(),
                WinMode = winMode
            };
            return true;
        }

        private static bool TryParseWinMode(string token, out BingoWinMode mode)
        {
            mode = BingoWinMode.Line;
            if (string.Equals(token, "blackout", StringComparison.OrdinalIgnoreCase))
            {
                mode = BingoWinMode.Blackout;
                return true;
            }

            if (string.Equals(token, "line", StringComparison.OrdinalIgnoreCase))
            {
                mode = BingoWinMode.Line;
                return true;
            }

            return false;
        }
    }
}
