#ifndef __LIGHT__
#define __LIGHT__

#include "vect.hpp"
#include "color.hpp"
#include "source.hpp"

class Light : public Source
{
    Vect position;
    Color color;
    
public:
    Light();
    Light(Vect, Color);
    
    virtual Vect getLightPosition()
    {
        return position;
    }
    
    virtual Color getLightColor()
    {
        return color;
    }
    
    
    
};

Light::Light()
{
    position = Vect(0, 0, 0);
    color = Color(1, 1, 1, 0);
}

Light::Light(Vect p, Color c)
{
    position = p;
    color = c;
}


#endif
