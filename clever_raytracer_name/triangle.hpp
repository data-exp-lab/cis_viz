#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "ray.hpp"
#include "point.hpp"
#include "constants.hpp"
#include "climate.hpp"
#include "normal.hpp"
#include "box.hpp"
#include <vector>


class Triangle
{
public:
    Point hit_point;
    Point v0;
    Point v1;
    Point v2;
    Normal normal;
    double area;
    
    vector<double> photonFlux_up_dir;
    vector<double> photonFlux_up_diff;
    vector<double> photonFlux_up_scat;
    vector<double> photonFlux_down_dir;
    vector<double> photonFlux_down_diff;
    vector<double> photonFlux_down_scat;
    
    vector<double> LEAFT;
    vector<double> GS;
    vector<double> CI;
    vector<double> PPFDSAT;
    
    vector<int> isRubiscoLimit;
    
    int leID;
    double CLAI;
    double leL;
    double pos;
    double chlSPA;
    double kLeafReflectance;
    double kLeafTransmittance;
    double nitrogenPerA;
    
    vector<double> A;
    
    Triangle(double startHour, double endHour, double hourInterval);
    Triangle(const Point& a, const Point& b, const Point& c, const int leafID, const double leafL, const double position, const double chlSPAD, const double KT, double KR, const double nitrogenPerArea, double startHour, double endHour, double hourInterval);
    virtual ~Triangle();
    
    bool hit(const raytrace::Ray& ray, double& tmin, Constants cs);
    void compute_normal(void);

    Box get_bounding_box(void);
    
    
};

#endif
