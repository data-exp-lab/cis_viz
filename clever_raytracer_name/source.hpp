#ifndef __SOURCE__
#define __SOURCE__
#include "ray.hpp"
#include "color.hpp"

class Source
{
public:
    Source();
    
    virtual raytrace::Vect getLightPosition()
    {
        return raytrace::Vect(0, 0, 0);
    }
    virtual Color getLightColor()
    {
        return Color(1, 1, 1, 0);
    }

};




#endif
