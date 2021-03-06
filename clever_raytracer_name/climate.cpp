/*#include "climate.hpp"
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

void Climate::climate_calculation_PPFD(double latitude, double solarTimeNoon, double atmosphericTransmittance, vector<float> day, double startHour, double endHour, double hourInterval, Parameters ip, Constants cs)
{
    int num = (int)((endHour - startHour) / hourInterval);
    for(int i = 0; i <= num; i++)
    {
        double hour = startHour + (i * hourInterval);
        
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
        else if(temp <= -1)
        {
            PHYS = cs.PI;
        }
        else
        {
            PHYS = acos(temp);
        }
        
        if(h > 0)
        {
            PHYS = cs.PI - PHYS;
        }
        
        double elevationAngle = THETAS;
        double azimuthAngle = PHYS;
        if(elevationAngle >= 0)
        {
            Vect v;
            v.x = cos(elevationAngle) * cos(azimuthAngle);
            v.y = cos(elevationAngle) * sin(azimuthAngle);
            v.z = -sin(elevationAngle);
            
            direct_light_d_list.push_back(v);
            
            //DIRECT LIGHT
            double PPFD1 = cs.kSOLAR_constant * pow(atmosphericTransmittance, 1 / sin(elevationAngle)) * sin(elevationAngle);
            //DIFFUSE LIGHT
            double PPFD2 = 0.5 * cs.kSOLAR_constant * (1 - pow(atmosphericTransmittance, 1 / sin(elevationAngle))) * sin(elevationAngle);
            
            ppfd_direct_light.push_back(PPFD1);
            ppfd_diffuse_light.push_back(PPFD2);
        }
        else
        {
            Vect v;
            v.x = 0;
            v.y = 0;
            v.z = 0;
            direct_light_d_list.push_back(v);
            
            double PPFD1 = 0;
            double PPFD2 = 0;
            
            ppfd_direct_light.push_back(PPFD1);
            ppfd_diffuse_light.push_back(PPFD2);
        }
    
    }
    
}
*/

