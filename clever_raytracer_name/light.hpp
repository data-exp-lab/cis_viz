#include "ray.hpp"
#include "color.hpp"
#include "source.hpp"


class Light : public Source
{
    raytrace::Vect position;
    Color color;
    
public:
    Light();
    Light(raytrace::Vect, Color);
    
    virtual raytrace::Vect getLightPosition()
    {
        return position;
    }
    virtual Color getLightColor()
    {
        return color;
    }

    
};

