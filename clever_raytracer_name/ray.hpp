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
    
    //USE FOR BOUNDING BOX
    //BASED ON SCRATCHAPIXEL
    inverse_direction = Vect(1/dir.x, 1/dir.y, 1/dir.z);
    sign[0] = (inverse_direction.x < 0);
    sign[1] = (inverse_direction.y < 0);
    sign[2] = (inverse_direction.z < 0);
}

#endif
