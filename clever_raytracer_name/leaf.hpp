#ifndef __LEAF__
#define __LEAF__

#include "parameters.hpp"
#include "constants.hpp"
#include "ray.hpp"
#include "triangle.hpp"

class Leaf
{
public:
    //MAKE DYNAMIC
    double ppfd_direct_list[368];
    double ppfd_diffuse_list[368];
    
    Leaf();
    virtual ~Leaf();
    double* runCurves(Constants cs, Parameters ip, char X[], double x_start, double x_end, char Y[], char T[], double[3]);
    
    raytrace::Vect get_reflect_dir(raytrace::Vect L, raytrace::Vect N);
    void get_reflect_dir_2(raytrace::Ray ray, Triangle* triangle_ptr, vector<raytrace::Ray*> &scatter_rays, double ignore_Photon_Flux_threshold);
    raytrace::Vect get_transmit_dir(raytrace::Vect L, raytrace::Vect N);
    
    void run_FarquharModel(double startHour, double endHour, double hourInterval, Constants &cs, Climate &cl);
    void run_FarquharModel_core(Constants &cs, Parameters &ps, double plantHeight);
    
private:
    void randReflectRayDir(raytrace::Vect N, raytrace::Vect & r);
    void randThroughRayDir(raytrace::Vect N, raytrace::Vect & r);
    
    double getfr(double hv_wave_length, raytrace::Vect V, raytrace::Vect L, raytrace::Vect N);
    raytrace::Vect vMidLine(raytrace::Vect An, raytrace::Vect B);
    double vAngle(raytrace::Vect An, raytrace::Vect B);
    
};

#endif
