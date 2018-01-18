#include <math.h>
#include "point.hpp"

Point::Point():x(0), y(0), z(0)
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

Point& Point::operator=(cosnt Point& rhs)
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

