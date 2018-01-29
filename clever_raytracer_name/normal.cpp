#include "normal.hpp"
#include <math.h>

Normal::Normal(void) : x(0.0), y(0.0), z(0.0)
{}

Normal::Normal(double a) : x(a), y(a), z(a)
{}

Normal::Normal(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
{}

Normal::Normal(const Normal& n) : x(n.x), y(n.y), z(n.z)
{}

Normal::Normal(const Vect& v) : x(v.x), y(v.y), z(v.z)
{}

Normal::~Normal(void)
{}

Normal& Normal::operator= (const Normal& rhs)
{
    if(this == &rhs)
    {
        return (*this);
    }
    
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    
    return (*this);
}

Normal& Normal::operator= (const raytrace::Vect& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    
    return (*this);
}

Normal& Normal::operator= (const Point& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    
    return (*this);
}

void Normal::normalize(void)
{
    double length = sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
}




