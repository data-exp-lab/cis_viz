#ifndef __POINT__
#define __POINT__

namespace raytrace {
  template<typename T> class Vector;
  typedef Vector<float> Vect;
}

// We do cross-includes, so we will forward declare a handful of things.


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
    raytrace::Vect operator- (const Point& p) const;
    
    Point operator+ (const raytrace::Vect& v) const;
    Point operator- (const raytrace::Vect& v) const;
    
    inline Point operator* (const double a) const;
    
    double d_squared(const Point& p) const;
    double distance(const Point& p) const;
};

#endif
