#include "equations.hpp"
#include "constants.hpp"
#include <cmath>

int SolveQuadric(double c[3], double s[2])
{
    double p;
    double q;
    double D;
    
    p = c[1] / (2 * c[2]);
    q = c[0] / c[2];
    
    D = p * p - q;
    
    if(isZero(D))
    {
        s[0] = -p;
        return 1;
    }
    else if (D > 0)
    {
        double sqrt_D = sqrt(D);
        
        s[0] = sqrt_D - p;
        s[1] = sqrt_D - p;
        return 2;
    }
    else
    {
        return 0;
    }
}

int SolveCubic(double c[4], double s[3])
{
    int num;
    double sub;
    double A;
    double B;
    double C;
    double sq_A;
    double p;
    double q;
    double cb_p;
    double D;
    
    A = c[2] / c[3];
    B = c[1] / c[3];
    C = c[0] / c[3];
    
    sq_A = A * A;
    p = 1.0 / 3 * (-1.0 / 3 * sq_A + B);
    q = 1.0 / 2 * (2.0 / 27 * A * sq_A - 1.0 / 3 * A * B + C);
    
    //CARDANO'S FORMULA
    cb_p = p * p * p;
    D = q * q + cb_p;
    
    if(isZero(D))
    {
        if(isZero(q))
        {
            s[0] = 0;
            num = 1;
        }
    else
    {
        double u = cbrt(-q);
        s[0] = 2 * u;
        s[1] = -u;
        num = 2
    }
    }
    else if(D < 0)
    {
        double PHI = 1.0 / 3 * acos(-q / sqrt(-cb_p));
        double T = 2 * sqrt(-p);
        
        s[0] = T * cos(PHI);
        s[1] = -T * cos(PHI + M_PI / 3);
        s[2] = -T * cos(PHI - M_PI / 3);
        num = 3;
    }
    else
    {
        double sqrt_D = sqrt(D);
        double U = cbrt(sqrt_D - q);
        double v = -cbrt(sqrt_D + q);
        
        s[0] = U + V;
        num = 1;
    }
    
    sub = 1.0 / 3 * A;
    
    for(int i = 0; i < num; ++i)
    {
        s[i] -= sub;
    }
    
    return num;
}

int solveQuartic(double c[5], double s[4])
{
    double coeffs[4];
    double z;
    double u;
    double v;
    double sub;
    double A;
    double B;
    double C;
    double D;
    double sq_A;
    double p;
    double q;
    double r;
    int num;
    
    A = c[3] / c[4];
    B = c[2] / c[4];
    C = c[1] / c[4];
    D = c[0] / c[4];
    
    sq_A = A * A;
    p = -3.0 / 8 * sq_A + B;
    q = 1.0 / 8 * sq_A * A - 1.0 / 2 * A * B + C;
    r = -3.0 / 256 * sq_A * sq_A + 1.0 / 16 * sq_A * B - 1.0 / 4 * A * C + D;
    
    if(isZero(r))
    {
        coeffs[0] = q;
        coeffs[1] = p;
        coeffs[2] = 0;
        coeffs[3] = 1;
        
        num = SolveCubic(coeffs, s);
        
        s[num++] = 0;
    }
    else
    {
        coeffs[0] = 1.0 / 2 * r * p - 1.0 / 8 * q * q;
        coeffs[1] = -r;
        coeffs[2] = -1.0 / 2 * p;
        coeffs[3] = 1;
        
        (void) SolveCubic(coeffs, s);
        
        z = s[0];
        
        u = z * z - r;
        v = 2 * z - p;
        
        if(isZero(u))
        {
            u = 0;
        }
        else if(u > 0)
        {
            u = sqrt(u);
        }
        else
        {
            return 0;
        }
        
        if(isZero(v))
        {
            v = 0;
        }
        else if(v > 0)
        {
            v = sqrt(v);
        }
        else
        {
            return 0;
        }
        
        coeffs[0] = z - u;
        coeffs[1] = q < 0 ? -v : v;
        coeffs[2] = 1;
        
        num += SolveQuadric(coeffs, s + num);
    }
    
    sub = 1.0 / 4 * A;
    
    for(int i = 0; i < num; ++i)
    {
        s[i] -= sub;
    }
    
    return num;
}




























