#include "stomata.hpp"
//#include "constants.hpp"
#include <cmath>

Parameters Stomata::emperical_model_1(Constants cs, Parameters ip)
{
    double CA = cs.CA / 10;
    double A = ip.A;
    if(A < 0)
    {
        A = 0;
    }
    
    double GB = cs.BOUNDARY_LAYER_CONDUCTANCE;
    double CS = CA - (1.4 * 100 / GB) * A;
    
    double ESAT_LEAF = 6.13755 * exp((ip.Tleaf * (18.564 - ip.Tleaf / 254.4)) / (ip.Tleaf + 255.57));
    double ESAT_AIR = 6.13755 * exp((cs.TAIR * (18.564 - cs.TAIR / 254.4)) / (cs.TAIR + 255.57));
    
    double RH = cs.RH / 100;
    if(RH < 0.2)
    {
        RH = 0.2;
    }
    double RHP = RH * ESAT_AIR / ESAT_LEAF;
    double A1 = cs.G1 * A * 100;
    double b = cs.G0;
    double c = RHP * GB;
    
    double d = CS * GB;
    
    double A2 = CS;
    double B2 = -(b * CS + A1 - d);
    double C2 = -(A1 * c + b * d);
    
    double GSWithSurfaceRH = (-B2 + sqrt(pow(B2,2) - 4 * A2 * C2)) / (2 * A2);
    Parameters ip_out = ip;
    ip_out.GS = GSWithSurfaceRH;
    
    return ip_out;
}

Parameters Stomata::const_gs(Constants cs, Parameters ip)
{
    Parameters ip_out = ip;
    ip.GS = ip.GS * 1000;
    
    return ip_out;
}
