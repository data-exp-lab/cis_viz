#include "triangle.hpp"
#include "equations.hpp"
#include <iostream>

Triangle::Triangle(double startHour, double endHour, double hourInterval)
{
    int num = (int)((endHour - starthour) / hourInterval);
    
    for(int i = 0; i <= num; i++)
    {
        photonFlux_up_dir.push_back(0.0);
        photonFlux_up_diff.push_back(0.0);
        photonFlux_up_scat.push_back(0.0);
        photonFlux_down_dir.push_back(0.0);
        photonFlux_down_diff.push_back(0.0);
        photonFlux_down_scat.push_back(0.0);
        A.push_back(0.0);
        GS.push_back(0.0);
        CI.push_back(0.0);
        LEAFT.push_back(0.0);
        PPFDSAT.push_back(0.0);
        isRubiscoLimit.push_back(0);
    }
}

Triangle::Triangle(const Point& a, const Point& b, const Point& c, const int leafID, const double leafL, const double position, const double CLAI1, const double KT, const double KR, const double nitrogenPerArea, double startHour, double endHour, double hourInterval)
{
    v0 = a;
    v1 = b;
    v2 = c;
    
    //RIGHT HAND RULE
    normal = (v1 - v0) ^ (v2 - v0);
    normal.normalize();
    
    area = ((v1 - v0) ^ (v2 - v0)).length() * 0.5;
    int num = (int)((endHour - startHour) / hourInterval);
    
    for(int i = 0; i <= num; i++)
    {
        photonFlux_up_dir.push_back(0.0);
        photonFlux_up_diff.push_back(0.0);
        photonFlux_up_scat.push_back(0.0);
        photonFlux_down_dir.push_back(0.0);
        photonFlux_down_diff.push_back(0.0);
        photonFlux_down_scat.push_back(0.0);
        A.push_back(0.0);
        GS.push_back(0.0);
        CI.push_back(0.0);
        LEAFT.push_back(0.0);
        PPFDSAT.push_back(0.0);
        isRubiscoLimit.push_back(0);
    }
    
    nitrogenPerA = nitrogenPerArea;
    leID = leafID;
    leL = leafL;
    pos = position;
    CLAI = CLAI1;
    kLeafTransmittance = KT;
    kLeafReflectance = KR;
}

Triangle::~Triangle()
{
    
}

void method1()
{
    
}

void Triangle::compute_normal(void)
{
    normal = (v1 - v0) ^ (v2 - v0);
    normal.normalize();
}

bool Triangle::hit(const Ray& ray, double& tmin)
{
    double a = v0.x - v1.x;
    double b = v0.x - v2.x;
    double c = ray.d.x;
    double d = v0.x - ray.o.x;
    double e = v0.y - v1.y;
    double f = v0.y - v2.y;
    double g = ray.d.y;
    double h = v0.y - ray.o.y;
    double i = v0.z - v1.z;
    double j = v0.z - v2.z;
    double k = ray.d.z;
    double l = v0.z - ray.o.z;
    double m = f * k - g * j;
    double n = h * k - g * l;
    double p = f * l - h * j;
    double q = g * i - e * k;
    double s = e * j - f * i;
    
    double inv_denom = 1.0 / (a * m + b * q + c * s);
    
    double e1 = d * m - b * n - c * p;
    double beta = e1 * inv_denom;
    
    if(beta < 0.0)
    {
        return false;
    }
    
    double r = e * l - h * i;
    double e2 = a * n + d * q + c * r;
    double gamma = e2 * inv_denom;
    
    if(gamma < 0.0)
    {
        return false;
    }
    if(beta + gamma > 1.0)
    {
        return false;
    }
    
    double e3 = a * p + b * r + d * s;
    double t = e3 * inv_denom;
    
    if(t < kEpsilon)
    {
        return false;
    }
    
    tmin = t;
    
    hit_point = ray.o + t * ray.d;
    
    return true;
}

