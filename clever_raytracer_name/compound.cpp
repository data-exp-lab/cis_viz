#include "compound.hpp"
#include "constants.hpp"
#include "leaf.hpp"
#include "ray.hpp"
#include <iostream>
#include "normal.hpp"

Compound::Compound()
{
    
}

Compound::~Compound()
{
    
}

//CALCULATE NEAREST HIT TARGET
bool Compound::hit(const raytrace::Ray& ray, double& tmin, int& j_hit, int& updown, Constants cs)const
{
    double t;
    bool hit = false;
    tmin = cs.kHugeValue;
    int num_triangles = triangleList.size();
    
    for(int j = 0; j < num_triangles; j++)
    {
        if(triangleList[j]->hit(ray, t, cs) && (t < tmin))
        {
            hit = true;
            tmin = t;
            j_hit = j;
            
            raytrace::Vect dir_by_triangleList = ray.direction * triangleList[0]->normal;
            if(dir_by_triangleList.x < 0 && dir_by_triangleList.y < 0 && dir_by_triangleList.z < 0)
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











