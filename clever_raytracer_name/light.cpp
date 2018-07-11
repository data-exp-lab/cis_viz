#include "light.hpp"

Light::Light()
{
    position = raytrace::Vect(0, 0, 0);
    color = Color(1, 1, 1, 0);
}
Light::Light(raytrace::Vect p, Color c)
{
    position = p;
    color = c;
}


