#ifndef __GRID__
#define __GRID__

#include "point.hpp"
#include "compound.hpp"
#include "ray.hpp"
#include "triangle.hpp"
#include "leaf.hpp"
#include <vector>

class Grid
{
public:
    double ignore_Photon_Flux_threshold;
    
    Grid(double ignore_threshold);
    virtual ~Grid();
    
    void setup_cells(Point p0, Point p1);
    void add_triangle(Triangle* triangle);
    
    vector<Compound*> get_cells();
    vector<Triangle*> get_triangles();
    
    bool hit(raytrace::Ray & ray, double& tmin, const int& hour_th, int& firstStep) const;
    
    double plantHeight;
    
private:
    vector<Compound*> cells;
    vector<Triangle*> triangles;
    
    int nx;
    int ny;
    int nz;
    
    bool hit_scatter_rays(vector<raytrace::Ray*> scatter_rays, const int& hour_th) const;
    bool generate_scatter_rays(raytrace::Ray& ray, Triangle* triangle_ptr, const int& hour_th) const;
    
    Point min_coordinates(void);
    Point max_coordinates(void);
    
    Leaf* leaf_optics;
    
    
};






#endif







