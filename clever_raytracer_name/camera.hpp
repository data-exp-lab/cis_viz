#ifndef __CAMERA__
#define __CAMERA__

using namespace std;
using namespace raytrace;

//CHANGE FROM POS, DIR, RIGHT, AND DOWN TO EYE, LOOKAT, AND UP
class Camera
{
private:
    raytrace::Vect eye; //camera position
    raytrace::Vect lookAt; //camera direction
    raytrace::Vect up;
    float right;
    float left;
    float top;
    float bottom;
    float focalLength;
    
public:
    ////////
    raytrace::Vect position;
    raytrace::Vect direction;
    raytrace::Vect camera_right;
    raytrace::Vect camera_down;
    
    Camera() : right(0.0), left(0.0), top(0.0), bottom(0.0), focalLength(0.0){}
    Camera(const raytrace::Vect &e, const raytrace::Vect &la, const raytrace::Vect &u, const float &fl) : eye(e), lookAt(la), up(u), right(0.0), left(0.0), top(0.0), bottom(0.0), focalLength(fl){}
    Camera(raytrace::Vect pos, raytrace::Vect dir, raytrace::Vect right, raytrace::Vect down)
    {
        position = pos;
        direction = dir;
        camera_right = right;
        camera_down = down;
    }
    
    raytrace::Vect getCameraPosition()
    {
        return position;
    }
    
    raytrace::Vect getCameraDirection()
    {
        return direction;
    }
    
    raytrace::Vect getCameraRight()
    {
        return camera_right;
    }
    
    raytrace::Vect getCameraDown()
    {
        return camera_down;
    }

    raytrace::Vect getEye() const
    {
        return eye;
    }
    
    //CAMERA COORDINATES
    raytrace::Vect getW() const
    {
        return (eye - lookAt).normalize();
    }
    raytrace::Vect getU() const
    {
        return (up.crossProduct(getW())).normalize();
    }
    raytrace::Vect getV() const
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
    /*float setFocalLength() const
    {
        focalLength = f;
    }*/
};

#endif
