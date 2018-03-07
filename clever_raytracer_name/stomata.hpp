#ifndef __STOMATA__
#define __STOMATA__

#include "parameters.hpp"
#include "constants.hpp"
#include "climate.hpp"

class Stomata
{
public:
    Parameters emperical_model_1(Constants cs, Parameters ip);
    Parameters const_gs(Constants cs, Parameters ip);
    
};

#endif
