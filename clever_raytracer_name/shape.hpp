#ifndef __SHAPE__
#define __SHAPE__

#include "math.h"
#include "object.hpp"
#include "point.hpp"


class Point;

class Shape
{
public:
    raytrace::Vect center;
    raytrace::Vect position;
    Color color;
    //std::vector<Light> lights;
    
    Shape(){}
    virtual ~Shape(){}
    
    virtual bool intersectShape(const raytrace::Ray &ray, float &t0, float &t1);
    
    virtual Color getColor()
    {
        return Color(1.0, 1.0, 1.0, 1.0);
    }
    
    virtual raytrace::Vect getNormalAt(raytrace::Vect intersection_position)
    {
        return raytrace::Vect(0, 0, 0);
    }
    
    raytrace::Vect getPosition() const
    {
        return position;
    }
};

class Sphere2 : public Shape
{
public:
    float radius;
    float radiusSquared;
    
    Sphere2() : radius(0.0), radiusSquared(0.0){}
    Sphere2(const raytrace::Vect &pos, const float &r, /*const std::vector<Light> &lights,*/ Color color) : radius(r), radiusSquared(r * r)
    {
        Shape::position = pos;
        Shape::color = color;
    }
    
    bool intersectShape(const raytrace::Ray &ray, float &t0, float &t1);
    
};

class Triangle3 : public Shape
{
    raytrace::Vect A;
    raytrace::Vect B;
    raytrace::Vect C;
    raytrace::Vect surfaceNormal;
    
    Triangle3(){}
    Triangle3(const raytrace::Vect &a, const raytrace::Vect &b, const raytrace::Vect &c, const raytrace::Vect &sn, const Color &col/*, const std::vector<Light> &lights*/) : A(a), B(b), C(c), surfaceNormal(sn)
    {
        Shape::color = col;
        //Shape::lights = lights;
    }
    
    bool intersectShape(const raytrace::Ray &ray, float &t0, float &t1);
    
    raytrace::Vect getSideA() const
    {
        return A;
    }
    
    raytrace::Vect getSideB() const
    {
        return B;
    }
    
    raytrace::Vect getSideC() const
    {
        return C;
    }
    
};



/////////////

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
    
    /*virtual bool intersect(const Ray &ray, float &t0, float &t1)
    {
        Vect L = ray.getOrigin();
        Vect D = ray.getDirection();
        Vect tv = getPos() - L;
        float v = tv.dotProduct(D);
        if(v < 0)
        {
            return false;
        }
        float dsq = tv.dotProduct(tv) - (v * v);
        if(dsq > radSqrd)
        {
            return false;
        }
        float b = sqrt(radSqrd - dsq);
        t0 = v - b;
        t1 = v + b;
        
        return true;
    }*/
    
   /* virtual bool intersect2(const raytrace::Ray &ray, float &t0, float &t1)
    {
        raytrace::Vect L = ray.getOriginVect();
        raytrace::Vect D = ray.getDirection();
        
        raytrace::Vect tv = getPosition() - L;
        
        float v = tv.dotProduct(D);
        if(v < 0)
        {
            return false;
        }
        float dsq = tv.dotProduct(tv) - (v * v);
        if(dsq > radSqrd)
        {
            return false;
        }
        float b = sqrt(radSqrd - dsq);
        t0 = v - b;
        t1 = v + b;
        
        return true;
    }*/
    
    virtual double findIntersection(raytrace::Ray ray)
    {
        //Point ray_origin = ray.getOrigin();
        raytrace::Vect ray_origin = ray.getOriginVect();
        double ray_origin_x = ray_origin.getX();
        double ray_origin_y = ray_origin.getY();
        double ray_origin_z = ray_origin.getZ();
        //double ray_origin_x, ray_origin_y, ray_origin_z;
    
        //FIX
        raytrace::Vect ray_direction = ray.getDirection();
        double ray_direction_x = ray_direction.getX();
        double ray_direction_y = ray_direction.getY();
        double ray_direction_z = ray_direction.getZ();
        //double ray_direction_x, ray_direction_y, ray_direction_z;
        
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
            double b = 10;
            //double b = normal.dotProduct(ray.getOrigin().vectAdd(normal.vectMult(distance).negative()));
            return -1 * b / a;
        }
        
    }
};

class Triangle2 : public Object
{
private:
    raytrace::Vect A;
    raytrace::Vect B;
    raytrace::Vect C;
    raytrace::Vect surfaceNorm;
    Color color;
    
public:
    Triangle2(){}
    Triangle2(const raytrace::Vect &a, const raytrace::Vect &b, const raytrace::Vect &c, Color col/*, const raytrace::Vect &sn*/)
    {
        color = col;
    }
    
    raytrace::Vect getSideA() const
    {
        return A;
    }
    
    raytrace::Vect getSideB() const
    {
        return B;
    }
    
    raytrace::Vect getSideC() const
    {
        return C;
    }
    
    bool intersect(const raytrace::Ray &ray, float &t0, float &t1)
    {
        raytrace::Vect E1 = B - A;
        raytrace::Vect E2 = C - A;
        
        raytrace::Vect dir = ray.getDirection();
        raytrace::Vect ori = ray.getOriginVect();
        
        raytrace::Vect pVect = dir.crossProduct(E2);
        float det = E1.dotProduct(pVect);
        float invDet = 1 / det;
        
        if(det < 0.0000001f)
        {
            return false;
        }
        
        raytrace::Vect tVect = ori - A;
        float u = tVect.dotProduct(pVect)*invDet;
        if(u < 0.0 || u > 1)
        {
            return false;
        }
        
        raytrace::Vect qVect = tVect.crossProduct(E1);
        float v = dir.dotProduct(qVect) * invDet;
        
        if(v < 0 || u + v > 1)
        {
            return false;
        }
        t0 = E2.dotProduct(qVect) * invDet;
        
        return true;
    }

    
    
};





#endif

