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

void Climate::directlyInput(Parameters ip, Constants cs)
{
    ppfd_direct_list.push_back(cs.DIR_LIGHT_INTENSITY);
    ppfd_diffuse_list.push_back(cs.DIFF_LIGHT_INTENSITY);
    
    double elevationAngle = cs.SOLAR_ELE_ANGLE;
    double azimuthAngle = cs.SOLAR_DIR_ANGLE;
    if(elevationAngle >= 0)
    {
        raytrace::Vect v;
        v.x = cos(elevationAngle) * cos(azimuthAngle);
        v.y = cos(elevationAngle) * sin(azimuthAngle);
        v.z = -sin(elevationAngle);
        direct_light_d_list.push_back(v);
    }
    else
    {
        raytrace::Vect v;
        v.x = 0;
        v.y = 0;
        v.z = 0;
        direct_light_d_list.push_back(v);
    }
}

void Climate::climate_calculation_PPFD(double latitude, double solarTimeNoon, double atmosphericTransmittance, vector<float> day, double startHour, double endHour, double hourInterval, Parameters ip, Constants cs)
{
    int num = (int)((endHour - startHour) / hourInterval);
    for(int i = 0; i <= num; i++)
    {
        double hour = startHour + i * hourInterval;
        
        double DERTA = -23.45 * cos(2 * cs.PI * (day[i] + 10) / 365) / 180 * cs.PI;
        //LATITUDE
        double PHI = latitude / 180 * cs.PI;
        double TSN = solarTimeNoon;
        //TIME ANGLE
        double h = 15 * (hour - TSN) / 180 * cs.PI;
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
                PHYS = cs.PI - acos(temp);
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
            raytrace::Vect v;
            v.x = cos(elevationAngle) * cos(azimuthAngle);
            v.y = cos(elevationAngle) * sin(azimuthAngle);
            v.z = -sin(elevationAngle);
            direct_light_d_list.push_back(v);
            
            double PPFD1 = cs.kSOLAR_constant * pow(atmosphericTransmittance, 1 / sin(elevationAngle)) * sin(elevationAngle);
            double PPFD2 = 0.5 * cs.kSOLAR_constant * (1 - pow(atmosphericTransmittance, 1 / sin(elevationAngle))) * sin(elevationAngle);
            
            ppfd_direct_list.push_back(PPFD1);
            ppfd_diffuse_list.push_back(PPFD2);
        }
        else
        {
            raytrace::Vect v;
            v.x = 0;
            v.y = 0;
            v.z = 0;
            direct_light_d_list.push_back(v);
            
            double PPFD1 = 0;
            double PPFD2 = 0;
            
            ppfd_direct_list.push_back(PPFD1);
            ppfd_diffuse_list.push_back(PPFD2);
        }
    }
}

void Climate::climate_calculation_Tair(double latitude, double solarTimeNoon, double atmosphericTransmittance, vector<float> day, double startHour, double endHour, double hourInterval, Parameters ip, Constants cs)
{
    int num = (int)((endHour - startHour) / hourInterval);
    for(int i = 0; i <= num; i++)
    {
        double hour = startHour + i * hourInterval;
        //DEFAULT RELATIVE HUMIDITY
        RH.push_back(0.7);
        
        double YearAvgTemp = cs.T1;
        double YearAvgTempRange = cs.T2;
        double DailyAvgRange = cs.T3;
        double MaxDailyRange = cs.T4;
        double SineWaveStart = day[i];
        
        //DEFAULT AND CONSTANT
        double TemperaturePeakHour = 14;
        
        //CALCULATE AIR TEMPERATURE ABOVE CANOPY TEMPERATURES
        double currentDailyMean = YearAvgTemp + YearAvgTempRange * sin(2 * cs.PI * ((day[i] - SineWaveStart) / 365));
        double dailyRange = DailyAvgRange + (MaxDailyRange - DailyAvgRange) * sin(2 * cs.PI * ((day[i] - SineWaveStart) / 365));
        double dailyExcursion = sin(2 * cs.PI * ((hour - (TemperaturePeakHour - 6)) / 24));
        double InstantTemp = currentDailyMean + dailyRange * dailyExcursion;
        
        //CALCULATE HIGHEST AND LOWEST DAILY TEMPERATURES
        double currentHighest = currentDailyMean + dailyRange;
        double currentLowest = currentDailyMean - dailyRange;
        
        TAIR.push_back(InstantTemp);
    }
}
