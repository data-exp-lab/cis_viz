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

inline Point Point::operator- (void) const
{
    return (Point(-x, -y, -z));
}

inline raytrace::Vector Point::operator- (const Point& p) const
{
    return (raytrace::Vector(x - p.x, y - p.y, z - p.z));
}

inline Point Point::operator- (const raytrace::Vector& v) const
{
    //return (Point&(x - v.x, y - v.y, z - v.z));
    return (Point(x - v.x, y - v.y, z - v.z));
}

inline Point Point::operator* (const double a) const
{
    return (Point(x * a, y * a, z * a));
}

inline double Point::d_squared(const Point& p) const
{
    return ((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z));
}

Point operator*(double a, const Point& p);

inline Point operator* (double a, const Point& p)
{
    return (Point(a * p.x, a * p.y, a * p.z));
}

