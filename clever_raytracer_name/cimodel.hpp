#include "parameters.hpp"
#include "constants.hpp"

class CiModel
{
public:
    Parameters calculateCi(Constants cs, Parameters ip);
    Parameters const_Ci(Constants cs, Parameters ip);
private:
    bool check(Constants cs, Parameters ip);
};
