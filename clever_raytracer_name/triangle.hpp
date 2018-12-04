#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "shape.hpp"
#include "vect.hpp"
#include "color.hpp"
#include "ray.hpp"

#include "climate.hpp"
#include "equations.hpp"
#include "normal.hpp"

#include <vector>

class Triangle : public Shape
{
public:
    Vect normal;
    
    Vect A;
    Vect B;
    Vect C;
    
    
    //FOR PPFD CALCULATIONS
    std::vector<double> photonFlux_up_dir;
    std::vector<double> photonFlux_up_diff;
    std::vector<double> photonFlux_up_scat;
    std::vector<double> photonFlux_down_dir;
    std::vector<double> photonFlux_down_diff;
    std::vector<double> photonFlux_down_scat;
    
    
    double distance;
    double area;
    Color color;
    
    vector<Triangle*> triangles;
    
    vector<double> LEAFT;
    //GS: STOMATAL CONDUCTANCE
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
    
    vector<double> a;
    
    Triangle();
    
    Triangle(Vect, Vect, Vect, Color);
    
    Triangle(double startHour, double endHour, double intervalHour);
    
    Triangle(const Vect& A, const Vect& B, const Vect& C, const int leafID, const double leafL, const double position, const double CLAI1, const double KT, double KR, const double nitrogenPerArea, double startHour, double endHour, double hourInterval);
    
    Triangle(const Vect A, const Vect B, const Vect C, const int leafID, const double leafL, const double position, const double CLAI1, const double KT, double KR, const double nitrogenPerArea, double startHour, double endHour, double hourInterval, Color color);
    
    void setPoint1(Vect point1)
    {
        A = point1;
    }
    
    void setPoint2(Vect point2)
    {
        B = point2;
    }
    
    void setPoint3(Vect point3)
    {
        C = point3;
    }
    
    float getPoint1X()
    {
        return A.getVectX();
    }
    float getPoint1Y()
    {
        return A.getVectY();
    }
    float getPoint1Z()
    {
        return A.getVectZ();
    }
    float getPoint2X()
    {
        return B.getVectX();
    }
    float getPoint2Y()
    {
        return B.getVectY();
    }
    float getPoint2Z()
    {
        return B.getVectZ();
    }
    float getPoint3X()
    {
        return C.getVectX();
    }
    float getPoint3Y()
    {
        return C.getVectY();
    }
    float getPoint3Z()
    {
        return C.getVectZ();
    }
    
    void add_triangle(Triangle* triangle_ptr)
    {
        triangles.push_back(triangle_ptr);
    }
    
    vector<Triangle*> get_triangles()
    {
        return triangles;
    }
    
    Vect getTriangleNormal()
    {
        Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
        Vect BA(B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());
        
        normal = CA.crossProduct(BA).normalize();
        
        return normal;
    }
    
    double getTriangleDistance()
    {
        normal = getTriangleNormal();
        distance = normal.dotProduct(A);
        
        return distance;
    }
    
    virtual Color getColor()
    {
        return color;
    }
    
    virtual Vect getNormalAt(Vect point)
    {
        normal = getTriangleNormal();
        
        return normal;
    }
    
    virtual double findIntersection(Ray ray)
    {
        Vect ray_direction = ray.getRayDirection();
        Vect ray_origin = ray.getRayOrigin();
        
        normal = getTriangleNormal();
        distance = getTriangleDistance();
        
        double a = ray_direction.dotProduct(normal);
        
        if(a == 0)
        {
            //RAY PARALLEL TO TRIANGLE
            return -1;
        }
        else
        {
            double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
            double distance_to_plane = -1 * b / a;
            
            double Qx = ray_direction.vectMult(distance_to_plane).getVectX() + ray_origin.getVectX();
            double Qy = ray_direction.vectMult(distance_to_plane).getVectY() + ray_origin.getVectY();
            double Qz = ray_direction.vectMult(distance_to_plane).getVectZ() + ray_origin.getVectZ();
            
            Vect Q(Qx, Qy, Qz);
            
            //Q = POINT OF INTERSECTION
            //[CA x QA] * n >= 0
            Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
            Vect QA(Q.getVectX() - A.getVectX(), Q.getVectY() - A.getVectY(), Q.getVectZ() - A.getVectZ());
            double test_CA = (CA.crossProduct(QA)).dotProduct(normal);
            
            //[BC x QC] * n >= 0
            Vect BC(B.getVectX() - C.getVectX(), B.getVectY() - C.getVectY(), B.getVectZ() - C.getVectZ());
            Vect QC(Q.getVectX() - C.getVectX(), Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
            double test_BC = (BC.crossProduct(QC)).dotProduct(normal);
            
            //[AB x QB] * n >= 0
            Vect AB(A.getVectX() - B.getVectX(), A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
            Vect QB(Q.getVectX() - B.getVectX(), Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
            double test_AB = (AB.crossProduct(QB)).dotProduct(normal);
            
            if((test_CA >= 0) && (test_BC >= 0) && (test_AB >= 0))
            {
                //INSIDE OF TRIANGLE
                return -1 * b / a;
            }
            else
            {
                //MISSED TRIANGLE
                return -1;
            }
        }
    }
};

Triangle::Triangle()
{
    A = Vect(1, 0, 0);
    B = Vect(0, 1, 0);
    C = Vect(0, 0, 1);
    color = Color(0.5, 0.5, 0.5, 0);
}

Triangle::Triangle(Vect pointA, Vect pointB, Vect pointC, Color colorValue)
{
    A = pointA;
    B = pointB;
    C = pointC;
    
    color = colorValue;
}

Triangle::Triangle(double startHour, double endHour, double hourInterval)
{
    //CALCUALTE NUMBER OF TIMESTEPS
    int num = (int)((endHour - startHour) / hourInterval);
    
    for(int i = 0; i <= num; i++)
    {
        photonFlux_up_dir.push_back(0.0);
        photonFlux_up_diff.push_back(0.0);
        photonFlux_up_scat.push_back(0.0);
        photonFlux_down_dir.push_back(0.0);
        photonFlux_down_diff.push_back(0.0);
        photonFlux_down_scat.push_back(0.0);
        
        a.push_back(0.0);
        GS.push_back(0.0);
        CI.push_back(0.0);
        LEAFT.push_back(0.0);
        PPFDSAT.push_back(0.0);
        isRubiscoLimit.push_back(0.0);
    }
}

Triangle::Triangle(const Vect& A, const Vect& B, const Vect& C, const int leafID, const double leafL, const double position, const double CLAI1, const double KT, double KR, const double nitrogenPerArea, double startHour, double endHour, double hourInterval)
{
    Vect v0 = A;
    Vect v1 = B;
    Vect v2 = C;
    
    //RIGHT HAND RULE
    normal = (v1 - v0) ^ (v2 - v0);
    normal.normalize();
    
    area = ((v1 -v0) ^ (v2 - v0)).length() * 0.5;
    int num = (int)((endHour - startHour) / hourInterval);
    
    for(int i = 0; i <= num; i++)
    {
        photonFlux_up_dir.push_back(0.0);
        photonFlux_up_diff.push_back(0.0);
        photonFlux_up_scat.push_back(0.0);
        photonFlux_down_dir.push_back(0.0);
        photonFlux_down_diff.push_back(0.0);
        photonFlux_down_scat.push_back(0.0);
        
        a.push_back(0.0);
        GS.push_back(0.0);
        CI.push_back(0.0);
        LEAFT.push_back(0.0);
        PPFDSAT.push_back(0.0);
        isRubiscoLimit.push_back(0.0);
    }
    
    nitrogenPerA = nitrogenPerArea;
    leID = leafID;
    leL = leafL;
    pos = position;
    CLAI = CLAI1;
    //WHAT GOES THROUGH THE LEAF
    kLeafTransmittance = KT;
    //WHAT REFLECTS OFF IT
    kLeafReflectance = KR;
    //ABSORBED LIGHT (?)
    //kA;
}

//INCLUDES COLOR
Triangle::Triangle(const Vect A, const Vect B, const Vect C, const int leafID, const double leafL, const double position, const double CLAI1, const double KT, double KR, const double nitrogenPerArea, double startHour, double endHour, double hourInterval, Color color)
{
    Vect v0 = A;
    Vect v1 = B;
    Vect v2 = C;
    Color tri_color = color;
    
    //RIGHT HAND RULE
    normal = (v1 - v0) ^ (v2 - v0);
    normal.normalize();
    
    area = ((v1 -v0) ^ (v2 - v0)).length() * 0.5;
    int num = (int)((endHour - startHour) / hourInterval);
    cout << "area: " << area << endl;
    
    for(int i = 0; i <= num; i++)
    {
        photonFlux_up_dir.push_back(0.0);
        photonFlux_up_diff.push_back(0.0);
        photonFlux_up_scat.push_back(0.0);
        photonFlux_down_dir.push_back(0.0);
        photonFlux_down_diff.push_back(0.0);
        photonFlux_down_scat.push_back(0.0);
        
        a.push_back(0.0);
        GS.push_back(0.0);
        CI.push_back(0.0);
        LEAFT.push_back(0.0);
        PPFDSAT.push_back(0.0);
        isRubiscoLimit.push_back(0.0);
    }
    
    nitrogenPerA = nitrogenPerArea;
    leID = leafID;
    leL = leafL;
    pos = position;
    CLAI = CLAI1;
    kLeafTransmittance = KT;
    kLeafReflectance = KR;
}


#endif
