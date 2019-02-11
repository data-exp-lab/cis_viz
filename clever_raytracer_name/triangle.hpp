#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "shape.hpp"
#include "vect.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "bbox.hpp"

#include "climate.hpp"
#include "equations.hpp"
#include "normal.hpp"
#include "debug.hpp"


#include <vector>

class Triangle : public Shape
{
public:
    Vect normal;
    
    Vect A;
    Vect B;
    Vect C;
    
    BBox bbox;
    
    const float kEpsilon = 1e-8;
    
    
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
    Triangle* test_triangle;
    
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
    
    static long long triangle_misses;
    static long long triangle_hits;
    static long long bbox_hits;
    static long long bbox_misses;
    
    Triangle();
    
    Triangle(Vect, Vect, Vect, Color);
    
    Triangle(double startHour, double endHour, double intervalHour);
    
    Triangle(const Vect& A, const Vect& B, const Vect& C, const int leafID, const double leafL, const double position, const double CLAI1, const double KT, double KR, const double nitrogenPerArea, double startHour, double endHour, double hourInterval);
    
    Triangle(const Vect A, const Vect B, const Vect C, const int leafID, const double leafL, const double position, const double CLAI1, const double KT, double KR, const double nitrogenPerArea, double startHour, double endHour, double hourInterval, Color color);
    
    BBox getBoundingBox(void);
    
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
    
    Vect getPoint1()
    {
        return A;
    }
    
    Vect getPoint2()
    {
        return B;
    }
    
    Vect getPoint3()
    {
        return C;
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
    
    double getTriangleDistance(Vect normal)
    {
        //normal = getTriangleNormal();
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
    
    Vect cross(const Vect& a, const Vect& b)
    {
        return Vect((a.y * b.z - a.z * b.y), (a.z * b.x - a.x * b.z), (a.x * b.y - a.y * b.x));
    }
    
    float dot(const Vect& va, const Vect& vb)
    {
        return va.x * vb.x + va.y * vb.y + va.z * vb.z;
    }
    
    virtual double findIntersection(Ray ray)
    {
        Vect ray_direction = ray.getRayDirection();
        Vect ray_origin = ray.getRayOrigin();

        //cout << "ray_direction: " << ray_direction.getVectX() << "  " << ray_direction.getVectY() << "  " << ray_direction.getVectZ() << endl;
        //cout << "ray_origin: " << ray_origin.getVectX() << "  " << ray_origin.getVectY() << "  " << ray_origin.getVectZ() << endl;
        
        //TEST FOR INTERSECTION WITH BBOX
        //if it doesn't intersect with bbox, terminate
        //else, continue
        float t;
        //BOUNDING BOX SPECIFIC TO EACH TRIANGLE
        BBox bbox_for_triangle = getBoundingBox();

        /*cout << "bbox_for_triangle minX: " << bbox_for_triangle.getBBoxXMin() << endl;
        cout << "bbox_for_triangle maxX: " << bbox_for_triangle.getBBoxXMax() << endl;
        cout << "bbox_for_triangle minY: " << bbox_for_triangle.getBBoxYMin() << endl;
        cout << "bbox_for_triangle maxY: " << bbox_for_triangle.getBBoxYMax() << endl;
        cout << "bbox_for_triangle minZ: " << bbox_for_triangle.getBBoxZMin() << endl;
        cout << "bbox_for_triangle maxZ: " << bbox_for_triangle.getBBoxZMax() << endl;*/
        
        if(bbox_for_triangle.intersectBBox(ray, bbox_for_triangle))
        //if(bbox_for_triangle.insideBBox(A) && bbox_for_triangle.insideBBox(B) && bbox_for_triangle.insideBBox(C))
        {
            //std::cout << "TRUE" << std::endl;
            this->bbox_hits++;
        }
        else
        {
            //std::cout << "FALSE" << std::endl;
            this->bbox_misses++;
            return false;
        }
        
        normal = getTriangleNormal();
        distance = getTriangleDistance(normal);
        double a = ray_direction.dotProduct(normal);
        
        if(a == 0)
        {
            //RAY PARALLEL TO TRIANGLE
            this->triangle_misses++;
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
            if(test_CA < 0)
            {
                this->triangle_misses++;
                return -1;
            }
            
            //[BC x QC] * n >= 0
            Vect BC(B.getVectX() - C.getVectX(), B.getVectY() - C.getVectY(), B.getVectZ() - C.getVectZ());
            Vect QC(Q.getVectX() - C.getVectX(), Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
            double test_BC = (BC.crossProduct(QC)).dotProduct(normal);
            if(test_BC < 0)
            {
                this->triangle_misses++;
                return -1;
            }
            
            //[AB x QB] * n >= 0
            Vect AB(A.getVectX() - B.getVectX(), A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
            Vect QB(Q.getVectX() - B.getVectX(), Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
            double test_AB = (AB.crossProduct(QB)).dotProduct(normal);
            if(test_AB < 0)
            {
                this->triangle_misses++;
                return -1;
            }
            
            //if((test_CA >= 0) && (test_BC >= 0) && (test_AB >= 0))
            //{
                //INSIDE OF TRIANGLE
                this->triangle_hits++;
                return distance_to_plane;
                //return -1 * b / a;
            //}
            /*else
            {
                //MISSED TRIANGLE
                this->misses++;
                return -1;
            }*/
        }
    }
  
    //BASED ON MOLLER-TRUMBOR METHOD
    //https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    //FIX: MAKE REFERENCES TO INFO SO NOT PASSING THINGS AROUND
    bool rayTriangleIntersect(Vect origin, Vect direction, Vect v0, Vect v1, Vect v2, float &t, float& u, float &v)
    {
        cout << "inside rayTriangleIntersect" << endl;
        Vect v0v1 = v1 - v0;
        Vect v0v2 = v2 - v0;
        Vect p_vect = cross(direction, v0v2);
        float determinant = dot(v0v1, p_vect);
        
        //IF CLOSE TO 0, RAY AND TRIANGLE ARE PARALLEL
        if(fabs(determinant) < kEpsilon)
        {
            return false;
        }
        
        float inv_determinant = 1 / determinant;
        
        Vect t_vect = origin - v0;
        u = dot(t_vect, p_vect) * inv_determinant;
        if(u < 0 || u > 1)
        {
            return false;
        }
        
        Vect q_vect = cross(t_vect, v0v1);
        v = dot(direction, q_vect) * inv_determinant;
        if(v < 0 || u + v > 1)
        {
            return false;
        }
        
        t = dot(v0v2, q_vect) * inv_determinant;
        
        return true;
    }
    
    //virtual bool intersect(Ray& ray, float &tNear, Triangle* test_triangles)
    bool intersect(Ray& ray, float &tNear, vector<Triangle*> triangles)
    {
        cout << "inside triangle intersect function" << endl;
        float t;
        float u;
        float v;
        int intersected_triangle_index;
        bool intersected = false;

        //cout << "triangles.size(): " << triangles.size() << endl;
        for(int i = 0; i < triangles.size(); ++i)
        //for(const auto& triangle: triangles)
        {
            //if(rayTriangleIntersect(ray.getRayOrigin(), ray.getRayDirection(), triangles[i]->getPoint1(), triangles[i]->getPoint2(), triangles[i]->getPoint3(), t, u, v) && t < tNear)
            if(rayTriangleIntersect(ray.getRayOrigin(), ray.getRayDirection(), triangles[i]->getPoint1(), triangles[i]->getPoint2(), triangles[i]->getPoint3(), t, u, v) && t < tNear)
            {
                tNear = t;
                intersected_triangle_index = i;
                //intersected_triangle_index = index;
                intersected = true;
            }
        }
        
        return intersected;
    }
    
    /*virtual bool intersect(Ray& ray, float &tNear)
    {
        float t;
        float u;
        float v;
        int intersected_triangle_index;
        bool intersected = false;
        
        //cout << "triangles.size(): " << triangles.size() << endl;
        for(int i = 0; i < triangles.size(); ++i)
        {
        if(rayTriangleIntersect(ray.getRayOrigin(), ray.getRayDirection(), triangles[i]->getPoint1(), triangles[i]->getPoint2(), triangles[i]->getPoint3(), t, u, v) && t < tNear)
        //if(rayTriangleIntersect(ray.getRayOrigin(), ray.getRayDirection(), test_triangles->getPoint1(), test_triangles->getPoint2(), test_triangles->getPoint3(), t, u, v) && t < tNear)
            {
                tNear = t;
                intersected_triangle_index = i;
                //intersected_triangle_index = index;
                intersected = true;
            }
        }
        
        return intersected;
    }*/
    
    
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

//MIGHT NEED TO PASS A, B, AND C AS PARAMETERS
BBox Triangle::getBoundingBox(void)
{
    double delta = 0.0001;
    //double delta = 0.5;
    /*cout << "A.x: " << A.x << endl;
    cout << "A.y: " << A.y << endl;
    cout << "A.z: " << A.z << endl;
    cout << "B.x: " << B.x << endl;
    cout << "B.y: " << B.y << endl;
    cout << "B.z: " << B.z << endl;
    cout << "C.x: " << C.x << endl;
    cout << "C.y: " << C.y << endl;
    cout << "C.z: " << C.z << endl;
    */
    return (BBox(min(min(A.x, B.x), C.x) - delta, max(max(A.x, B.x), C.x) + delta,
                 min(min(A.y, B.y), C.y) - delta, max(max(A.y, B.y), C.y) + delta,
                 min(min(A.z, B.z), C.z) - delta, max(max(A.z, B.z), C.z) + delta));
}
//FIX: CHECK IN GET_BBOX IN TRIANGLE FOR IF THE RAY STARTS INSIDE OF THE BBOX-->RETURN TRUE IF IT DOES



long long Triangle::triangle_misses = 0;
long long Triangle::triangle_hits = 0;
long long Triangle::bbox_hits = 0;
long long Triangle::bbox_misses = 0;


#endif
