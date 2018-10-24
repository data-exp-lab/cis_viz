#ifndef __PARAMETERS__
#define __PARAMETERS__

#include <cmath>
#include "constants.hpp"

class Parameters
{
public:
    double PPFD;
    double Tleaf;
    
    double F;
    double facetArea;

    //PHOTOSYNTHETIC PARAMETERS
    double VCMAX0;
    double JMAX0;
    double RD0;
    double VCMAX;
    double JMAX;
    double KMO;
    double KMC;
    double THETA;
    double GSTAR;
    
    double CI;
    double OI;
    double A;
    double GS;
    double RD;
    
    double KC;
    double KO;
    
    double PPFDsat;
    
    int isRubiscoLimit;
    
    Parameters();
    Parameters(int parameterFlag);
    
    void prepare(int leafID, double CLAI, double PPFD1);
    
    void prepare_avgVcmaxJmax(double PPFD1, double Tleaf);
};


#endif


