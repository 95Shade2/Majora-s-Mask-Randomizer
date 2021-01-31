#ifndef RANDO_TIME_HPP
#define RANDO_TIME_HPP

#include <map>
#include <string>
#include <vector>

// bad shade!
using namespace std;

class Time
{
  public:
    int Day;
    bool isNight;
    int timeStart;
    int DayEnd;
    int isNightEnd;
    int timeEnd;
    vector<string> Items_Needed;
    map<string, bool> Flags;
    vector<string> Flag_Setters;

    void Default_Flags()
    {
        Flags["KillSakon"] = false;
        Flags["NoKillSakon"] = false;
    }

    void Set_Flag(string flag)
    {
        if (flag != "")
        {
            Flags[flag] = true;
        }
    }

    void Flag_Setter(string flag)
    {
        if (flag != "")
        {
            Flag_Setters.push_back(flag);
        }
    }

    Time()
    {
        Day = -1;
        isNight = false;
        timeStart = -1;
        timeEnd = -1;
        DayEnd = -1;
        isNightEnd = false;

        Default_Flags();
    }

    Time(int day_number,
         bool is_night,
         int start_time,
         int day_number_end,
         int is_night_end,
         int end_time)
    {
        Day = day_number;
        isNight = is_night;
        timeStart = start_time;
        DayEnd = day_number_end;
        isNightEnd = is_night_end;
        timeEnd = end_time;

        Default_Flags();
    }

    Time(int day_number,
         bool is_night,
         int start_time,
         int day_number_end,
         int is_night_end,
         int end_time,
         string flag)
    {
        Day = day_number;
        isNight = is_night;
        timeStart = start_time;
        DayEnd = day_number_end;
        isNightEnd = is_night_end;
        timeEnd = end_time;

        Default_Flags();

        Set_Flag(flag);
    }

    Time(int day_number,
         bool is_night,
         int start_time,
         int day_number_end,
         int is_night_end,
         int end_time,
         string flag,
         string set_flag)
    {
        Day = day_number;
        isNight = is_night;
        timeStart = start_time;
        DayEnd = day_number_end;
        isNightEnd = is_night_end;
        timeEnd = end_time;

        Default_Flags();

        Flag_Setter(set_flag);

        Set_Flag(flag);
    }

    Time(int day_number,
         bool is_night,
         int start_time,
         int day_number_end,
         int is_night_end,
         int end_time,
         string flag,
         string set_flag,
         vector<string> items)
    {
        Day = day_number;
        isNight = is_night;
        timeStart = start_time;
        DayEnd = day_number_end;
        isNightEnd = is_night_end;
        timeEnd = end_time;

        Default_Flags();

        Set_Flag(flag);

        Items_Needed = items;
    }

    Time &operator=(const Time &other)
    {
        if (&other != this)
        {
            Day = other.Day;
            isNight = other.isNight;
            timeStart = other.timeStart;
            DayEnd = other.DayEnd;
            isNightEnd = other.isNightEnd;
            timeEnd = other.timeEnd;

            Flags = other.Flags;
        }

        return *this;
    }
};

#endif
