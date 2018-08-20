#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "shape.hpp"
#include "vect.hpp"
#include "color.hpp"
#include "ray.hpp"

class Triangle : public Shape
{
    Vect A;
    Vect B;
    Vect C;
    
    Vect normal;
    
    double distance;
    Color color;
    
public:
    Triangle();
    
    Triangle(Vect, Vect, Vect, Color);
    
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






#endif
