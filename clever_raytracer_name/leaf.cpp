#include "leaf.hpp"
#include "equations.hpp"
#include "farquharModel.hpp"
#include "thermal.hpp"
#include "stomata.hpp"
#include "energyBalance.hpp"
#include "ciModel.hpp"
#include <cmath>
#include <iostream>

double* Leaf::runCurves(Constants cs, Parameters ip, char X[], double x_start, double x_end, char Y[], char T[], double t[3])
{
    double *x_ptr = NULL;
    if(strcmp("CO2",X) == 0)
    {
        x_ptr = & ip.CA;
    }
    else if(strcmp(("CI",X) == 0)
    {
        x_ptr = & cs.CI;
    }
    else if(strcmp("PPFD",X) == 0)
    {
        x_ptr = cs.PPFD;
    }
    else if(strcmp("T",X) == 0)
    {
        x_ptr = & cs.TLEAF;
    }
    else
    {
        std::cout << "insufficient parameters for X axis" << std::endl;
    }
            
    double *t_ptr = NULL;
    if(strcmp("CO2",T) == 0)
    {
        t_ptr = & ip.CA;
    }
    else if(strcmp(("CI",T) == 0)
    {
        t_ptr = & cs.CI;
    }
    else if(strcmp("PPFD",T) == 0)
    {
        t_ptr = cs.PPFD;
    }
    else if(strcmp("T",T) == 0)
    {
        t_ptr = & cs.TLEAF;
    }
    else
    {
        std::cout << "insufficient parameters for T axis" << std::endl;
    }
    
    Leaf LPS = Leaf();
    double plantHeight = 100; //CM
    double interval = (x_end - x+start) / 49;
    
    //CHANGE TO BE DYNAMIC
    double *y_ptr = new double[150];
    int k = 0;
    
    for(int i = 0; i < 3; i++)
    {
        *t_ptr = t[i];
        for(int j = 0; j < 50; j++)
        {
            *x_ptr = x_start + j * interval;
        }
        
        LPS.run_FarquharModel_core(cs, ip, plantHeight);
        
        if(strcmp("A", Y) == 0)
        {
            y_ptr[k] = cs.A;
        }
        else if(strcmp("GS", Y) == 0)
        {
            y_ptr[k] = cs.GS;
        }
        else if(strcmp("TLEAF", Y) == 0)
        {
            y_ptr[k] = cs.TLEAF;
        }
        
        k++;
    }
    
    return y_ptr;
}
            
Leaf::Leaf()
{
    
}

Leaf::~Leaf()
{
    
}
            
Vect Leaf::get_reflect_dir(Vect L, Vect N)
{
    double E = 0;
    double FR = 0;
    Vect R;
    int xxx = 0;
    while(true)
    {
        xxx++;
        randReflectRayDir(N, R);
        FR = getfr(650, R, L, N);
        
        E = rand() * invRAND_MAX;
        
        if(E < FR)
        {
            break;
        }
    }
    
    return R;
}

void Leaf::get_reflect_dir_2(Ray ray, Triangle* triangle_ptr, vector<Ray*> &scatter_rays, double ignore_Photon_Flux_threshold)
{
    double E = 0;
    double FR1;
    double FR2;
    double FR3;
    double FR4;
    double FR5;
    double FRA;
    Vect R1;
    Vect R2;
    Vect R3;
    Vect R4;
    Vect R5;
    
    triangle_ptr->compute_normal();
    Vect normal_triangle(triangle_ptr->normal);
    
    if(normal_triangle * ray.d > 0)
    {
        normal_triangle = -normal_triangle;
    }
    
    //TOTAL REFLECT RAYS ENERGY
    double PF = ray.photonFlux2 * triangle_ptr->kLeafReflectance;
    
    randReflectRayDir(normal_triangle, R1);
    FR1 = getfr(650, R1, -ray.d, normal_triangle);
    randReflectRayDir(normal_triangle, R2);
    FR2 = getfr(650, 21, -ray.d, normal_triangle);
    randReflectRayDir(normal_triangle, R3);
    FR3 = getfr(650, R3, -ray.d, normal_triangle);
    randReflectRayDir(normal_triangle, R4);
    FR4 = getfr(650, R4, -ray.d, normal_triangle);
    randReflectRayDir(normal_triangle, R5);
    FR5 = getfr(650, R5, -ray.d, normal_triangle);
    
    FRA = FR1 + FR2 + FR3 + FR4 + FR5;
    
    if(PF > ignore_Photon_Flux_threshold)
    {
        scatter_rays.push_back(new Ray(triangle_ptr->hit_point, R1, PF * FR1 / FRA));
        scatter_rays.push_back(new Ray(triangle_ptr->hit_point, R2, PF * FR2 / FRA));
        scatter_rays.push_back(new Ray(triangle_ptr->hit_point, R3, PF * FR3 / FRA));
        scatter_rays.push_back(new Ray(triangle_ptr->hit_point, R4, PF * FR4 / FRA));
        scatter_rays.push_back(new Ray(triangle_ptr->hit_point, R5, PF * FR5 / FRA));
    }
}
            
Vect Leaf::get_transmit_dir(Vect L, Vect N)
{
    Vect t;
    randThroughRayDir(N, t);
    
    return t;
}

void Leaf::randReflectRayDir(Vect N, Vect & r)
{
    while(true)
    {
        double x = rand() * invRAND_MAX * 2 - 1;
        double y = rand() * invRAND_MAX * 2 - 1;
        double z = rand() * invRAND_MAX * 2 - 1;
        
        if(x * x + y * y + z * z > 1)
        {
            continue;
        }
        if(x * x.N + y * N.y + z * N.z <= 0)
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
            
void Leaf::randThroughRayDir(Vect N, Vect & r)
{
    while(true)
    {
        double x = rand() * invRAND_MAX * 2 - 1;
        double y = rand() * invRAND_MAX * 2 - 1;
        double z = rand() * invRAND_MAX * 2 - 1;
        
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
            
double Leaf::getfr(double hv_wave_length, Vect V, Vect L, Vect N)
{
    double S = BRFD_S;
    double F0 = BRDF_F0;
    double RD = BRDF_RD;
    double M = BRDF_M;
    
    Vect N = vMidLine(L, V);
    
    double G1 = 2 * (N * H) * (N * V) / (V * H);
    double G2 = 2 * (N * H) * (N * L) / (V * H);
    double G = min(1, min(G1, G2));
    
    double A = vAngle(N, H);
    double D = 1 / (M * M * pow(cos(A), 4)) * exp(-pow(tan(A) / M, 2));
    double A2 = vAngle(N, L);
    double F = F0 + (1 - F0) * pow(1 - cos(A2), 5);
    
    double FR = S * F / PI * D * G / ((N * L) * (N * V)) + (1 - S) * RD;
    
    return FR;
}
            
double Leaf::vMidLine(Vect A, Vect B)
{
    Vect C = A + B;
    C.normalize();
    
    return C;
}
            
double Leaf::vAngle(Vect A, Vect B)
{
    double angle = acos((A * B) / (A.length() * B.length()));
    
    return angle;
}
            
void Leaf::run_FarquharModel(double startHour, double endHour, double hourInterval, Parameters &ip, Climate &cl)
{
    Parameters ps;
    double facet_area;
    int leafID;
    double PPFD;
    double CLAI;
    
    vector<Triangle*> v = grid->get_triangles();
    vector<Triangle*>::iterator it;
    
    //FIX
    double plantHeight = 1;
    
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
            
            ip.TAIR = cl.TAIR[i];
            
            cs.prepare(ip, leafID, CLAI, PPFD);
            
            run_FarquharModel_core(cs, ip, plantHeight);
            
            (*it)->PPFDSAT[i] = cs.PPFDSAT;
            (*it)->isRubiscoLimit[i] = cs.isRubiscoLimit;
            (*it)->leafT[i] = cs.TLEAF;
            (*it)->GS[i] = cs.GS;
            (*it)->CI[i] = cs.CI;
            (*it)->A[i] = cs.A;
        }
    }
}
            
void Leaf::run_FarquharModel_core(Constants &cs, Parameters &ip, double plantHeight)
{
    FarquharModel fm;
    Thermal thm;
    Stomata sm;
    EnergyBalance eb;
    CiModel cim;
    
    cs = thm.temperatureAdj(cs);
    
    //PRE-RUN PHOTOSYNTHESIS
    fm.calculation(cs);
    
    cs = sm.emperical_model_1(cs, ip);
    cs = cim.calculateCi(cs, ip);
    cs = sm.const_gs(cs, ip);
    
    fm.calculation(cs);
    
    double DT = 100;
    double OLDT = 0;
    int N_LOOP = 0;
    
    while(abs(DT) > 0.02 && N_LOOP < 20)
    {
        OLDT = cs.TLEAF;
        cs = thm.temperatureAdj(cs);
        
        cs = sm.emperical_model_1(cs, ip);
        cs.cim.calculateCi(cs, ip);
        fm.calculation(cs);
        
        cs = eb.calculateLeafT_withWindSpeed(cs, ip, plantHeight);
        DT = cs.TLEAF - OLDT;
        
        N_LOOP++
    }
}
