#ifndef __VECT__
#define __VECT__

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>

//template<typename T>
class Vect
{
public:
    double x;
    double y;
    double z;
    
    Vect ();
    
    Vect (double, double, double);
    
    //METHOD FUNCTIONS
    double getVectX()
    {
        return x;
    }
    
    double getVectY()
    {
        return y;
    }
    
    double getVectZ()
    {
        return z;
    }
    
    void setVectX(float input_x)
    {
        x = input_x;
    }
    
    void setVectY(float input_y)
    {
        y = input_y;
    }
    
    void setVectZ(float input_z)
    {
        z = input_z;
    }
    
    double magnitude()
    {
        return sqrt((x * x) + (y * y) + (z * z));
    }
    
    Vect inverse()
    {
        return (Vect(1 / x, 1 / y, 1 / z));
    }
    
    Vect normalize()
    {
        double magnitude = sqrt((x * x) + (y * y) + (z * z));
        return (Vect(x / magnitude, y / magnitude, z / magnitude));
        
        /*if(magnitude > 0)
        {
            double invMag = 1 / magnitude;
        }
        return(Vect(x *= invMag, y *= invMag, z *= invMag));*/
    }
    
    Vect negative()
    {
        return (Vect(-x, -y, -z));
    }
    
    double dotProduct(Vect v)
    {
        return (x * v.getVectX() + y * v.getVectY() + z * v.getVectZ());
    }
    
    Vect crossProduct(Vect v)
    {
        return (Vect(y * v.getVectZ() - z * v.getVectY(), z * v.getVectX() - x * v.getVectZ(), x * v.getVectY() - y * v.getVectX()));
    }
    
    Vect vectAdd(Vect v)
    {
        return (Vect(x + v.getVectX(), y + v.getVectY(), z + v.getVectZ()));
    }
    
    Vect vectMult (double scalar)
    {
        return (Vect(x * scalar, y * scalar, z * scalar));
    }
    
    Vect operator - (const Vect &n) const
    {
        return Vect(x - n.x, y - n.y, z - n.z);
    }
    
    Vect operator + (const Vect& v) const
    {
        return (Vect(x + v.x, y + v.y, z + v.z));
    }
    
    Vect operator = (const Vect& n)
    {
        x = n.x;
        y = n.y;
        z = n.z;
        
        return(*this);
    }
    
    Vect operator^(const Vect& p)
    {
        return(Vect(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x));
    }

    double length(void)
    {
        return (sqrt(x * x + y * y + z * z));
    }
    
    
};

Vect::Vect ()
{
    x = 0;
    y = 0;
    z = 0;
}

Vect::Vect (double i, double j, double k)
{
    x = i;
    y = j;
    z = k;
}


#endif
