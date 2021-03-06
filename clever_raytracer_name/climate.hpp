#ifndef __CLIMATE__
#define __CLIMATE__

#include "ray.hpp"
#include "vect.hpp"
#include <vector>
#include "constants.hpp"
#include "parameters.hpp"

#include "math.h"
#include <valarray>

using namespace std;

class Climate
{
public:
    vector<Vect> direct_light_d_list;
    
    vector<double> ppfd_direct_light;
    vector<double> ppfd_diffuse_light;
    
    vector<double> TAIR;
    vector<double> RH;
    
    Climate()
    {}
    virtual ~Climate() {}
    
    void climate_calculation_PPFD(double latitude, double solarTimeNoon, double atmosphericTransmittance, vector<float> day, double startHour, double endHour, double hourInterval, Parameters ip, Constants cs);
    
    //TESTING VALIDITY OF PPFD CALCUATIONS --> SIMPLEST FORM
    void calculate_PPFD(double startHour, double endHour, double hourInterval, Constants cs);
    
    void calculate_PPFD_direct_light(int current_pixel, valarray<float>& PPFD_dir_light_per_triangle, Constants cs, double& individual_PPFD);
    
    //FIX: Need these later?
    //void climate_calculation_TAIR(double latitude, double solarTimeNoon, double atmosphericTransmittance, vector<float> day, double startHour, double endHour, double hourInterval, Parameters ip, Constants cs);
    //void directlyInput(Parameters ip, Constants cs);

};

void Climate::calculate_PPFD(double startHour, double endHour, double hourInterval, Constants cs)
{
    
}

void Climate::calculate_PPFD_direct_light(int current_pixel, valarray<float>& PPFD_dir_light_per_triangle, Constants cs, double& individual_PPFD)
{
    //ITERATION 1: set to 1.0
    //PPFD_dir_light_per_triangle[current_pixel] = 1.0;
    
    //FIX: Currently hard coded; need to make it variable
    int day = 180;
    
    //FIX: Hard code to noon
    double hour = 12;
    double solarTimeNoon = 12;
    double latitude = 40.6;
    //TIME ANGLE
    double h = 15 * (hour - solarTimeNoon) / 180 * cs.PI;
    
    double derta = -23.45 * cos(2 * cs.PI * (day + 10) / 365) / 180 * cs.PI;
    double phi = latitude / 180 * cs.PI;
    double thetas = asin((cos(h) * cos(derta) * cos(phi) + sin(derta) * sin(phi)));
    
    double temp = (float)(sin(derta) * cos(phi) - cos(h) * cos(derta) * sin(phi)) / cos(thetas);
    
    double phys;
    if(temp >= 1)
    {
        phys = 0;
    }
    else
    {
        if(temp < -1)
        {
            temp = -1;
        }
    }
    
    if(h > 0)
    {
        phys = -phys;
    }
    
    double elevationAngle = thetas;
    double azimuthAngle = phys;
    
    //FIX: double check there isn't a stack issue here like with the triangles
    Vect v;
    v.x = cos(elevationAngle) * cos(azimuthAngle);
    v.y = cos(elevationAngle) * sin(azimuthAngle);
    v.z = -sin(elevationAngle);
    
    double ppfd = cs.kSOLAR_constant * pow(1., 1 / sin(elevationAngle)) * sin(elevationAngle);
    
    PPFD_dir_light_per_triangle[current_pixel] = ppfd;
    individual_PPFD = ppfd;
    
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
            //double PPFD2 = 0.5 * cs.kSOLAR_constant * (1 - pow(atmosphericTransmittance, 1 / sin(elevationAngle))) * sin(elevationAngle);
            
            ppfd_direct_light.push_back(PPFD1);
            //ppfd_diffuse_light.push_back(PPFD2);
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
            //ppfd_diffuse_light.push_back(PPFD2);
        }
        
    }
    
}



#endif
