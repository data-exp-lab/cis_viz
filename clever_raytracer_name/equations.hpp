#ifndef __EQUATIONS__
#define __EQUATIONS__

inline double max(double x0, double x1)
{
    return ((x0 > x1) ? x0 : x1);
}

inline double min(double x0, double x1)
{
    return ((x0 < x1) ? x0 : x1);
}

inline int clamp(double a, int b, int c)
{
    return ((int)max(b, min(a, c)));
}

inline float degToRad(float d)
{
    return (d * 0.01745329);
}

inline float radToDeg(float d)
{
    return (d * 57.2957795);
}

#endif
