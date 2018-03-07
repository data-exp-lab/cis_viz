using namespace std;

double max(double x0, double x1);

double max(double x0, double x1)
{
    return ((x0 > x1) ? x0 : x1);
}

double min(double x0, double x1);

double min(double x0, double x1)
{
    return ((x0 < x1) ? x0 : x1);
}

int clamp(double a, int b, int c);

int clamp(double a, int b, int c)
{
    return((int)max(b, min(a, c)));
}

float degToRad(float d)
{
    return d * 0.01745329;
}

float radToDeg(float d)
{
    return d * 57.2957795;
}
