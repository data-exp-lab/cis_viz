#ifndef __SOURCE__
#define __SOURCE__

class Source
{
public:
    Source();
    
    virtual Vect getLightPosition()
    {
        return Vect(0, 0, 0);
    }
    
    virtual Color getLightColor()
    {
        //RETURN WHITE LIGHT CURRENTLY
        return Color(1, 1, 1, 0);
    }
    
};

Source::Source()
{
    
}







#endif
