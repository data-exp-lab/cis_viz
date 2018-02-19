#include "parameters.hpp"
#include "constants.hpp"

class EnergyBalance
{
public:
    Parameters calculateLeafT(Constants cs, Parameters ip, double canopyHeight);
    Parameters calculateLeafT_withWindSpeed(Constants cs, Parameters ip, double canopyHeight);
    EnergyBalance();
private:
    double leafWidth;
    double windSpeedHeightCoeff;
    double windSpeedRoughnessLength;
};
