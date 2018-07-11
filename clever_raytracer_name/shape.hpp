#ifndef __SHAPE__
#define __SHAPE__

#include "math.h"
#include "object.hpp"




class Sphere: public Object
{
    raytrace::Vect center;
    double radius;
    Color color;
    
public:
    Sphere()
    {
        center = raytrace::Vect(0, 0, 0);
        radius = 1.0;
        color = Color(0.5, 0.5, 0.5, 0);
    }
    
    Sphere(raytrace::Vect centerValue, double r, Color c)
    {
        center = centerValue;
        radius = r;
        color = c;
    }
    
    raytrace::Vect getSphereCenter()
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
    
    virtual raytrace::Vect getNormalAt(raytrace::Vect point)
    {
        raytrace::Vect normal_vect = point.vectAdd(center.negative()).normalize();
        return normal_vect;
    }
    
    virtual double findIntersection(raytrace::Ray ray)
    {
        //FIX
        /*raytrace::Vect ray_origin = ray.getOrigin();
        double ray_origin_x = ray_origin.getX();
        double ray_origin_y = ray_origin.getY();
        double ray_origin_z = ray_origin.getZ();*/
        double ray_origin_x, ray_origin_y, ray_origin_z;
    
        //FIX
        /*raytrace::Vect ray_direction = ray_direction.getDirection();
        double ray_direction_x = ray_direction.getX();
        double ray_direction_y = ray_direction.getY();
        double ray_direction_z = ray_direction.getZ();*/
        double ray_direction_x, ray_direction_y, ray_direction_z;
        
        raytrace::Vect sphere_center = center;
        double sphere_center_x = sphere_center.getX();
        double sphere_center_y = sphere_center.getY();
        double sphere_center_z = sphere_center.getZ();
        
        double a = 1; //NORMALIZED
        double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x) + (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) + (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);
        double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (radius * radius);
        
        double discriminant = b * b - 4 * a * c;
        
        if(discriminant > 0)
        {
            //RAY INTERSECTS SPHERE
            
            //FIRST ROOT (CLOSEST TO CAMERA)
            double root1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001;
            
            if(root1 > 0)
            {
                //FIRST ROOT IS HIT
                return root1;
            }
            else
            {
                //SECOND ROOT IS HIT
                double root2 = ((sqrt(discriminant) - b) / 1) - 0.000001;
                return root2;
            }
        }
        else
        {
            //RAY MISSED SPHERE
            return -1;
        }
        
    }
    
};

class Plane : public Object
{
public:
    
    Plane()
    {
        normal = raytrace::Vect(1, 0, 0);
        distance = 1.0;
        color = Color(0.5, 0.5, 0.5, 0);
    }
    
    Plane(raytrace::Vect normalValue, double distanceValue, Color colorValue)
    {
        normal = normalValue;
        distance = distanceValue;
        color = colorValue;
    }
    
    raytrace::Vect normal; //orientation
    double distance;
    Color color;
    
    raytrace::Vect getPlaneNormal()
    {
        return normal;
    }
    
    double getPlaneDistance()
    {
        return distance;
    }
    
    virtual Color getColor()
    {
        return color;
    }
    
    virtual raytrace::Vect getNormalAt(raytrace::Vect point)
    {
        return normal;
    }
    
    virtual double findIntersection(raytrace::Ray ray)
    {
        raytrace::Vect ray_direction = ray.getDirection();
        
        double a = ray_direction.dotProduct(normal);
        
        if(a == 0)
        {
            //RAY IS PARALLEL TO PLANE
            return -1;
        }
        else
        {
            //FIX
            double b = 1;
            //double b = normal.dotProduct(ray.getOrigin().vectAdd(normal.vectMult(distance).negative()));
            return -1 * b / a;
        }
        
    }
};

#endif

