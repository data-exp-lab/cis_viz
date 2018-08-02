#ifndef __OBJECT__
#define __OBJECT__

#include "ray.hpp"
#include "color.hpp"

class Object
{
public:
    Object()
    {
        
    }
    
    raytrace::Vect position;
    //std::vector<Light> lights;
    
    virtual Color getColor()
    {
        //ARBITRARY VALUE
        return Color(1.0, 1.0, 1.0, 0);
    }
    virtual raytrace::Vect getNormalAt(raytrace::Vect intersection_position)
    {
        return raytrace::Vect(0, 0, 0);
    }
    
    /*virtual double findIntersection(raytrace::Ray ray)
    {
        return 0;
    }*/

    
};


#endif
