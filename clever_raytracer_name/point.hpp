#include "ray.hpp"

class Point
{
public:
    double x;
    double y;
    double z;
    
    Point();
    Point(const double a);
    Point(const double a, const double b, const double c);
    Point(const Point& p);
    ~Point();
    
    Point& operator= (const Point& p);
    
    Point operator- (void) const;
    Vect operator- (const Point& p) const;
    
    Point operator+ (const Vect& v) const;
    Point operator- (const Vect& v) const;
    
    Point operator* (const double a) const;
    
    double d_squared(cosnt Point& p) const;
    double distance(const Point& p) const;
};

inline Point Point::operator- (void) const
{
    return (Point(-x, -y, -z));
}

inline Vect Point::operator- (const Point& p) const
{
    return (Vect(x - p.x, y - p.y, z - p.z));
}

inline Point Point::operator- (const Vect& v) const
{
    return (Point&(x - v.x, y - v.y, z - v.z));
}

inline double Point::operator* (cosnt double a) const
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


