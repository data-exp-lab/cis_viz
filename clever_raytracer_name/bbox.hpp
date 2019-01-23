#ifndef __BBOX__
#define __BBOX__

#include "ray.hpp"
using namespace std;

class BBox
{
public:
    //MIN POINT
    double x0 = 0;
    double y0 = 0;
    double z0 = 0;
    //MAX POINT
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;
    
    //BASED ON SCRATCHAPIXEL
    Vect bounds[2];
    
    BBox();
    
    BBox(const double _x0, const double _x1, const double _y0, const double _y1, const double _z0, const double _z1) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), z0(_z0), z1(_z1) {}
    
    void setBBoxXMin(float _x0)
    {
        x0 = _x0;
    }
    
    void setBBoxXMax(float _x1)
    {
        x1 = _x1;
    }
    
    void setBBoxYMin(float _y0)
    {
        y0 = _y0;
    }
    
    void setBBoxYMax(float _y1)
    {
        y1 = _y1;
    }
    
    void setBBoxZMin(float _z0)
    {
        z0 = _z0;
    }
    
    void setBBoxZMax(float _z1)
    {
        z1 = _z1;
    }
    
    float getBBoxXMin()
    {
        return x0;
    }
    
    float getBBoxXMax()
    {
        return x1;
    }
    
    float getBBoxYMin()
    {
        return y0;
    }
    
    float getBBoxYMax()
    {
        return y1;
    }
    
    float getBBoxZMin()
    {
        return z0;
    }
    
    float getBBoxZMax()
    {
        return z1;
    }
    
    
    bool insideBBox(const Vect& point) const
    {
        return ((point.x > x0 && point.x < x1) && (point.y > y0 && point.y < y1) && (point.z > z0 && point.z < z1));
    }
    
    //IF RAY INTERSECTS WITH BBOX, RETURN TRUE; ELSE, RETURN FALSE
    bool intersectBBox(const Ray& ray, BBox bbox_for_triangle) const
    {
        //BBOX INITIALIZATION
        //MIN POINT
        double x0 = bbox_for_triangle.x0;
        double y0 = bbox_for_triangle.y0;
        double z0 = bbox_for_triangle.z0;
        //MAX POINT
        double x1 = bbox_for_triangle.x1;
        double y1 = bbox_for_triangle.y1;
        double z1 = bbox_for_triangle.z1;
        
        //RAY ITSELF-->ORIGIN
        double ox = ray.origin.x;
        double oy = ray.origin.y;
        double oz = ray.origin.z;
        //RAY ITSELF-->DIRECTION
        double dx = ray.direction.x;
        double dy = ray.direction.y;
        double dz = ray.direction.z;
        
        //MIN AND MAX VALUES AT X, Y, and Z COORDINATES
        double tx_min;
        double ty_min;
        double tz_min;
        double tx_max;
        double ty_max;
        double tz_max;
        
        double inverse_dx = 1.0 / dx;
        if(inverse_dx >= 0)
        {
            tx_min = (x0 - ox) * inverse_dx;
            tx_max = (x1 - ox) * inverse_dx;
        }
        else
        {
            tx_min = (x1 - ox) * inverse_dx;
            tx_max = (x0 - ox) * inverse_dx;
        }
        
        double inverse_dy = 1.0 / dy;
        if(inverse_dy >= 0)
        {
            ty_min = (y0 - oy) * inverse_dy;
            ty_max = (y1 - oy) * inverse_dy;
        }
        else
        {
            ty_min = (y1 - oy) * inverse_dy;
            ty_max = (y0 - oy) * inverse_dy;
        }
        
        double inverse_dz = 1.0 / dz;
        if(inverse_dz >= 0)
        {
            tz_min = (z0 - oz) * inverse_dz;
            tz_max = (z1 - oz) * inverse_dz;
        }
        else
        {
            tz_min = (z1 - oz) * inverse_dz;
            tz_max = (z0 - oz) * inverse_dz;
        }
        
        //INTERVAL IN WHICH HITS CAN BE VALID (t0, t1)
        double t0;
        double t1;
        
        //FIND LARGEST ENTERING T VALUE
        if(tx_min > ty_min)
        {
            t0 = tx_min;
        }
        else
        {
            t0 = ty_min;
        }
        if(tz_min > t0)
        {
            t0 = tz_min;
        }
        
        //FIND SMALLEST EXITING T VALUE
        if(tx_max < ty_max)
        {
            t1 = tx_max;
        }
        else
        {
            t1 = ty_max;
        }
        if(tz_max < t1)
        {
            t1 = tz_max;
        }
        
        return (t0 < t1 && t1 > 0);
    }
    
    
};

#endif
