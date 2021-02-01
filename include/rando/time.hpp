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

    void Default_Flags();

    void Set_Flag(const string &flag);

    void Flag_Setter(const string &flag);

    Time();

    Time(int day_number,
         bool is_night,
         int start_time,
         int day_number_end,
         int is_night_end,
         int end_time);

    Time(int day_number,
         bool is_night,
         int start_time,
         int day_number_end,
         int is_night_end,
         int end_time,
         string flag);

    Time(int day_number,
         bool is_night,
         int start_time,
         int day_number_end,
         int is_night_end,
         int end_time,
         string flag,
         string set_flag);

    Time(int day_number,
         bool is_night,
         int start_time,
         int day_number_end,
         int is_night_end,
         int end_time,
         string flag,
         string set_flag,
         vector<string> items);

    Time &operator=(const Time &other);
};

#endif
