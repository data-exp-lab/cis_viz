#ifndef __RAY__
#define __RAY__

#include "vect.hpp"

class Ray
{
public:
    Vect origin;
    Vect direction;
    double photonFlux;
    
    //FOR BBOX-->BASED ON SCXRATCHAPIXEL
    Vect inverse_direction;
    int sign[3];
    
    Ray();
    
    Ray(Vect, Vect);
    
    Vect getRayOrigin()
    {
        return origin;
    }
    
    Vect getRayDirection()
    {
        return direction;
    }
};

Ray::Ray()
{
    origin = Vect(0, 0, 0);
    direction = Vect(1, 0, 0);
}

Ray::Ray(Vect ori, Vect dir)
{
    origin = ori;
    direction = dir;
}

#endif
