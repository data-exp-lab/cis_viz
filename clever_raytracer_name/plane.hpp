#ifndef __PLANE__
#define __PLANE__

#include "math.h"
#include "shape.hpp"
#include "vect.hpp"
#include "color.hpp"

class Plane : public Shape
{
    Vect normal;
    double distance_from_origin;
    Color color;
    
public:
    
    Plane();
    Plane(Vect, double, Color);
    
    Vect getPlaneNormal()
    {
        return normal;
    }
    
    double getPlaneDistance()
    {
        return distance_from_origin;
    }
    
    virtual Color getColor()
    {
        return color;
    }
    
    virtual Vect getNormalAt(Vect point)
    {
        return normal;
    }
    
    //double is distance from location to ray origin
    virtual double findIntersection(Ray ray)
    {
        Vect ray_direction = ray.getRayDirection();
        
        double a = ray_direction.dotProduct(normal);
        
        if(a == 0)
        {
            //RAY IS PARALLEL TO PLANE
            //DOES NOT INTERSECT
            return -1;
        }
        else
        {
            double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance_from_origin).negative()));
            
            //DISTANCE FROM RAY ORIGIN TO POINT OF INTERSECTION
            return -1 * b / a;
        }
    }
    
};

Plane::Plane()
{
    normal = Vect(1, 0, 0);
    distance_from_origin = 0.0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Plane::Plane(Vect norm, double dist, Color col)
{
    normal = norm;
    distance_from_origin = dist;
    color = col;
}


#endif
