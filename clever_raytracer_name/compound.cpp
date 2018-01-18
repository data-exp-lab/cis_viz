#include "compound.hpp"
#include "constants.hpp"
#include "leaf.hpp"
#include <iostream>

Compound::Compound()
{
    
}

Compound::~Compound()
{
    
}

//CALCULATE NEAREST HIT TARGET
bool Compound::hit(const Ray& ray, double& tmin, int& j_hit, int& updown)const
{
    double t;
    bool hit = false;
    tmin = kHugeValue;
    int num_triangles = triangleList.size();
    
    for(int j = 0; j < num_triangles; j++)
    {
        if(triangleList[j]->hit(ray, t) && (t < tmin))
        {
            hit = true;
            tmin = t;
            j_hit = j;
            if(ray.d * triangleList[j]->normal < 0)
            {
                updown = 1;
            }
            else
            {
                updown = -1;
            }
        }
    }
    return hit;
}

void Compound::add_triangle(Triangle* triangle_ptr)
{
    triangleList.push_back(triangle_ptr);
}

vector<Triangle*> Compound::get_triangleList()
{
    return triangleList;
}











