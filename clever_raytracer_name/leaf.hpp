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
    
    Vect get_reflect_dir(Vect L, Vect N);
    void get_reflect_dir_2(Ray ray, Triangle* triangle_ptr, vector<Ray*> &scatter_rays, double ignore_Photon_Flux_threshold);
    Vect get_transmit_dir(Vect L, Vect N);
    
    void run_FarquharModel(double startHour, double endHour, double hourInterval, Parameters &ip, Climate &cl);
    void run_FarquharModel_core(Constants &cs, Parameters &ip, dobule plantHeight);
    
private:
    void randReflectRayDir(Vect N, Vect & r);
    void randThroughRayDir(Vect N, Vect & r);
    
    double getfr(double hv_wave_length, Vect V, Vect L, Vect N);
    Vect vMidLine(Vect An, Vect B);
    double vAngle(Vect An, Vect B);
    
};
