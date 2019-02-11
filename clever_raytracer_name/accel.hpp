#ifndef __ACCELERATION__
#define __ACCELERATION__

#include <memory>
#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>
#include <chrono>
#include <queue>

#include "vect.hpp"
#include "ray.hpp"
#include "constants.hpp"
#include "equations.hpp"
#include "normal.hpp"
#include "parameters.hpp"
#include "triangle.hpp"

using namespace std;

const float kInfinity = std::numeric_limits<float>::max();

class Acceleration
{
public:
    //Acceleration(vector<std::unique_ptr<Triangle>>& t) : triangles(std::move(t)) {}
    Acceleration(vector<Triangle*> tri) : triangles2(std::move(tri)) {}
    virtual ~Acceleration() {}
    
    //virtual bool intersect(Ray& ray, const int& rayId, float& tHit) const
    virtual bool intersect(Ray& ray, float& tHit, vector<Triangle*> triangles1)
    {
        //POINT TO GEOMETRY (TRIANGLES IN THIS CASE)
        Triangle* intersectedTriangle = nullptr;
        float t = kInfinity;
        //for(int i = 0; i < triangles.size(); i++)
        for(const auto& triangle: triangles1)
        {
            //if(triangles[i]->intersect(ray, t) && t < tHit)
            if(triangle->intersect(ray, t, triangles1) && t < tHit)
            {
                //intersectedTriangle = triangles[i];//.get();
                intersectedTriangle = triangle;
                tHit = t;
            }
        }
        
        return (intersectedTriangle != nullptr);
    }
    
protected:
    //change scene_triangles to this-->either move it to here or change scene_triangles to contain unique_ptr
    //const vector<std::unique_ptr<Triangle>> triangles;
    vector<Triangle*> triangles2;
 
};

#endif
