#ifndef __NORMAL__
#define __NORMAL__

#include "ray.hpp"
#include "vect.hpp"
#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>

class Normal
{
public:
    double x;
    double y;
    double z;
    
    Normal(void) : x(0.0), y(0.0), z(0.0){}
    Normal(double a) : x(a), y(a), z(a){}
    Normal(double _x, double _y, double _z) : x(_x), y(_y), z(_z){}
    Normal(const Normal& n) : x(n.x), y(n.y), z(n.z){}
    Normal(const Vect& v) : x(v.x), y(v.y), z(v.z){}
    ~Normal(void){}
    
    Normal& operator= (const Normal& rhs)
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
    
    Normal& operator= (const Vect& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        
        return (*this);
    }
    
    Normal operator- (void) const;
    Normal operator+ (const Normal& n) const;
    Normal& operator+= (const Normal& n);
    
    double operator* (const Vect& v) const;
    Normal operator* (const double a) const;
    
    /*Normal operator^(Vect& a, Vect& b)
    {
        return (Normal(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x));
    }*/
    
    
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

Normal operator^(Vect& a, Vect& b)
{
    return (Normal(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x));
}

#endif
