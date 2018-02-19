#include "thermal.hpp"
#include <cmath>

using namespace std;

Parameters Thermal::temperatureAdj(Constants cs, Parameters ip)
{
    double KMC = exp(38.05 - (79.43 / (8.314 / 1000 * (ip.Tleaf + 273.15))));
    double KMO = exp(20.30 - (36.38 / (8.314 / 1000 * (ip.Tleaf + 273.15))));
    
    double VCMAX = ip.VCMAX0 * exp(26.35 - (65.33 / (8.314 / 1000 * (ip.Tleaf + 273.15))));
    double JMAX = ip.JMAX0 * exp(17.7 - (43.9 / (8.314 / 1000 * (ip.Tleaf + 273.14))));
    double RD = ip.RD0 * exp(18.72 - (46.39 / (8.314 / 1000 * (ip.Tleaf + 273.15))));
    double GSTAR = exp(19.02 - (37.83 / (8.314 / 1000 * (ip.Tleaf + 273.15))));
    
    Parameters ip_out = ip;
    ip_out.KC = KMC;
    ip_out.KO = KMO;
    ip_out.VCMAX = VCMAX;
    ip_out.JMAX = JMAX;
    ip_out.RD = RD;
    ip_out.GSTAR = GSTAR;
    
    return ip_out;
}
