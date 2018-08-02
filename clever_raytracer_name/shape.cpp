#include "shape.hpp"

using namespace raytrace;

bool Sphere2::intersectShape(const raytrace::Ray &ray, float &t0, float &t1)
{
    raytrace::Vect L = ray.getOriginVect();
    raytrace::Vect D = ray.getDirection();
    
    raytrace::Vect tv = getPosition() - L;
    
    float v = tv.dotProduct(D);
    
    if(v < 0)
    {
        return false;
    }
    float d_squared = tv.dotProduct(tv) - (v * v);
    
    if(d_squared > radiusSquared)
    {
        return false;
    }
    
    float b= sqrt(radiusSquared - d_squared);
    
    t0 = v - b;
    t1 = v + b;
    
    return true;
}

/*bool Triangle3::intersectShape(const raytrace::Ray &ray, float &t0, float &t1)
{
    raytrace::Vect E1 = B - A;
    raytrace::Vect E2 = C - A;
    
    raytrace::Vect dir = ray.getDirection();
    raytrace::Vect ori = ray.getOriginVect();
    
    raytrace::Vect pVect = dir.crossProduct(E2);
    
    float det = E1.dotProduct(pVect);
    float invDet = 1 / det;
    
    if(det < 0.00000000001f)
    {
        return false;
    }
    
    raytrace::Vect tVect = ori - A;
    
    float u = tVect.dotProduct(pVect) * invDet;
    
    if(u < 0.0 || u > 1)
    {
        return false;
    }
    
    raytrace::Vect qVect = tVect.crossProduct(E1);
    
    float v = dir.dotProduct(qVect) * invDet;
    
    if(v < 0 || u + v > 1)
    {
        return false;
    }
    
    t0 = E2.dotProduct(qVect) * invDet;
    
    return true;
}*/

Triangle4::Triangle4()
{
    A = raytrace::Vect(1, 0, 0);
    B = raytrace::Vect(0, 1, 0);
    C = raytrace::Vect(0, 0, 1);
    
    color = Color(0.5, 0.5, 0.5, 0);
}

Triangle4::Triangle4(raytrace::Vect pointA, raytrace::Vect pointB, raytrace::Vect pointC, Color colorValue)
{
    A = pointA;
    B = pointB;
    C = pointC;
    
    color = colorValue;
}

raytrace::Vect Triangle4::getTriangleNormal()
{
    raytrace::Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
    raytrace::Vect BA(B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());
    normal = CA.crossProduct(BA).normalize();
    
    return normal;
}

double Triangle4::getTriangleDistance()
{
    normal = getTriangleNormal();
    distance = normal.dotProduct(A);
    
    return distance;
}

Color Triangle4::getColor()
{
    return color;
}

raytrace::Vect Triangle4::getNormalAt(raytrace::Vect point)
{
    normal = getTriangleNormal();
    return normal;
}

raytrace::Vect Triangle4::getSideA() const
{
    return A;
}

raytrace::Vect Triangle4::getSideB() const
{
    return B;
}

raytrace::Vect Triangle4::getSideC() const
{
    return C;
}

double Triangle4::findIntersection(raytrace::Ray ray)
{
    raytrace::Vect ray_direction = ray.getDirection();
    raytrace::Vect ray_origin = ray.getOriginVect();
    
    normal = getTriangleNormal();
    distance = getTriangleDistance();
    
    double a = ray_direction.dotProduct(normal);
    
    if(a == 0)
    {
        //RAY IS PARALLEL TO TRIANGLE
        return -1;
    }
    else
    {
        double b = normal.dotProduct(ray.getOriginVect().vectAdd(normal.vectMult(distance).negative()));
        double distance_to_plane = -1 * b / a;
        
        double Qx = ray_direction.vectMult(distance_to_plane).getVectX() + ray_origin.getVectX();
        double Qy = ray_direction.vectMult(distance_to_plane).getVectY() + ray_origin.getVectY();
        double Qz = ray_direction.vectMult(distance_to_plane).getVectZ() + ray_origin.getVectZ();
        
        //Q = POINT OF INTERSECTION
        raytrace::Vect Q(Qx, Qy, Qz);
        
        //[CA x QA] * n >= 0
        raytrace::Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
        raytrace::Vect QA(Q.getVectX() - A.getVectX(), Q.getVectY() - A.getVectY(), Q.getVectZ() - A.getVectZ());
        double test1 = (CA.crossProduct(QA)).dotProduct(normal);
        
        //[BC x QC] * n >= 0
        raytrace::Vect BA(B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());
        raytrace::Vect QC(Q.getVectX() - C.getVectX(), Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
        double test2 = (BA.crossProduct(QC)).dotProduct(normal);
        
        //[AB x QB] * n >= 0
        raytrace::Vect AB(A.getVectX() - B.getVectX(), A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
        raytrace::Vect QB(Q.getVectX() - B.getVectX(), Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
        double test3 = (AB.crossProduct(QB)).dotProduct(normal);
        
        if((test1 >= 0) && (test2 >= 0) && (test3 >= 0))
        {
            //INSIDE TRIANGLE
            return -1 * b / a;
        }
        else
        {
            //MISSED TRIANGLE
            return -1;
        }
    }
}









