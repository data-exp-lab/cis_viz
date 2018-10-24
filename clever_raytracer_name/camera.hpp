#ifndef __CAMERA__
#define __CAMERA__

#include "vect.hpp"

class Camera
{
    Vect camera_position;
    Vect camera_direction;
    Vect camera_right;
    Vect camera_down;
    
public:
    
    Camera();
    Camera(Vect, Vect, Vect, Vect);
    
    Vect getCameraPosition()
    {
        return camera_position;
    }
    
    Vect getCameraDirection()
    {
        return camera_direction;
    }
    
    Vect getCameraRight()
    {
        return camera_right;
    }
    
    Vect getCameraDown()
    {
        return camera_down;
    }
    
};

Camera::Camera()
{
    camera_position = Vect(0, 0, 0);
    camera_direction = Vect(0, 0, 1);
    camera_right = Vect(0, 0, 0);
    camera_down = Vect(0, 0, 0);
}

Camera::Camera(Vect pos, Vect dir, Vect right, Vect down)
{
    camera_position = pos;
    camera_direction = dir;
    camera_right = right;
    camera_down = down;
}





#endif
