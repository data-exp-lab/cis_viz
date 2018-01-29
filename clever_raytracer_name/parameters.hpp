#ifndef __PARAMETERS__
#define __PARAMETERS__

#include <cmath>

class Parameters
{
public:
    double PPFD;
    double Tleaf;
    
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
    
    double F;
    
    double facetArea;
    
    double CI;
    double OI;
    double A;
    double GS;
    double RD;
    
    double PPFDsat;
    
    int isRubiscoLimit;
    
    Parameters();
    Parameters(int parameterFlag);
    
    void prepare(/*INPUT PARAMETERS*/ int leafID, double CLAI, double PPFD1);
    
    void prepare_avgVcmaxJmax(/*INPUT PARAMETERS*/ double PPFD1, double leafT);

};

#endif
