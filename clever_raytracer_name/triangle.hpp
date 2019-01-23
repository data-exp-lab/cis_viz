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
    
    /*setupTrianglesPLY(vector<Triangle*>& scene_triangles, int num_element_face, int num_vertices, int vertex1_index, int vertex2_index, int vertex3_index, Vect point1, Vect point2, Vect point3, float point1_x_coord, float point1_y_coord, float point1_z_coord, float point2_x_coord, float point2_y_coord, float point2_z_coord, float point3_x_coord, float point3_y_coord, float point3_z_coord, Color triangle_color, Color point1_color, Color point2_color, Color point3_color, double tri_red_average, double tri_green_average, double tri_blue_average, float point1_red, float point1_green, float point1_blue, float point2_red, float point2_green, float point2_blue, float point3_red, float point3_green, float point3_blue, vector<float>& num_vertices_to_connect_main, vector<float>& x_main, vector<float>& y_main, vector<float>& z_main, vector<float>& red_main, vector<float>& green_main, vector<float>& blue_main, vector<float>& vertex1_main, vector<float>& vertex2_main, vector<float>& vertex3_main)
    {
        for(int i = 0; i < num_element_face; i++)
        {
            cout << "TRIANGLE " << i << endl;
            debug("TRIANGLE %d", i);
            
            num_vertices = num_vertices_to_connect_main[i];
            if(num_vertices == 3)
            {
                vertex1_index = vertex1_main[i];
                vertex2_index = vertex2_main[i];
                vertex3_index = vertex3_main[i];
                cout << "vertex1_index: " << vertex1_index << endl;
                cout << "vertex2_index: " << vertex2_index << endl;
                cout << "vertex3_index: " << vertex3_index << endl;
                
                point1_x_coord = x_main[vertex1_index];
                point1_y_coord = y_main[vertex1_index];
                point1_z_coord = z_main[vertex1_index];
                cout << "x_main[vertex1_index]: " << x_main[vertex1_index] << endl;
                cout << "y_main[vertex1_index]: " << y_main[vertex1_index] << endl;
                cout << "z_main[vertex1_index]: " << z_main[vertex1_index] << endl;
                point1.setVectX(point1_x_coord);
                point1.setVectY(point1_y_coord);
                point1.setVectZ(point1_z_coord);
                cout << "point1.setVectX(): " << point1.getVectX() << endl;
                cout << "point1.setVectY(): " << point1.getVectY() << endl;
                cout << "point1.setVectZ(): " << point1.getVectZ() << endl;
                
                point2_x_coord = x_main[vertex2_index];
                point2_y_coord = y_main[vertex2_index];
                point2_z_coord = z_main[vertex2_index];
                cout << "x_main[vertex2_index]: " << x_main[vertex2_index] << endl;
                cout << "y_main[vertex2_index]: " << y_main[vertex2_index] << endl;
                cout << "z_main[vertex2_index]: " << z_main[vertex2_index] << endl;
                point2.setVectX(point2_x_coord);
                point2.setVectY(point2_y_coord);
                point2.setVectZ(point2_z_coord);
                cout << "point2.setVectX(): " << point2.getVectX() << endl;
                cout << "point2.setVectY(): " << point2.getVectY() << endl;
                cout << "point2.setVectZ(): " << point2.getVectZ() << endl;
                
                point3_x_coord = x_main[vertex3_index];
                point3_y_coord = y_main[vertex3_index];
                point3_z_coord = z_main[vertex3_index];
                cout << "x_main[vertex3_main[vertex3_index]]: " << x_main[vertex3_index] << endl;
                cout << "y_main[vertex3_main[vertex3_index]]: " << y_main[vertex3_index] << endl;
                cout << "z_main[vertex3_main[vertex3_index]]: " << z_main[vertex3_index] << endl;
                point3.setVectX(point3_x_coord);
                point3.setVectY(point3_y_coord);
                point3.setVectZ(point3_z_coord);
                cout << "point3.setVectX(): " << point3.getVectX() << endl;
                cout << "point3.setVectY(): " << point3.getVectY() << endl;
                cout << "point3.setVectZ(): " << point3.getVectZ() << endl;
                
                //IF R/G/B ARE OUT OF 255, CONVERT TO OUT OF 1
                if(red_main[vertex1_index] > 1.0)
                {
                    point1_red = red_main[vertex1_index] / 256;
                }
                else
                {
                    point1_red = red_main[vertex1_index];
                }
                
                if(green_main[vertex1_index] > 1.0)
                {
                    point1_green = green_main[vertex1_index] / 256;
                }
                else
                {
                    point1_green = green_main[vertex1_index];
                }
                
                if(blue_main[vertex1_index] > 1.0)
                {
                    point1_blue = blue_main[vertex1_index] / 256;
                }
                else
                {
                    point1_blue = blue_main[vertex1_index];
                }
                
                if(red_main[vertex2_index] > 1.0)
                {
                    point2_red = red_main[vertex2_index] / 256;
                }
                else
                {
                    point2_red = red_main[vertex2_index];
                }
                
                if(green_main[vertex2_index] > 1.0)
                {
                    point2_green = green_main[vertex2_index] / 256;
                }
                else
                {
                    point2_green = green_main[vertex2_index];
                }
                
                if(blue_main[vertex2_index] > 1.0)
                {
                    point2_blue = blue_main[vertex2_index] / 256;
                }
                else
                {
                    point2_blue = blue_main[vertex2_index];
                }
                
                if(red_main[vertex3_index] > 1.0)
                {
                    point3_red = red_main[vertex3_index] / 256;
                }
                else
                {
                    point3_red = red_main[vertex3_index];
                }
                
                if(green_main[vertex3_index] > 1.0)
                {
                    point3_green = green_main[vertex3_index] / 256;
                }
                else
                {
                    point3_green = green_main[vertex3_index];
                }
                
                if(blue_main[vertex3_index] > 1.0)
                {
                    point3_blue = blue_main[vertex3_index] / 256;
                }
                else
                {
                    point3_blue = blue_main[vertex3_index];
                }
                
                //COLOR
                point1_color.setColorRed(point1_red);
                point1_color.setColorGreen(point1_green);
                point1_color.setColorBlue(point1_blue);
                point2_color.setColorRed(point2_red);
                point2_color.setColorGreen(point2_green);
                point2_color.setColorBlue(point2_blue);
                point3_color.setColorRed(point3_red);
                point3_color.setColorGreen(point3_green);
                point3_color.setColorBlue(point3_blue);
                
                tri_red_average = (point1_color.getColorRed() + point2_color.getColorRed() + point3_color.getColorRed()) / 3;
                tri_green_average = (point1_color.getColorGreen() + point2_color.getColorGreen() + point3_color.getColorGreen()) / 3;
                tri_blue_average = (point1_color.getColorBlue() + point2_color.getColorBlue() + point3_color.getColorBlue()) / 3;
                
                triangle_color.setColorRed(tri_red_average);
                triangle_color.setColorGreen(tri_green_average);
                triangle_color.setColorBlue(tri_blue_average);
                
                scene_triangles.push_back(new Triangle(point1, point2, point3, triangle_color));
    }*/
    
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
    
    virtual double findIntersection(Ray ray)
    {
        Vect ray_direction = ray.getRayDirection();
        Vect ray_origin = ray.getRayOrigin();
        
        normal = getTriangleNormal();
        distance = getTriangleDistance(normal);

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
