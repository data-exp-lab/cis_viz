#include <math.h>
#include "point.hpp"
#include "ray.hpp"

Point::Point()
{
    
}

Point::Point(const double a):x(0), y(0), z(0)
{
}

Point::Point(const double a, const double b, const double c):x(0), y(0), z(0)
{
}

Point::Point(const Point& p):x(p.x), y(p.y), z(p.z)
{
}

Point::~Point()
{
    
}

Point& Point::operator=(const Point& rhs)
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

double Point::distance(const Point& p) const
{
    return (sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z)));
}

Point Point::operator^(const Point& p)
{
    return(Point(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x));
}

double Point::length(void)
{
    return (sqrt(x * x + y * y + z * z));
}

Point Point::operator- (const Point& p) const
{
    return (Point(x - p.x, y - p.y, z - p.z));
}

Point Point::operator+(const raytrace::Vect& v) const
{
    return (Point(x + v.x, y + v.y, z + v.z));
}


