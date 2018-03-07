#ifndef __COMPOUND__
#define __COMPOUND__

#include "triangle.hpp"
#include "ray.hpp"
#include <vector>

using namespace std;

class Compound
{
public:
    void add_triangle(Triangle* triangle);
    vector<Triangle*> get_triangleList();
    
    virtual bool hit(const raytrace::Ray& ray, double & tmin, int& j_hit, int& updown, Constants cs) const;
    
    Compound();
    virtual ~Compound();
    
protected:
    vector<Triangle*> triangleList;
};

#endif
