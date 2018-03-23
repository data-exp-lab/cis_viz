#include "normal.hpp"
#include "ray.hpp"
#include <math.h>

Normal::Normal(void) : x(0.0), y(0.0), z(0.0)
{}

Normal::Normal(double a) : x(a), y(a), z(a)
{}

Normal::Normal(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
{}

Normal::Normal(const Normal& n) : x(n.x), y(n.y), z(n.z)
{}

Normal::Normal(const raytrace::Vect& v) : x(v.x), y(v.y), z(v.z)
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
    
}

void Normal::normalize(double x, double y, double z)
{
    double length = sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
}

//FIX
void Normal::normalize(Point x, Point y, Point z)
{
    //double length = sqrt(x * x + y * y + z * z);
    //x /= length;
    //y /= length;
    //z /= length;
}

Normal operator^(Point& a, Point& b)
{
    return (Normal(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x));
    
}

inline raytrace::Vect operator- (const raytrace::Vect& v, const Normal& n)
{
    return (raytrace::Vect(v.x - n.x, v.y - n.y, v.z - n.z));
}

double Normal::operator* (const raytrace::Vect& v) const
{
    return (x * v.x + y * v.y + z * v.z);
}



