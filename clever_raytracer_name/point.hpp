#ifndef __POINT__
#define __POINT__

//#include "ray.hpp"

namespace raytrace
{
    template<typename T> class Vector;
    typedef Vector<float> Vect;
}

class Point
{
public:
    float x;
    float y;
    float z;
    
    Point();
    Point(const double a);
    Point(const double a, const double b, const double c);
    Point(const Point& p);
    ~Point();
    
    Point& operator= (const Point& p);
    
    Point operator- (void) const;
    Point operator- (const Point& p) const;
    
    Point operator+ (const raytrace::Vect& v) const;
    Point operator- (const raytrace::Vect& v) const;
    
    Point operator* (const double a) const;
    double d_squared(const Point& p) const;
    double distance(const Point& p) const;
    
    Point operator^(const Point& p);
    double length(void);
    
    double getX();
    double getY();
    double getZ();
};

/*inline Point Point::operator- (void) const
{
    return (Point(-x, -y, -z));
}

inline Point Point::operator- (const Point& p) const
{
    return (raytrace::Vect(x - p.x, y - p.y, z - p.z));
}

inline Point Point::operator- (const raytrace::Vect& v) const
{
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

inline Point operator* (double a, const Point& p)
{
    return (Point(a * p.x, a * p.y, a * p.z));
}
*/

#endif
