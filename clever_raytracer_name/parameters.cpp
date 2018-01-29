#include "parameters.hpp"

Parameters::Parameters()
{
    PPFD = -1;
}

Parameters::Parameters(int parameterFlag)
{
    PPFd = 1000;
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
}

void Parameters::prepare(/*INPUT PARAMETERS*/ int leafID, double CLAI, double PPFD1)
{
    Tleaf = TAIR;
    PPFD = PPFD1;
    THETA = THETA; //FROM INPUT PARAMETERS
    F = F; //FROM INPUT PARAMETERS
    JMAX = JMAX; //FROM INPUT PARAMETERS
    
    OI = OA;
    GSTAR = GSTAR; //FROM INPUT PARAMETERS
    RD0 = RD;
    KMC = KC;
    KMO = KO;
    VCMAX = VCMAX; //FROM INPUT PARAMETERS
    
    VCMAX0 = VCMAX_LIST[leafID - 1];
    JMAX0 = JMAX_LIST[leafID - 1];
    double KN = KN; //FROM INPUT PARAMETERS
    VCMAX = VCMAX * exp(-KN * CLAI);
    JMAX = JMAX * exp(-KN * CLAI);
    
    CI = CA * 0.7;
}

void Parameters::prepare_avgVcmaxJmax(/*INPUT PARAMETERS*/ double PPFD1, double leafT)
{
    double CLAI = 1;
    prepare(ip, 1, CLAI, PPFD1);
    double sumVcmax = 0;
    double sumJmax = 0;
    
    for(int i = 0; i < VCMAX_LIST.size(); i++)
    {
        sumVcmax += VCMAX_LIST[i];
        sumJmax += JMAX_LIST[i];
    }
    
    VCMAX = sumVcmax / VCMAX_LIST.size();
    JMAX = sumJmax / JMAX_LIST.size();
    VCMAX0 = VCMAX;
    JMAX0 = JMAX;
    
}





