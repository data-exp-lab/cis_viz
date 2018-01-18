#include "thermal.hpp"
#include <cmath>

using namespace std;

Parameters Thermal::temperatureAdj(Constants cs)
{
    double KMC = exp(38.05 - (79.43 / (8.314 / 1000 * (cs.TLEAF + 273.15))));
    double KMO = exp(20.30 - (36.38 / (8.314 / 1000 * (cs.TLEAF + 273.15))));
    
    double VCMAX = cs.VCMAX0 * exp(26.35 - (65.33 / (8.314 / 1000 * (cs.TLEAF + 273.15))));
    double JMAX = cs.JMAX0 * exp(17.7 - (43.9 / (8.314 / 1000 * (cs.TLEAF + 273.14))));
    double RD = cs.RD0 * exp(18.72 - (46.39 / (8.314 / 1000 * (cs.TLEAF + 273.15))));
    double GSTAR = exp(19.02 - (37.83 / (8.314 / 1000 * (cs.TLEAF + 273.15))));
    
    Constants cs_out = cs;
    cs_out.KMC = KMC;
    cs_out.KMO = KMO;
    cs_out.VCMAX = VCMAX;
    cs_out.JMAX = JMAX;
    cs_out.RD = RD;
    cs_out.GSTAR = GSTAR;
    
    return cs_out;
}
