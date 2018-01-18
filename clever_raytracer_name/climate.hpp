#include "ray.hpp"
#include <vector>
#include "parameters.hpp"

using namespace std;

class Climate
{
public:
    vector<Vect> direct_light_d_list;
    vector<double> ppfd_direct_list;
    vector<double> ppfd_diffuse_list;
    
    vector<double> TAIR;
    vector<double> RH;
    
    Climate();
    virtual ~Climate();
    
    void climate_calculation_PPFD(double latitude, double solarTimeNoon, double atmosphericTransmittance, int day, double startHour, double endHour, double hourInterval, Parameters ip);
    void climate_calculation_Tair(double latitude, double solarTimeNoon, double atmosphericTransmittance, int day, double startHour, double endHour, double hourInterval, Parameters ip);
    void directlyInput(Parameters ip);
};
