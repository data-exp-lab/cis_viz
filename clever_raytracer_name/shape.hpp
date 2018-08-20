#ifndef __SHAPE__
#define __SHAPE__

#include "ray.hpp"
#include "vect.hpp"
#include "color.hpp"

class Shape
{
    
public:
    Shape();
    
    virtual Color getColor()
    {
        return Color(0.0, 0.0, 0.0, 0);
    }
    
    virtual Vect getNormalAt(Vect intersection_position)
    {
        return Vect(0, 0, 0);
    }
    
    virtual double findIntersection(Ray ray)
    {
        return 0;
    }
    
};

Shape::Shape()
{
    
}



#endif
