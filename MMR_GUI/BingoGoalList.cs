using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class BingoGoalList
    {
        private static IList<BingoGoal> _cached;

        public static IList<BingoGoal> LoadStandard()
        {
            if (_cached != null)
            {
                return _cached;
            }

            string jsonPath = ResolveJsonPath();
            string json = File.ReadAllText(jsonPath, Encoding.UTF8);
            JavaScriptSerializer serializer = new JavaScriptSerializer();
            serializer.MaxJsonLength = int.MaxValue;
            Dictionary<string, object> root = serializer.Deserialize<Dictionary<string, object>>(json);

            if (!root.ContainsKey("goals"))
            {
                throw new InvalidOperationException("mm_bingo_standard.json is missing a goals array.");
            }

            ArrayList rawGoals = root["goals"] as ArrayList;
            if (rawGoals == null)
            {
                throw new InvalidOperationException("mm_bingo_standard.json goals must be an array.");
            }

            List<BingoGoal> goals = new List<BingoGoal>();
            foreach (object entry in rawGoals)
            {
                Dictionary<string, object> row = entry as Dictionary<string, object>;
                if (row == null)
                {
                    continue;
                }

                goals.Add(ParseGoal(row));
            }

            _cached = goals;
            return _cached;
        }

        public static BingoGoal LookupByName(string name, IList<BingoGoal> allGoals)
        {
            for (int i = 0; i < allGoals.Count; i++)
            {
                if (allGoals[i].Name == name)
                {
                    return allGoals[i];
                }
            }

            return null;
        }

        private static BingoGoal ParseGoal(Dictionary<string, object> row)
        {
            BingoGoal goal = new BingoGoal
            {
                Name = GetString(row, "name"),
                Difficulty = GetInt(row, "difficulty"),
                Types = GetStringArray(row, "types"),
                RequiredItems = GetStringArray(row, "requiredItems"),
                RequiredAnyOf = GetStringArrayArray(row, "requiredAnyOf")
            };

            return goal;
        }

        private static string GetString(Dictionary<string, object> row, string key)
        {
            if (!row.ContainsKey(key) || row[key] == null)
            {
                return "";
            }

            return row[key].ToString();
        }

        private static int GetInt(Dictionary<string, object> row, string key)
        {
            if (!row.ContainsKey(key) || row[key] == null)
            {
                return 0;
            }

            return Convert.ToInt32(row[key]);
        }

        private static string[] GetStringArray(Dictionary<string, object> row, string key)
        {
            if (!row.ContainsKey(key) || row[key] == null)
            {
                return new string[0];
            }

            ArrayList list = row[key] as ArrayList;
            if (list == null)
            {
                return new string[0];
            }

            string[] values = new string[list.Count];
            for (int i = 0; i < list.Count; i++)
            {
                values[i] = list[i] != null ? list[i].ToString() : "";
            }

            return values;
        }

        private static string[][] GetStringArrayArray(Dictionary<string, object> row, string key)
        {
            if (!row.ContainsKey(key) || row[key] == null)
            {
                return new string[0][];
            }

            ArrayList outer = row[key] as ArrayList;
            if (outer == null)
            {
                return new string[0][];
            }

            string[][] groups = new string[outer.Count][];
            for (int i = 0; i < outer.Count; i++)
            {
                ArrayList inner = outer[i] as ArrayList;
                if (inner == null)
                {
                    groups[i] = new string[0];
                    continue;
                }

                groups[i] = new string[inner.Count];
                for (int j = 0; j < inner.Count; j++)
                {
                    groups[i][j] = inner[j] != null ? inner[j].ToString() : "";
                }
            }

            return groups;
        }

        private static string ResolveJsonPath()
        {
            string[] candidates = new string[]
            {
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "mm_bingo_standard.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Data", "mm_bingo_standard.json"),
                Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "MMR_GUI", "Data", "mm_bingo_standard.json")
            };

            for (int i = 0; i < candidates.Length; i++)
            {
                string full = Path.GetFullPath(candidates[i]);
                if (File.Exists(full))
                {
                    return full;
                }
            }

            throw new FileNotFoundException("mm_bingo_standard.json not found.");
        }
    }
}
