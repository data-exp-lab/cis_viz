#ifndef __COLOR__
#define __COLOR__

#include "math.h"

class Color
{
    double red;
    double green;
    double blue;
    double alpha;
    
public:
    Color();
    Color(double, double, double, double);
    
    double getColorRed()
    {
        return red;
    }
    
    double getColorGreen()
    {
        return green;
    }
    
    double getColorBlue()
    {
        return blue;
    }
    
    double getColorAlpha()
    {
        return alpha;
    }
    
    double setColorRed(double redValue)
    {
        red = redValue;
    }
    
    double setColorGreen(double greenValue)
    {
        green = greenValue;
    }
    
    double setColorBlue(double blueValue)
    {
        blue = blueValue;
    }
    
    double setColorAlpha(double alphaValue)
    {
        alpha = alphaValue;
    }
    
    double brightness()
    {
        return(red + green + blue) / 3;
    }
    
    Color colorScalar(double scalar)
    {
        return Color((red * scalar), (green * scalar), (blue * scalar), alpha);
    }
    
    Color colorAdd(Color color)
    {
        return Color(red + color.getColorRed(), green + color.getColorGreen(), blue + color.getColorBlue(), alpha);
    }
    
    Color colorMultiply(Color color)
    {
        return Color((red * color.getColorRed()), (green * color.getColorGreen()), (blue * color.getColorBlue()), alpha);
    }
    
    Color colorAverage(Color color)
    {
        return Color((red + color.getColorRed())/ 2, (green + color.getColorGreen()) / 2, (blue + color.getColorBlue()) / 2, alpha);
    }
    
    Color clip()
    {
        double total_lights = red + green + blue;
        double extra_light = total_lights - 3;
        if(extra_light > 0)
        {
            red = red + extra_light * (red / total_lights);
            green = green + extra_light * (green / total_lights);
            blue = blue + extra_light * (blue / total_lights);
        }
        if(red > 1)
        {
            red = 1;
        }
        if(green > 1)
        {
            green = 1;
        }
        if(blue > 1)
        {
            blue = 1;
        }
        if(red < 0)
        {
            red = 0;
        }
        if(green < 0)
        {
            green = 0;
        }
        if(blue < 0)
        {
            blue = 0;
        }
        
        return Color(red, green, blue, alpha);

    }
};

Color::Color()
{
    red = 0.5;
    green = 0.5;
    blue = 0.5;
}

Color::Color(double r, double g, double b, double a)
{
    red = r;
    green = g;
    blue = b;
    alpha = a;
}





#endif
