#include "farquharModel.hpp"
#include "parameters.hpp"
#include "equations.hpp"
#include <iostream>

using namespace std;

void FarquharModel::calculation(Constants &cs, Parameters &ip)
{
    //SATURATION RATE
    double WC = cs.VCMAX * ip.CI / (ip.CI + ip.KMC * (1 + ip.OI / ip.KMO));
    double I2 = ip.PPFD * (1 - 0.15) / 2;
    double J = (I2 + cs.JMAX - sqrt(pow(I2 + cs.JMAX,2) - 4 * cs.THETA * I2 * cs.JMAX)) / (2 * cs.THETA);
    //NADPH LIMITED EQUATION
    double WJ = J * ip.CI / (4 * ip.CI + 8 * cs.GSTAR);
    
    double WMIN = WC;
    ip.isRubiscoLimit = 1;
    if(WJ < WMIN)
    {
        WMIN = WJ;
        ip.isRubiscoLimit = 0;
    }
    
    ip.A = (1 - cs.GSTAR / ip.CI) * WMIN - cs.RD;
    
    /*
     DETERMINATION OF LIGHT SATURATION OR LIGHT LIMITED ALGORITHM:
     CALCULATE A (W) WHEN PPFD = 2000;
     SET A_SAT = 90% OF A IN 2000
     REVERSE CALCULATION OF PPFDSAT FOR A_SAT
     IF HIGHER THAN PPFDSAT, LIGHT SATURATION; IF LOWER, LIGHT LIMITED
     */
    
    double PPFD2000 = 2000;
    I2 = PPFD2000 * (1 - 0.15) / 2;
    J = (I2 + cs.JMAX - sqrt(pow(I2 + cs.JMAX,2) - 4 * cs.THETA * I2 * cs.JMAX))
    / (2 * cs.THETA);
    double WJ_2000 = J * ip.CI / (4 * ip.CI + 8 * cs.GSTAR);
    double WJ_SAT;
    if(WJ_2000 < WC)
    {
        WJ_SAT = WJ_2000 * 0.9;
    }
    else
    {
        WJ_SAT = WC * 0.9;
    }
    
    double J_SAT = WJ_SAT * (4 * ip.CI + 8 * cs.GSTAR) / ip.CI;
    double I2_SAT = (cs.THETA * pow(J_SAT,2) - J_SAT * cs.JMAX) / (J_SAT - cs.JMAX);
    double PPFD_SAT = I2_SAT * 2 / (1 - 0.15);
    
    ip.PPFD = PPFD_SAT;
}

bool FarquharModel::check(Constants &cs, Parameters &ip)
{
    if(cs.VCMAX > 0 && cs.JMAX > 0 && ip.CI > 0 && ip.OI > 0 && ip.KMO > 0 && ip.KMC > 0 && cs.GSTAR > 0 && cs.THETA > 0 && ip.PPFD >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
