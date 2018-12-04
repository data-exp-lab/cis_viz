#ifndef __SHAPE__
#define __SHAPE__

#include "ray.hpp"
#include "vect.hpp"
#include "color.hpp"

#include <vector>

class Shape
{
    
public:
    double red;
    double green;
    double blue;
    double alpha;
    
    Shape();
    
    std::vector<Shape*> shapes;
    
    virtual Color getColor()
    {
        return Color(0.0, 0.0, 0.0, 0);
    }
    
    virtual Color setColor(float r, float g, float b, int a)
    {
        return Color(r, g, b, 0);
    }
    
    double getColorRed()
    {
        return red;
    }
    
    double getColorGreen()
    {
        return green;
    }
    
    double getColorBlue()
    {
        return blue;
    }
    
    double getColorAlpha()
    {
        return alpha;
    }
    
    virtual Color setColor(Color color)
    {
        return color;
    }
    
    virtual Vect getNormalAt(Vect intersection_position)
    {
        return Vect(0, 0, 0);
    }
    
    virtual double findIntersection(Ray ray)
    {
        return 0;
    }
    
    virtual std::vector<Shape*> get_shapes()
    {
        return shapes;
    }
    
};

Shape::Shape()
{
    
}

#endif
