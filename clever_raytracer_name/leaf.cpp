#include "leaf.hpp"
#include "equations.hpp"
#include "farquharModel.hpp"
#include "thermal.hpp"
#include "stomata.hpp"
#include "grid.hpp"
#include "energyBalance.hpp"
#include "cimodel.hpp"
#include "constants.hpp"
#include <cmath>
#include <math.h>
#include <iostream>
#include "ray.hpp"

using namespace std;


Leaf::Leaf()
{
    
}

Leaf::~Leaf()
{
    
}

double* Leaf::runCurves(Constants cs, Parameters ip, char X[], double x_start, double x_end, char Y[], char T[], double t[3])
{
    double *x_ptr = NULL;
    double *t_ptr = NULL;
    double *y_ptr = NULL;

    x_ptr = &ip.PPFD;
    t_ptr = &cs.CA;
    y_ptr = &ip.A;
    
    Leaf* LPS = new Leaf();
    double plantHeight = 100; //CM
    double interval = (x_end - x_start) / 49;
    
    
    int k = 0;
    
    for(int i = 0; i < 3; i++)
    {
        *t_ptr = t[i];
        for(int j = 0; j < 50; j++)
        {
            *x_ptr = x_start + j * interval;
        }
        
        LPS->run_FarquharModel_core(cs, ip, plantHeight);
        
        k++;
    }
    
    return y_ptr;
}
            

            
raytrace::Vect Leaf::get_reflect_dir(raytrace::Vect L, raytrace::Vect N, Constants cs)
{
    double E = 0;
    double FR = 0;
    raytrace::Vect R;
    int xxx = 0;
    while(true)
    {
        xxx++;
        randReflectRayDir(N, R, cs);
        FR = getfr(650, R, L, N, cs);
        
        E = rand() * cs.invRAND_MAX;
        
        if(E < FR)
        {
            break;
        }
    }
    
    return R;
}

void Leaf::get_reflect_dir_2(raytrace::Ray ray, Triangle* triangle_ptr, vector<raytrace::Ray*> &scatter_rays, double ignore_Photon_Flux_threshold, Constants cs)
{
    double E = 0;
    double FR1;
    double FR2;
    double FR3;
    double FR4;
    double FR5;
    double FRA;
    raytrace::Vect R1;
    raytrace::Vect R2;
    raytrace::Vect R3;
    raytrace::Vect R4;
    raytrace::Vect R5;
    
    triangle_ptr->compute_normal();
    raytrace::Vect normal_triangle(triangle_ptr->normal);

    if(normal_triangle.x * ray.direction.x > 0 && normal_triangle.y * ray.direction.y > 0 && normal_triangle.z * ray.direction.z > 0)
    {
        normal_triangle.negative();
    }
    
    //TOTAL REFLECT RAYS ENERGY
    double PF = ray.photonFlux2 * triangle_ptr->kLeafReflectance;
    raytrace::Vect neg_direction = ray.direction * -1;
    
    randReflectRayDir(normal_triangle, R1, cs);
    FR1 = getfr(650, R1, neg_direction, normal_triangle, cs);
    randReflectRayDir(normal_triangle, R2, cs);
    FR2 = getfr(650, R2, neg_direction, normal_triangle, cs);
    randReflectRayDir(normal_triangle, R3, cs);
    FR3 = getfr(650, R3, neg_direction, normal_triangle, cs);
    randReflectRayDir(normal_triangle, R4, cs);
    FR4 = getfr(650, R4, neg_direction, normal_triangle, cs);
    randReflectRayDir(normal_triangle, R5, cs);
    FR5 = getfr(650, R5, neg_direction, normal_triangle, cs);
    
    FRA = FR1 + FR2 + FR3 + FR4 + FR5;
    
    if(PF > ignore_Photon_Flux_threshold)
    {
        scatter_rays.push_back(new raytrace::Ray(triangle_ptr->hit_point, R1, PF * FR1 / FRA));
        scatter_rays.push_back(new raytrace::Ray(triangle_ptr->hit_point, R2, PF * FR2 / FRA));
        scatter_rays.push_back(new raytrace::Ray(triangle_ptr->hit_point, R3, PF * FR3 / FRA));
        scatter_rays.push_back(new raytrace::Ray(triangle_ptr->hit_point, R4, PF * FR4 / FRA));
        scatter_rays.push_back(new raytrace::Ray(triangle_ptr->hit_point, R5, PF * FR5 / FRA));
    }
}

void Leaf::randReflectRayDir(raytrace::Vect N, raytrace::Vect & r, Constants cs)
{
    while(true)
    {
        double x = rand() * cs.invRAND_MAX * 2 - 1;
        double y = rand() * cs.invRAND_MAX * 2 - 1;
        double z = rand() * cs.invRAND_MAX * 2 - 1;
        
        if(x * x + y * y + z * z > 1)
        {
            continue;
        }
        if(x * N.x + y * N.y + z * N.z <= 0)
        {
            continue;
        }
        
        r.x = x;
        r.y = y;
        r.z = z;
        r.normalize();
        
        break;
    }
}
            
void Leaf::randThroughRayDir(raytrace::Vect N, raytrace::Vect & r, Constants cs)
{
    while(true)
    {
        double x = rand() * cs.invRAND_MAX * 2 - 1;
        double y = rand() * cs.invRAND_MAX * 2 - 1;
        double z = rand() * cs.invRAND_MAX * 2 - 1;
        
        if(x * x + y * y + z * z)
        {
            continue;
        }
        if(x * N.x + y * N.y + z * N.z >= 0)
        {
            continue;
        }
        
        r.x = x;
        r.y = y;
        r.z = z;
        r.normalize();
        
        break;
    }
}
            
raytrace::Vect Leaf::get_transmit_dir(raytrace::Vect L, raytrace::Vect N, Constants cs)
{
    raytrace::Vect t;
    randThroughRayDir(N, t, cs);
    
    return t;
}
            
double Leaf::getfr(double hv_wave_length, raytrace::Vect V, raytrace::Vect L, raytrace::Vect N, Constants cs)
{
    double S = cs.BRDF_s;
    double F0 = cs.BRDF_F0;
    double RD = cs.BRDF_Rd;
    double M = cs.BRDF_m;
    double TWO = 2.0;
    
    raytrace::Vect H = vMidLine(L, V);
    
    double G1 = TWO * (N * H) * (N * V) / (V * H);
    double G2 = TWO * (N * H) * (N * L) / (V * H);
    double G_min = min(G1, G2);
    double G = min(1, G_min);
    
    double A = vAngle(N, H);
    double D = 1 / (M * M * pow(cos(A), 4)) * exp(-pow(tan(A) / M, 2));
    double A2 = vAngle(N, L);
    double F = F0 + (1 - F0) * pow(1 - cos(A2), 5);
    
    double FR = S * F / cs.PI * D * G / ((N * L) * (N * V)) + (1 - S) * RD;
    
    return FR;
}
            
raytrace::Vect Leaf::vMidLine(raytrace::Vect A, raytrace::Vect B)
{
    raytrace::Vect C = A + B;
    C.normalize();
    
    return C;
}
            
double Leaf::vAngle(raytrace::Vect A, raytrace::Vect B)
{
    double AB = (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
    //ACOS OF DOT PRODUCT EQUALS ANGLE
    double angle = acos((AB) / (A.length() * B.length()));
    
    //double angle = acos((A * B) / (A.length() * B.length()));
    
    return angle;
}

void Leaf::run_FarquharModel_core(Constants &cs, Parameters &ip, double plantHeight)
    {
        FarquharModel fm;
        Thermal thm;
        Stomata sm;
        EnergyBalance eb;
        CiModel cim;
        
        ip = thm.temperatureAdj(cs, ip);
        
        //PRE-RUN PHOTOSYNTHESIS
        fm.calculation(cs, ip);
        
        ip = sm.emperical_model_1(cs, ip);
        ip = cim.calculateCi(cs, ip);
        ip = sm.const_gs(cs, ip);
        
        //CALL FARQUHAR MODEL TO CALCULATE A
        //OUTPUT IS IP WITH THE NECESSARY PARAMETERS
        fm.calculation(cs, ip);
        
        double DT = 100;
        double OLDT = 0;
        int N_LOOP = 0;
        
        while(abs(DT) > 0.02 && N_LOOP < 20)
        {
            OLDT = ip.Tleaf;
            ip = thm.temperatureAdj(cs, ip);
            
            //GS
            ip = sm.emperical_model_1(cs, ip);
            //CI
            ip = cim.calculateCi(cs, ip);
            //A
            fm.calculation(cs, ip);
            
            //CALCULATE NEW LEAF TEMPERATURE
            ip = eb.calculateLeafT_withWindSpeed(cs, ip, plantHeight);
            DT = ip.Tleaf - OLDT;
            
            N_LOOP++;
        }
    }

            
void Leaf::run_FarquharModel(Grid* grid, double startHour, double endHour, double hourInterval, Parameters &ip, Constants &cs, Climate &cl)
{
    Leaf leaf;
    Parameters ps;
    double facet_area;
    int leafID;
    double PPFD;
    double CLAI;
    
    vector<Triangle*> v = grid->get_triangles();
    vector<Triangle*>::iterator it;
    
    double plantHeight = grid->plantHeight;
    
    for(it = v.begin(); it != v.end(); it++)
    {
        facet_area = (*it)->area;
        leafID = (*it)->leID;
        CLAI = (*it)-> CLAI;
        
        if(leafID <= 0)
        {
            continue;
        }
        
        int num = (int)((endHour - startHour) / hourInterval);
        
        for(int i = 0; i <= num; i++)
        {
            PPFD = ((*it)->photonFlux_up_dir[i] + (*it)->photonFlux_up_diff[i] + (*it)->photonFlux_up_scat[i] + (*it)->photonFlux_down_dir[i] + (*it)->photonFlux_down_diff[i] + (*it)->photonFlux_down_scat[i]) / (facet_area * 1e-4);
            
            cs.TAIR = cl.TAIR[i];
            
            ps.Parameters::prepare(leafID, CLAI, PPFD);
            
            leaf.run_FarquharModel_core(cs, ip, plantHeight);
            
            (*it)->PPFDSAT[i] = ip.PPFDsat;
            (*it)->isRubiscoLimit[i] = ip.isRubiscoLimit;
            (*it)->LEAFT[i] = ip.Tleaf;
            (*it)->GS[i] = ip.GS;
            (*it)->CI[i] = ip.CI;
            (*it)->A[i] = ip.A;
        }
    }
}
            

