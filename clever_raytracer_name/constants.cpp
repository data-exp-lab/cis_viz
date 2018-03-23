#include "constants.hpp"

Constants::Constants()
{
    
}

Constants::~Constants()
{
    
}

int convertDateToDOY(int month, int day)
{
    int DOY = 0;
    int DOY1 = 0;
    int dayInEachMonthList[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for(int i = 0; i < month; i++)
    {
        DOY1 = DOY1 + dayInEachMonthList[i];
    }
    DOY1 = DOY1 + day;
    DOY = DOY1;
    return DOY;
}

void calDirDiffPPFD(int TOT_LIGHT_INTENSITY, int DIR_TO_DIFF_RATIO)
{
    int DIR_LIGHT_INTENSITY;
    int DIFF_LIGHT_INTENSITY;
    DIR_LIGHT_INTENSITY = TOT_LIGHT_INTENSITY * DIR_TO_DIFF_RATIO / (DIR_TO_DIFF_RATIO + 1);
    DIFF_LIGHT_INTENSITY = TOT_LIGHT_INTENSITY - DIR_LIGHT_INTENSITY;
}

void calTotalPPFDandRatio(int DIR_LIGHT_INTENSITY, int DIFF_LIGHT_INTENSITY, int DIR_TO_DIFF_RATIO)
{
    int TOT_LIGHT_INTENSITY;
    TOT_LIGHT_INTENSITY = DIR_LIGHT_INTENSITY + DIFF_LIGHT_INTENSITY;
    if(DIFF_LIGHT_INTENSITY != 0)
    {
        DIR_TO_DIFF_RATIO = DIR_LIGHT_INTENSITY / DIFF_LIGHT_INTENSITY;
    }
    else
    {
        DIR_TO_DIFF_RATIO = 10000;
    }
}

void convertSouthLat(int NORTH_OR_SOUTH, int LATITUDE)
{
    if(NORTH_OR_SOUTH == 1)
    {
        LATITUDE = -LATITUDE;
    }
}

void calSolarTimeNoon(int SOLAR_TIME_AT_NOON_HOUR, int SOLAR_TIME_AT_NOON_MINUTE)
{
    int SOLAR_TIME_NOON;
    SOLAR_TIME_NOON = SOLAR_TIME_AT_NOON_HOUR + SOLAR_TIME_AT_NOON_MINUTE / 60;
}
