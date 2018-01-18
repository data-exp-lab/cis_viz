#include "farquharModel.hpp"
#include "parameters.hpp"
#include "equations.hpp"
#include <iostream>

using namespace std;

void FarquharModel::calculation(constants &cs)
{
    //SATURATION RATE
    double WC = cs.VCMAX * cs.CI / (cs.CI + cs.KMC * (1 + cs.OI / cs.KMO));
    double I2 = cs.PPFD * (1 - 0.15) / 2;
    double J = (I2 + cs.JMAX - sqrt(pow(I2 + cs.JMAX,2) - r * cs.THETA * I2 * cs.JMAX)) / (2 * cs.THETA);
    //NADPH LIMITED EQUATION
    double WJ = J * cs.CI / (4 * cs.CI + 8 * cs.GSTAR);
    
    double WMIN = WC;
    cs.isRubiscoLimit = 1;
    if(WJ < WMIN)
    {
        WMIN = WJ;
        cs.isRubiscoLimit = 0;
    }
    
    cs.A = (1 - cs.GSTAR / cs.CI) * WMIN - cs.RD;
    
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
    double WJ_2000 = J * cs.CI / (4 * cs.CI + 8 * cs.GSTAR);
    double WJ_SAT;
    if(WJ_2000 < WC)
    {
        WJ_SAT = WJ_2000 * 0.9;
    }
    else
    {
        WJ_SAT = WC * 0.9;
    }
    
    double J_SAT = WJ_SAT * (4 * cs.CI + 8 * cs.GSTAR) / cs.CI;
    double I2_SAT = (cs.THETA * pow(J_SAT,2) - J_SAT * cs.JMAX) / (J_SAT - cs.JMAX);
    double PPFD_SAT = I2_SAT * 2 / (1 - 0.15);
    
    cs.PPFDSAT = PPFD_SAT;
}

bool FarquharModel::check(Constants &cs)
{
    if(cs.VCMAX > 0 && cs.JMAX > 0 && cs.CI > 0 && cs.OI > 0 && cs.KMO > 0 && cs.KMC > 0 && cs.GSTAR > 0 && cs.THETA > 0 && cs.PPFD >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
