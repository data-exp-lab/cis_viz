#include "grid.hpp"
#include "constants.hpp"
#include "equations.hpp"
#include "leaf.hpp"
#include "box.hpp"
#include "ray.hpp"
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

Grid::Grid(double ignore_threshold)
{
    ignore_Photon_Flux_threshold = ignore_threshold;
    
    Leaf* leaf_optics = new Leaf();
}

void Grid::setup_cells(Point p0, Point p1)
{
    /*//FIND MINIMUM AND MAXIMUM COORDINATES OF THE GRID
    box.x0 = p0.x - cs.kEpsilon;
    box.y0 = p0.y - cs.kEpsilon;
    box.z0 = p0.z - cs.kEpsilon;
    box.x1 = p1.x + cs.kEpsilon;
    box.y1 = p1.y + cs.kEpsilon;
    box.z1 = p1.z + cs.kEpsilon;
    */
    box = Box(p0.x, p1.x, p0.y, p1.y, p0.z, p1.z);
    
    //COMPUTE NUMBER OF CELLS IN THE X, Y, AND Z DIRECTIONS
    int num_triangles = triangles.size();
    float wx = p1.x - p0.x;
    float wy = p1.y - p0.y;
    float wz = p1.z - p0.z;
    
    float multiplier = 2.0;
    float temp333 = 0.3333333;
    
    float s = pow(wx * wy * wz / num_triangles, temp333);
    
    nx = (int) (multiplier * wx / s + 1);
    ny = (int) (multiplier * wy / s + 1);
    nz = (int) (multiplier * wz / s + 1);
    
    //SET UP THE ARRAY OF CELLS WITH NULL POINTERS
    int num_cells = nx * ny * nz;
    cells.reserve(num_triangles);
    
    for(int j = 0; j < num_cells; j++)
    {
        cells.push_back(NULL);
    }
    
    //SET UP TEMPORARY ARRAY TO HOLD THE NUMBER OF OBJECTS IN EACH CELL
    vector<int> counts;
    counts.reserve(num_cells);
    
    for(int j = 0; j < num_cells; j++)
    {
        counts.push_back(0);
    }
    
    //PUT OBJECTS INTO CELLS
    Box obj_box;
    int index;
    //CANOPY HEIGHT; USED IN WIND SPEED FOR ENERGY BALANCE CALCULATION
    double plantHeight1 = 0;
    
    for(int j = 0; j < num_triangles; j++)
    {
        obj_box = triangles[j]->get_bounding_box();
        if(triangles[j]->v0.z > plantHeight1)
        {
            plantHeight1 = triangles[j]->v0.z;
        }
        
        //IF TRIANGLE IS OUT OF THE BOX, DO NOT PLACE IN GRID
        if(obj_box.x0 < box.x0 || obj_box.y0 < box.y0 || obj_box.z0 < box.z0 || obj_box.x1 > box.x1 || obj_box.y1 > box.y1 || obj_box.z1 > box.z1)
        {
            continue;
        }
        
        //COMPUTE CELL INDICES FOR THE CORNERS OF THE OBJECT BOX
        int ixmin = clamp((obj_box.x0 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
        int iymin = clamp((obj_box.y0 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
        int izmin = clamp((obj_box.z0 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
        int ixmax = clamp((obj_box.x1 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
        int iymax = clamp((obj_box.y1 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
        int izmax = clamp((obj_box.z1 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
        
        //ADD TRIANGLES TO CELLS
        for(int iz = izmin; iz <= izmax; iz++)
        {
            for(int iy = iymin; iy <= iymax; iy++)
            {
                //for(int iz = izmin; iz <= izmax; iz++)
                //{
                    for(int ix = ixmin; ix <= ixmax; ix++)
                    {
                        index = ix + nx * iy + nx * ny * iz;
                        
                        if(counts[index] == 0)
                        {
                            Compound* compound_ptr = new Compound;
                            cells[index] = compound_ptr;
                        }
                        //ADD TRIANGLE TO CELL
                        cells[index]->add_triangle(triangles[j]);
                        counts[index] += 1;
                    }
                //}
            }
        }
    }
    
    plantHeight = plantHeight1;
    
}

bool Grid::hit(raytrace::Ray& ray, double& t, const int& hour_th, int& lightType, Constants &cs) const
{
    double ox = ray.origin.x;
    double oy = ray.origin.y;
    double oz = ray.origin.z;
    double dx = ray.direction.x;
    double dy = ray.direction.y;
    double dz = ray.direction.z;
    
    double x0 = box.x0;
    double y0 = box.y0;
    double z0 = box.z0;
    double x1 = box.x1;
    double y1 = box.y1;
    double z1 = box.z1;
    
    double tx_min;
    double ty_min;
    double tz_min;
    double tx_max;
    double ty_max;
    double tz_max;
    
    double a = 1.0 / dx;
    if(a >= 0)
    {
        tx_min = (x0 - ox) * a;
        tx_max = (x1 - ox) * a;
    }
    else
    {
        tx_min = (x1 - ox) * a;
        tx_max = (x0 - ox) * a;
    }
    
    double b = 1.0 / dy;
    if(b >= 0)
    {
        ty_min = (y0 - oy) * b;
        ty_max = (y0 - oy) * b;
    }
    else
    {
        ty_min = (y1 - oy) * b;
        ty_max = (y0 - oy) * b;
    }
    
    double c = 1.0 / dz;
    if(c >= 0)
    {
        tz_min = (z0 - oz) * c;
        tz_max = (z0 - oz) * c;
    }
    
    double t0;
    double t1;
    
    if(tx_min > ty_min)
    {
        t0 = tx_min;
    }
    else
    {
        t0 = ty_min;
    }
    
    if(tz_min > t0)
    {
        t0 = tz_min;
    }
    
    if(tx_max < ty_max)
    {
        t1 = tx_max;
    }
    else
    {
        t1 = ty_max;
    }
    
    if(tz_max < t1)
    {
        t1 = tz_max;
    }
    
    if(t0 > t1)
    {
        return(false);
    }
    
    //INITIAL CELL COORDINATES
    int ix;
    int iy;
    int iz;
    
    if(box.inside(ray.origin))
    {
        ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
        iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
        iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
    }
    else
    {
        //INITIAL HIT POINT WITHIN GRID
        Point p = ray.origin + t0 * ray.direction;
        
        ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
        iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
        iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
    }
    
    //RAY PARAMETER INCREMENTS PER CELL IN X, Y, AND Z DIRECTIONS
    double dtx = (tx_max - tx_min) / nx;
    double dty = (ty_max - ty_min) / ny;
    double dtz = (tz_max - tz_min) / nz;
    
    double tx_next;
    double ty_next;
    double tz_next;
    
    int ix_step;
    int iy_step;
    int iz_step;
    int ix_stop;
    int iy_stop;
    int iz_stop;
    int ix_start;
    int iy_start;
    
    if(dx > 0)
    {
        tx_next = tx_min + (ix + 1) * dtx;
        ix_step = +1;
        ix_start = 0;
        ix_stop = nx;
    }
    else
    {
        tx_next = tx_min + (nx - ix) * dtx;
        ix_step = -1;
        ix_start = nx - 1;
        ix_stop = -1;
    }
    
    if(dx == 0.0)
    {
        tx_next = cs.kHugeValue;
        ix_step = -1;
        ix_start = nx - 1;
        ix_stop = -1;
    }
    
    if(dy > 0)
    {
        ty_next = ty_min + (iy + 1) * dty;
        iy_step = +1;
        ix_start = 0;
        ix_stop = ny;
    }
    else
    {
        ty_next = ty_min + (ny - iy) * dty;
        iy_step = -1;
        iy_start = ny - 1;
        iy_stop = -1;
    }
    
    if(dy == 0.0)
    {
        ty_next = cs.kHugeValue;
        iy_step = -1;
        iy_start = ny - 1;
        iy_stop = -1;
    }
    
    if(dz > 0)
    {
        tz_next = tz_min + (iz + 1) * dtz;
        iz_step = +1;
        iz_stop = nz;
    }
    else
    {
        tz_next = tz_min + (nz - iz) * dtz;
        iz_step = -1;
        iz_stop = -1;
    }
    
    if(dz == 0.0)
    {
        tz_next = cs.kHugeValue;
        iz_step = -1;
        iz_stop = -1;
    }
    
    //TRAVERSE THE GRID
    bool hit;
    int j_hit = -1;
    int updown = 0;
    
    while(true)
    {
        Compound* compound_ptr = cells[ix + nx * iy + nx * ny * iz];
        
        if(tx_next < ty_next && tx_next < tz_next)
        {
            if(compound_ptr && compound_ptr->hit(ray, t, j_hit, updown, cs) && t < tx_next)
            {
                //DIRECT LIGHT
                if(lightType == 1)
                {
                    if(updown == 1)
                    {
                        compound_ptr->get_triangleList()[j_hit]->photonFlux_up_dir[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                    }
                    if(updown == -1)
                    {
                        compound_ptr->get_triangleList()[j_hit]->photonFlux_down_dir[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                    }
                }
                //DIFFUSE LIGHT
                else if(lightType == 2)
                {
                    if(updown == 1)
                    {
                        compound_ptr->get_triangleList()[j_hit]->photonFlux_up_diff[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                    }
                    if(updown == -1)
                    {
                        compound_ptr->get_triangleList()[j_hit]->photonFlux_down_diff[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                    }
                }
                //SCATTER LIGHT
                else
                {
                    if(updown == 1)
                    {
                        compound_ptr->get_triangleList()[j_hit]->photonFlux_up_scat[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                    }
                    if(updown == -1)
                    {
                        compound_ptr->get_triangleList()[j_hit]->photonFlux_down_scat[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                    }
                }
                
                hit = generate_scatter_rays(ray, compound_ptr->get_triangleList()[j_hit], hour_th, cs);
                
                return hit;
                
            }
            
            tx_next += dtx;
            ix += ix_step;
            
            if(ix == ix_stop)
            {
                ray.origin.x = ray.origin.x - ix_step * (box.x1 - box.x0);
                ix = ix_start;
                
                continue;
            }
        }
        else
        {
            if(ty_next < tz_next)
            {
                if(compound_ptr && compound_ptr->hit(ray, t, j_hit, updown, cs) && t < ty_next)
                {
                    //DIRECT LIGHT
                    if(lightType == 1)
                    {
                        if(updown == 1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_up_dir[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                        if(updown == -1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_down_dir[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                    }
                    //DIFFUSE LIGHT
                    else if(lightType == 2)
                    {
                        if(updown == 1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_up_diff[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                        if(updown == -1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_down_diff[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                    }
                    //SCATTER LIGHT
                    else
                    {
                        if(updown == 1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_up_scat[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                        if(updown == -1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_down_scat[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                    }
                    
                    hit = generate_scatter_rays(ray, compound_ptr->get_triangleList()[j_hit], hour_th, cs);
                    return hit;
                }
                ty_next += dty;
                iy += iy_step;
                if(iy == iy_stop)
                {
                    ray.origin.y = ray.origin.y - iy_step * (box.y1 - box.y0);
                    iy = iy_start;
                    
                    continue;
                }
            }
            else
            {
                if(compound_ptr && compound_ptr->hit(ray, t, j_hit, updown, cs) && t < tz_next)
                {
                    //DIRECT LIGHT
                    if(lightType == 1)
                    {
                        if(updown == 1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_up_dir[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                        if(updown == -1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_down_dir[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                    }
                    //DIFFUSE LIGHT
                    else if(lightType == 2)
                    {
                        if(updown == 1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_up_diff[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                        if(updown == -1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_down_diff[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                    }
                    //SCATTER LIGHT
                    else
                    {
                        if(updown == 1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_up_scat[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                        if(updown == -1)
                        {
                            compound_ptr->get_triangleList()[j_hit]->photonFlux_down_scat[hour_th] += (ray.photonFlux2 * (1 - compound_ptr->get_triangleList()[j_hit]->kLeafReflectance - compound_ptr->get_triangleList()[j_hit]->kLeafTransmittance));
                        }
                    }
                    hit = generate_scatter_rays(ray, compound_ptr->get_triangleList()[j_hit], hour_th, cs);
                    return hit;
                }
                tz_next += dtz;
                iz += iz_step;
                if(iz == iz_stop)
                {
                    return (false);
                }
            }
        }
    }
}

bool Grid::hit_scatter_rays(vector<raytrace::Ray*> scatter_rays, const int& hour_t, Constants &cs) const
{
    return true;
}

bool Grid::generate_scatter_rays(raytrace::Ray& ray, Triangle* triangle_ptr, const int& hour_th, Constants &cs) const
{
    vector<raytrace::Ray*> scatter_rays;
    triangle_ptr->compute_normal();
    raytrace::Vect normal_triangle(triangle_ptr->normal);
    
    raytrace::Vect norm_by_direction = normal_triangle * ray.direction;
    
    //IF L AND N ARE OPPOSITE DIRECTION
    //IF NOT, FLIP THE N DIRECTION
    if(norm_by_direction.x > 0 && norm_by_direction.y > 0 && norm_by_direction.z > 0)
    {
        normal_triangle.negative();
    }
    
    Leaf* leaf_optics = new Leaf();
    
    double pf = ray.photonFlux2 * triangle_ptr->kLeafReflectance;
    if(pf > ignore_Photon_Flux_threshold)
    {
        raytrace::Vect L = ray.direction;// * -1;
        L.negative();
        raytrace::Vect reflect_d = leaf_optics->get_reflect_dir(L, normal_triangle, cs);
        scatter_rays.push_back(new raytrace::Ray(triangle_ptr->hit_point, reflect_d, pf));
    }
    
    double pf2 = ray.photonFlux2 * triangle_ptr->kLeafTransmittance;
    if(pf2 > ignore_Photon_Flux_threshold)
    {
        raytrace::Vect L = ray.direction;// * -1;
        L.negative();
        raytrace::Vect transmit_d = leaf_optics->get_transmit_dir(L, normal_triangle, cs);
        scatter_rays.push_back(new raytrace::Ray(triangle_ptr->hit_point, transmit_d, pf2));
    }
    
    delete leaf_optics;
    
    //SCATTER LIGHT
    int lightType3 = 3;
    for(unsigned int k = 0; k < scatter_rays.size(); k++)
    {
        double t = cs.kHugeValue;
        raytrace::Ray ray = raytrace::Ray(scatter_rays[k]);
        this->hit(ray, t, hour_th, lightType3, cs);
    }
    
    //DELETE THE VECTORS
    for(vector<raytrace::Ray*>::iterator iter = scatter_rays.begin(); iter != scatter_rays.end(); ++iter)
    {
        delete *iter;
    }
    
    scatter_rays.erase(scatter_rays.begin(), scatter_rays.end());
    
    return true;
    
}

void Grid::add_triangle(Triangle* triangle_ptr)
{
    triangles.push_back(triangle_ptr);
}

vector<Compound*> Grid::get_cells()
{
    return cells;
}

vector<Triangle*> Grid::get_triangles()
{
    return triangles;
}









