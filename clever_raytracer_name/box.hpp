#ifndef __BOX__
#define __BOX__

#include "ray.hpp"

class Box
{
public:
    double x0;
    double y0;
    double z0;
    double x1;
    double y1;
    double z1;
    
    Box();
    Box(const double x0, const double x1, const double y0, const double y1, const double z0, const double z1);
    
    bool hit(const raytrace::Ray& ray) const;
    bool inside(const Point& p) const;
    
    virtual ~Box();
    
};
    
#endif
