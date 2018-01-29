#ifndef __NORMAL__
#define __NORMAL__

#include "ray.hpp"
#include "point.hpp"

class Normal
{
public:
    double x, y, z;
    Normal(void);
    Normal(double a);
    Normal(double _x, double _y, double _z);
    Normal(const Normal& n);
    Normal(const raytrace::Vect& v);
    
    ~Normal(void);
    
    Normal& operator= (const Normal& rhs);
    Normal& operator= (const raytrace::Vect& rhs);
    Normal& operator= (const Point& rhs);
    
    Normal operator- (void) const;
    Normal operator+ (const Normal& n) const;
    Normal& operator+= (const Normal& n);
    
    double operator* (const raytrace::Vect& v) const;
    Normal operator* (const double a) const;
    
    void normalize(void);
    
};

inline Normal Normal::operator- (void) const
{
    return (Normal(-x, -y, -z));
}

inline Normal Normal::operator+ (const Normal& n) const
{
    return (Normal(x + n.x, y + n.y, z + n.z));
}

inline Normal& Normal::operator+= (const Normal& n)
{
    x += n.x;
    y += n.y;
    z += n.z;
    
    return (*this);
}

inline double Normal::operator* (const raytrace::Vect& v) const
{
    return (x * v.x + y * v.y + z * v.z);
}

inline Normal Normal::operator* (const double a) const
{
    return (Normal(x * a, y * a, z * a));
}

Normal operator* (const double a, const Normal& n);

inline Normal operator* (const double f, const Normal& n)
{
    return (Normal(f * n.x, f * n.y, f * n.z));
}

raytrace::Vect operator+ (const raytrace::Vect& v, const Normal& n);

inline raytrace::Vect operator+ (const raytrace::Vect& v, const Normal& n)
{
    return (raytrace::Vect(v.x + n.x, v.y + n.y, v.z + n.z));
}

raytrace::Vect operator- (const raytrace::Vect&, const Normal& n);

inline raytrace::Vect operator- (const raytrace::Vect& v, const Normal& n)
{
    return (raytrace::Vect(v.x - n.x, v.y - n.y, v.z - n.z));
}

double operator* (const raytrace::Vect& v, const Normal& n);

inline double operator* (const raytrace::Vect& v, const Normal& n)
{
    return (v.x * n.x + v.y * n.y + v.z * n.z);
}



#endif
