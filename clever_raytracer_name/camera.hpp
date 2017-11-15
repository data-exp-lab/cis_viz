using namespace std;
using namespace raytrace;

class Camera
{
private:
    Vect eye;
    Vect lookAt;
    Vect up;
    float right;
    float left;
    float top;
    float bottom;
    float focalLength;
public:
    Camera() : right(0.0), left(0.0), top(0.0), bottom(0.0), focalLength(0.0){}
    Camera(const Vect &e, const Vect &la, const Vect &u, const float &fl) : eye(e), lookAt(la), up(u), right(0.0), left(0.0), top(0.0), bottom(0.0), focalLength(fl){}
    Vect getEye() const
    {
        return eye;
    }
    
    //CAMERA COORDINATES
    Vect getW() const
    {
        return (eye - lookAt).normalize();
    }
    Vect getU() const
    {
        return (up.crossProduct(getW())).normalize();
    }
    Vect getV() const
    {
        return getW().crossProduct(getU());
    }
    
    //GETTER AND SETTER METHODS
    float getRight() const
    {
        return right;
    }
    float getLeft() const
    {
        return left;
    }
    float getTop() const
    {
        return top;
    }
    float getBottom() const
    {
        return bottom;
    }
    float getFocalLength() const
    {
        return focalLength;
    }
    void setRight(const float &r)
    {
        right = r;
    }
    void setLeft(const float &l)
    {
        left = l;
    }
    void setTop(const float &t)
    {
        top = t;
    }
    void setBottom(const float &b)
    {
        bottom = b;
    }
};
