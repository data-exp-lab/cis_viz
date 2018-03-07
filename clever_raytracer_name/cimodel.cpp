#include "cimodel.hpp"
#include <iostream>
#include <cmath>

Parameters CiModel::calculateCi(Constants cs, Parameters ip)
{
    double ppcsurface = cs.CA / 10;
    double ppNewCi = ppcsurface - ((ip.A * 1.6 * 100) / ip.GS);
    double CI = (ppNewCi * 10);
    Parameters ip_out = ip;
    ip_out.CI = CI;
    
    return ip_out;
}

Parameters CiModel::const_Ci(Constants cs, Parameters ip)
{
    double CI = ip.CI;
    Parameters ip_out = ip;
    ip_out.CI = CI;
    
    return ip_out;
}

bool CiModel::check(Constants cs, Parameters ip)
{
    if(ip.A >= -10 && ip.GS > 0 && cs.CA > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
