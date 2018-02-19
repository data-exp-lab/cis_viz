#include "parameters.hpp"
#include "constants.hpp"

Parameters::Parameters()
{
    PPFD = -1;
}

Parameters::Parameters(int parameterFlag)
{
    PPFD = 1000;
    Tleaf = 25;
    VCMAX0 = 78.8;
    JMAX0 = 155.7;
    VCMAX = VCMAX0;
    JMAX = JMAX0;
    KMC = 404;
    KMO = 278;
    GSTAR = 45;
    THETA = 0.7;
    RD = 1;
    CI = 270;
    OI = 210;
    KC = 404;
    KO = 278;
    GS = 0.7;
}

void Parameters::prepare(int leafID, double CLAI, double PPFD1)
{
    Constants cs;
    Parameters ip;
    
    Tleaf = cs.TAIR;
    PPFD = PPFD1;
    THETA = cs.THETA;
    F = cs.F;
    JMAX = cs.JMAX;
    
    OI = cs.OA;
    GSTAR = cs.GSTAR;
    RD0 = cs.RD;
    KMC = ip.KC;
    KMO = ip.KO;
    VCMAX = cs.VCMAX;
    
    VCMAX0 = cs.VCMAX_LIST[leafID - 1];
    JMAX0 = cs.JMAX_LIST[leafID - 1];
    double KN = cs.KN; 
    VCMAX = VCMAX * exp(-KN * CLAI);
    JMAX = JMAX * exp(-KN * CLAI);
    
    ip.CI = cs.CA * 0.7;
}

void Parameters::prepare_avgVcmaxJmax(double PPFD1, double leafT)
{
    Constants cs;
    
    double CLAI = 1;
    prepare(1, CLAI, PPFD1);
    double sumVcmax = 0;
    double sumJmax = 0;
    
    for(int i = 0; i < cs.VCMAX_LIST.size(); i++)
    {
        sumVcmax += cs.VCMAX_LIST[i];
        sumJmax += cs.JMAX_LIST[i];
    }
    
    VCMAX = sumVcmax / cs.VCMAX_LIST.size();
    JMAX = sumJmax / cs.JMAX_LIST.size();
    VCMAX0 = VCMAX;
    JMAX0 = JMAX;
    
}





