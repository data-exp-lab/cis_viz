#ifndef __CLIMATE__
#define __CLIMATE__

#include "ray.hpp"
#include <vector>
#include "constants.hpp"
#include "parameters.hpp"

using namespace std;

class Climate
{
public:
    vector<raytrace::Vect> direct_light_d_list;
    vector<double> ppfd_direct_list;
    vector<double> ppfd_diffuse_list;
    
    vector<double> TAIR;
    vector<double> RH;
    
    Climate();
    virtual ~Climate();
    
    void climate_calculation_PPFD(double latitude, double solarTimeNoon, double atmosphericTransmittance, vector<float> day, double startHour, double endHour, double hourInterval, Parameters ip, Constants cs);
    void climate_calculation_Tair(double latitude, double solarTimeNoon, double atmosphericTransmittance, vector<float> day, double startHour, double endHour, double hourInterval, Parameters ip, Constants cs);
    void directlyInput(Parameters ip, Constants cs);
};

#endif
