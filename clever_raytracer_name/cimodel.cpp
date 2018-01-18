#include "cimodel.hpp"
#include <iostream>
#include <cmath>

Parameters CiModel::calculateCi(Constants cs, Parameters ip)
{
    double ppcsurface = ip.CA / 10;
    double ppNewCi = ppcsurface - ((cs.A * 1.6 * 100) / cs.GS);
    double CI = (ppNewCi * 10);
    Constants cs_out = cs;
    cs_out.CI = CI;
    
    return cs_out;
}

Parameters::CiModel::const_Ci(Constants cs, Parameters ip)
{
    double CI = ip.CI;
    Cosntants cs_out = cs;
    cs_out.CI = CI;
    
    return cs_out;
}

bool CiModel::check(Constants cs, Parameters ip)
{
    if(cs.A >= -10 && cs.GS > 0 && ip.CA > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
