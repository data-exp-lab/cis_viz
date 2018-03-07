#ifndef __THERMAL__
#define __THERMAL__

#include "parameters.hpp"
#include "constants.hpp"

using namespace std;

class Thermal
{
public:
    Parameters temperatureAdj(Constants cs, Parameters ip);
};

#endif
