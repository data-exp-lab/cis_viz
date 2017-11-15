using namespace std;
using namespace raytrace;

class Material
{
private:
    Vect KA;
    Vect KD;
    Vect KS;
    Vect KR;
    Vect KO;
    float level;
    float eta;
public:
    Material() : level(0.0) {}
    Material(const Vect &ka, const Vect &kd, const Vect &ks, const Vect &kr, const Vect &ko, const float &lv, const float &e) : KA(ka), KD(kd), KS(ks), KR(kr), KO(ko), level(lv), eta(e){}
    Vect getKA() const
    {
        return KA;
    }
    Vect getKD() const
    {
        return KD;
    }
    Vect getKS() const
    {
        return KS;
    }
    Vect getKR() const
    {
        return KR;
    }
    Vect getKO() const
    {
        return KO;
    }
    float getLevel() const
    {
        return level;
    }
    float getETA() const
    {
        return eta;
    }
};
