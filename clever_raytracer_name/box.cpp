#include "box.hpp"

Box::Box()
{
    
}

Box::Box(const double _x0, const double _x1, const double _y0, const double _y1, const double _z0, const double _z1) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), z0(_z0), z1(_z1)
{}

Box::~Box()
{
    
}

bool Box::hit(const raytrace::Ray& ray) const
{
    double kEpsilon = 0;
    double x0 = 0;
    double y0 = 0;
    double z0 = 0;
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;
    
    double ox = ray.origin.x;
    double oy = ray.origin.y;
    double oz = ray.origin.z;
    double dx = ray.direction.x;
    double dy = ray.direction.y;
    double dz = ray.direction.z;
    
    double tx_min;
    double ty_min;
    double tz_min;
    double tx_max;
    double ty_max;
    double tz_max;
    
    double a = 1.0 / dx;
    if(a >= 0)
    {
        tx_min = (x0 - ox) * a;
        tx_max = (x1 - ox) * a;
    }
    else
    {
        tx_min = (x1 - ox) * a;
        tx_max = (x0 - ox) * a;
    }
    
    double b = 1.0 / dy;
    if(b >= 0)
    {
        ty_min = (y0 - oy) * b;
        ty_max = (y1 - oy) * b;
    }
    else
    {
        ty_min = (y1 - oy) * b;
        ty_max = (y0 - oy) * b;
    }
    
    double c = 1.0 / dz;
    if(c >= 0)
    {
        tz_min = (z0 - oz) * c;
        tz_max = (z1 - oz) * c;
    }
    else
    {
        tz_min = (z1 - oz) * c;
        tz_max = (z0 - oz * c);
    }
    
    double t0;
    double t1;
    
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
    
    //DETERMINE IF THE RAY HIT THE BOX
    return (t0 < t1 && t1 > kEpsilon);
    
}

bool Box::inside(const Point& p) const
{
    return ((p.x > x0 && p.x < x1) && (p.y > y0 && p.y < y1) && (p.z > z0 && p.z < z1));
}

