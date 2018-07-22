#include "shape.hpp"

using namespace raytrace;

bool Sphere2::intersectShape(const raytrace::Ray &ray, float &t0, float &t1)
{
    raytrace::Vect L = ray.getOriginVect();
    raytrace::Vect D = ray.getDirection();
    
    raytrace::Vect tv = getPosition() - L;
    
    float v = tv.dotProduct(D);
    
    if(v < 0)
    {
        return false;
    }
    float d_squared = tv.dotProduct(tv) - (v * v);
    
    if(d_squared > radiusSquared)
    {
        return false;
    }
    
    float b= sqrt(radiusSquared - d_squared);
    
    t0 = v - b;
    t1 = v + b;
    
    return true;
}

bool Triangle3::intersectShape(const raytrace::Ray &ray, float &t0, float &t1)
{
    raytrace::Vect E1 = B - A;
    raytrace::Vect E2 = C - A;
    
    raytrace::Vect dir = ray.getDirection();
    raytrace::Vect ori = ray.getOriginVect();
    
    raytrace::Vect pVect = dir.crossProduct(E2);
    
    float det = E1.dotProduct(pVect);
    float invDet = 1 / det;
    
    if(det < 0.00000000001f)
    {
        return false;
    }
    
    raytrace::Vect tVect = ori - A;
    
    float u = tVect.dotProduct(pVect) * invDet;
    
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









