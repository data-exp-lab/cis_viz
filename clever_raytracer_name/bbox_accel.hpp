#ifndef __BBOX_ACCEL__
#define __BBOX_ACCEL__

#include <vector>

#include "vect.hpp"
#include "ray.hpp"
#include "constants.hpp"
#include "equations.hpp"
#include "normal.hpp"
#include "parameters.hpp"
#include "triangle.hpp"
#include "accel.hpp"
#include "bbox.hpp"

using namespace std;

class BBoxAcceleration : public Acceleration
{
public:
    //BBoxAcceleration(vector<std::unique_ptr<Triangle>>& t) : Acceleration(t) {}
    BBoxAcceleration(vector<Triangle*> tri) : Acceleration(tri) {}
    
    //virtual bool intersect(Ray& ray, const int& rayId, float& tHit) const
    virtual bool intersect(Ray& ray, float& tHit, vector<Triangle*> triangles)
    {
        Triangle* intersectedTriangle = nullptr;
        Vect ray_direction = ray.getRayDirection();
        Vect inv_direction = ray_direction.inverse();
        float t = kInfinity;

        //for(int i = 0; i < triangles.size(); i++)
        for(const auto& triangle: triangles)
        {
            BBox tri_bbox = triangle->getBoundingBox();
            //cout << "inside intersect for loop in bbox_accel" << endl;
            //cout << "triangles[" << i << "]: " << endl;
            //TEST IF RAY INTERSECTS THE BOX
            //if(triangles[i]->bbox.intersectBBox(ray, triangles[i]->getBoundingBox()))
            //if(triangle->bbox.intersectBBox(ray, triangle->getBoundingBox()))
            if(triangle->bbox.intersectBBox(ray, tri_bbox))
            {
                //TEST IF RAY INTERSECTS TRIANGLE; IF IT DOESN'T, CHECK IF INTERSECTION DISTANCE IS NEAREST-->IF IT PASSES, UPDATE TNEAR WITH T AND KEEP POINTER TO INTERSECTED TRIANGLE
                //if(triangles[i]->intersect(ray, t) && t < tHit)
                cout << "inside triangle intersection in bbox_accel" << endl;
                if(triangle->intersect(ray, t, triangles) && t < tHit)
                {
                    cout << "inside if that says triangle is intersected" << endl;
                    tHit = t;
                    //intersectedTriangle = triangles[i];//get();
                    intersectedTriangle = triangle;
                }
            }
        }
        //cout << "intersectedTriangle != nullptr: " << (intersectedTriangle != nullptr) << endl;
        return (intersectedTriangle != nullptr);
    }
    
protected:
    //vector<std::unique_ptr<Triangle>> triangles;
    //vector<Triangle*> tri;
    
    
};





#endif
