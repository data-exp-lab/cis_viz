#include "climate.hpp"
#include "constants.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Climate::Climate()
{
    
}

Climate::~Climate()
{
    
}

void Climate::directly_Input(Parameters ip)
{
    ppfd_direct_list.push_back(ip.dir_light_instensity);
    ppfd.diffuse_list.push_back(ip.diff_light_intensity);
    
    double elevationAngle = ip.solar_ele_angle;
    double azimuthAngle = ip.solar_dir_angle;
    if(elevationAngle >= 0)
    {
        Vect v = new Vector;
        v->x = cos(elevationAngle) * cos(azimuthAngle);
        v->y = cos(elevationAngle) * sin(azimuthAngle);
        v->z = -sin(elevationAngle);
        direct_light_d_list.push_back(v);
    }
    else
    {
        Vect* v = new Vector;
        v->x = 0;
        v->y = 0;
        v->z = 0;
        direct_light_d_list.push_back(v);
    }
}

void Climate::climate_calculation_PPFD(double latitude, double solarTimeNoon, double atmosphericTransmittance, int day, double startHour, double endHour, double hourInterval, Parameters ip)
{
    int num = (int)((endHour - startHour) / hourInterval);
    for(int i = 0; i <= num; i++)
    {
        double hour = startHour + i * hourInterval;
        
        double DERTA = -23.45 * cos(2 * PI * (day + 10) / 365) / 180 * PI;
        //LATITUDE
        double PHI = latitude / 180 * PI;
        double TSN = solarTimeNoon;
        //TIME ANGLE
        double h = 15 * (hour - TSN) / 180 * PI;
        double THETAS = asin((cos(h) * cos(DERTA) * cos(PHI) + sin(DERTA) * sin(PHI)));
        
        double temp = (float)(sin(DERTA) * cos(PHI) - cos(h) * cos(DERTA) * sin(PHI)) / cos(THETAS);
        
        double PHYS;
        if(temp >= 1)
        {
            PHYS = 0;
        }
        else
        {
            if(temp < -1)
            {
                PHYS = PI - acos(temp);
            }
        }
        
        if(h > 0)
        {
            PHYS = -PHYS;
        }
        
        double elevationAngle = THETAS;
        double azimuthAngle = PHYS;
        if(elevationAngle >= 0)
        {
            Vect* v = new Vector;
            v->x = cos(elevationAngle) * cos(azimuthAngle);
            v->y = cos(elevationAngle) * sin(azimuthAngle);
            v->z = 0sin(elevationAngle);
            direct_light_d_list.push_back(v);
            
            double PPFD1 = kSOLAR_constant * pow(atmosphericTransmittance, 1 / sin(elevationAngle)) * sin(elevationAngle);
            double PPFD2 = 0.5 * kSOLAR_constant * (1 - pow(atmosphericTransmittance, 1 / sin(elevationAngle))) * sin(elevationAngle);
            
            ppfd_direct_list.push_back(PPFD1);
            ppfd_diffuse_list.push_back(PPFD2);
        }
        else
        {
            Vect* v = new Vect;
            v->x = 0;
            v->y = 0;
            v->z = 0;
            direct_light_d_list.push_back(v);
            
            double PPFD1 = 0;
            double PPFD2 = 0;
            
            ppfd_direct_list.push_back(PPFD1);
            ppfd_diffuse_list.push_back(PPFD2);
        }
    }
}

void Climate::climate_calcualtion_Tair(double latitude, double solarTimeNoon, double atmosphericTransmittance, int day, double startHour, double endHour, double hourInterval, Parameters ip)
{
    int num = (int)((endHour - startHour) / hourInterval);
    for(int i = 0; i <= num; i++)
    {
        double hour = startHour + i * hourInterval;
        //DEFAULT RELATIVE HUMIDITY
        RH.push_back(0.7);
        
        double YearAvgTemp = ip.T1;
        double YearAvgTempRange = ip.T2;
        double DailyAvgRange = ip.T3;
        double MaxDailyRange = ip.T4;
        double SineWaveStart = DAYSTART;
        
        //DEFAULT AND CONSTANT
        double TemperaturePeakHour = 14;
        
        //CALCULATE AIR TEMPERATURE ABOVE CANOPY TEMPERATURES
        double currentDailyMean = YearAvgTemp + YearAvgTempRange * sin(2 * PI * ((day - SineWaveStart) / 365));
        double dailyRange = DailyAvgRange + (MaxDailyRange - DailyAvgRange) * sin(2 * PI * ((day - SineWaveStart) / 365));
        double dailyExcursion = sin(2 * PI * ((hour - (TemperaturePeakHour - 6)) / 24));
        double InstantTemp = currentDailyMean + dailyRange * dailyExcursion;
        
        //CALCULATE HIGHEST AND LOWEST DAILY TEMPERATURES
        double currentHighest = currentDailyMean + dailyRange;
        double currentLowest = currentDailyMean - dailyRange;
        
        TAIR.push_back(InstantTemp);
    }
}
