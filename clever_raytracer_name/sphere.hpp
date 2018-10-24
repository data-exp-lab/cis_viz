#ifndef __SPHERE__
#define __SPHERE__

#include "math.h"
#include "vect.hpp"
#include "color.hpp"
#include "shape.hpp"

class Sphere : public Shape
{
    Vect center;
    double radius;
    
    Color color;
    
public:
    Sphere();
    Sphere(Vect, double, Color);
    
    Vect getSphereCenter()
    {
        return center;
    }
    
    double getSphereRadius()
    {
        return radius;
    }
    
    virtual Color getColor()
    {
        return color;
    }
    
    virtual Vect getNormalAt(Vect point)
    {
        //NORMAL ALWAYS POINTS AWAY FROM CENTER OF SPHERE
        Vect normal_vect = point.vectAdd(center.negative()).normalize();
        
        return normal_vect;
    }
    
    virtual double findIntersection(Ray ray)
    {
        Vect ray_origin = ray.getRayOrigin();
        double ray_origin_x = ray_origin.getVectX();
        double ray_origin_y = ray_origin.getVectY();
        double ray_origin_z = ray_origin.getVectZ();
        
        Vect ray_direction = ray.getRayDirection();
        double ray_direction_x = ray_direction.getVectX();
        double ray_direction_y = ray_direction.getVectY();
        double ray_direction_z = ray_direction.getVectZ();
        
        Vect sphere_center = center;
        double sphere_center_x = sphere_center.getVectX();
        double sphere_center_y = sphere_center.getVectY();
        double sphere_center_z = sphere_center.getVectZ();
        
        double a = 1; // NORMALIZED
        double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x) + (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) + (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);
        double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius * radius);
        
        double discriminant = b * b - 4 * c;
        
        if(discriminant > 0)
        {
            //RAY INTERSECTS SPHERE
            
            //FIRST ROOT
            double root1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001;
            
            if(root1 > 0)
            {
                //FIRST ROOT IS SMALLEST POSITIVE ROOT
                return root1;
            }
            else
            {
                //SECOND ROOT IS SMALLEST POSITIVE ROOT
                double root2 = ((sqrt(discriminant) - b) / 2) - 0.000001;
                return root2;
            }
        }
        else
        {
            //RAY MISSED THE SPHERE
            return -1;
        }
    }

};

Sphere::Sphere()
{
    center = Vect(0, 0, 0);
    radius = 1.0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Sphere::Sphere(Vect cen, double rad, Color col)
{
    center = cen;
    radius = rad;
    color = col;
}



#endif
