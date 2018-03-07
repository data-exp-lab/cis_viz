#include "parameters.hpp"
#include "constants.hpp"

class FarquharModel
{
public:
    void calculation(Constants &cs, Parameters &ip);
    
private:
    bool check(Constants &cs, Parameters &ip);
    
};
