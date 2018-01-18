#include "stomata.hpp"
#include "constants.hpp"
#include <cmath>

Parameters Stomata::emperical_model_1(Constants cs, Parameters ip)
{
    double CA = ip.CA / 10;
    double A = cs.A;
    if(A < 0)
    {
        A = 0;
    }
    
    double GB = BOUNDARY_LEVEL_CONDUCTANCE;
    double CS = CA - (1.4 * 100 / GB) * A;
    
    double ESAT_LEAF = 6.13755 * exp((cs.TLEAF * (18.564 - cs.TLEAF / 254.4)) / (cs.TLEAF + 255.57));
    double ESAT_AIR = 6.13755 * exp((ip.TAIR * (18.564 - ip.TAIR / 254.4)) / (ip.TAIR + 255.57));
    
    double RH = ip.RH / 100;
    if(RH < 0.2)
    {
        RH = 0.2;
    }
    double RHP = RH * ESAT_AIR / ESAT_LEAF;
    double A1 = ip.G1 * A * 100;
    double b = ip.G0;
    double c = RHP * GB;
    
    double d = CS * GB;
    
    double A2 = cs;
    double B2 = -(b * cs + A1 - d);
    double C2 = -(A1 * c + b * d);
    
    double GSWithSurfaceRH = (-B2 + sqrt(pow(B2,2) - 4 * A2 * C2)) / (2 * A2);
    Constants cs_out = cs;
    cs_out.GS = GSWithSurfaceRH;
    
    return cs_out;
}

Parameters Stomata::const_gs(Constants cs, Parameters ip)
{
    Constants cs_out = cs;
    cs_out.GS = ip.GS * 1000;
    
    return cs_out;
}
