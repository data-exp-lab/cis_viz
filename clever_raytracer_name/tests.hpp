#ifndef __TEST__
#define __TEST__

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include "vect.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "light.hpp"
#include "source.hpp"
#include "shape.hpp"



using namespace std;

int testingClosestShapeIndex(vector<double> shape_intersections)
{
    //RETURN INDEX OF WINNING OBJECT INTERSECTION
    int index_of_minimum_intersection;
    
    if(shape_intersections.size() == 0)
    {
        //NO INTERSECTIONS
        return -1;
    }
    else if(shape_intersections.size() == 1)
    {
        //ONLY 1 INTERSECTION
        if(shape_intersections.at(0) > 0)
        {
            //INTERSECTION IS GREATER THAN 0, IS INDEX OF MINIMUM INTERSECTION
            //INDEX IN VECTOR ARRAY --> STARTING AT 0
            return 0;
        }
        else
        {
            //ONLY INTERSECTION VALUE IS NEGATIVE (RAY MISSED EVERYTHING)
            return -1;
        }
    }
    else
    {
        //MORE THAN ONE INTERSECTION
        //DETERMINE WHICH ONE IS CLOSEST TO CAMERA/HAS LEAST VALUE
        
        //FIND MAX VALUE IN VECTOR
        double max = 0;
        for(int i = 0; i < shape_intersections.size(); i++)
        {
            if(max < shape_intersections.at(i))
            {
                max = shape_intersections.at(i);
                debug("max: %d", max);
            }
        }
        
        //STARTING FROM MAX VALUE, FIND MIN POSITIVE VALUE
        if(max > 0)
        {
            //ONLY WANT POSITIVE INTERSECTIONS
            for(int index = 0; index < shape_intersections.size(); index++)
            {
                if(shape_intersections.at(index) > 0 && shape_intersections.at(index) <= max)
                {
                    max = shape_intersections.at(index);
                    index_of_minimum_intersection = index;
                    debug("max: %d", max);
                    debug("index_of_minimum_intersection: %d", index_of_minimum_intersection);
                }
            }
            return index_of_minimum_intersection;
        }
        else
        {
            //ALL INTERSECTIONS NEGATIVE
            return -1;
        }
    }
}


Color testGetColorAt(Vect intersection_ray_position, Vect intersection_ray_direction, vector<Shape*> scene_shapes, int index_of_closest_shape, vector<Source*> scene_lights, double accuracy, double ambient_light, bool shadowed)
{
    Color closest_shape_color = scene_shapes.at(index_of_closest_shape)->getColor();
    Vect closest_shape_normal = scene_shapes.at(index_of_closest_shape)->getNormalAt(intersection_ray_position);
    
    Color final_color = closest_shape_color.colorScalar(ambient_light);
    
    //REFLECTION
    //ALPHA [2]
    if(closest_shape_color.getColorAlpha() > 0 && closest_shape_color.getColorAlpha() <= 1)
    {
        //REFLECTION OFF OBJECTS WITH SPECTRAL VALUES
        double dot1 = closest_shape_normal.dotProduct(intersection_ray_direction.negative());
        Vect scalar1 = closest_shape_normal.vectMult(dot1);
        Vect add1 = scalar1.vectAdd(intersection_ray_direction);
        Vect scalar2 = add1.vectMult(2);
        Vect add2 = intersection_ray_direction.negative().vectAdd(scalar2);
        Vect reflection_direction = add2.normalize();
        
        Ray reflection_ray(intersection_ray_position, reflection_direction);
        
        //DETERMINE WHAT RAY INTERSECTS WITH FIRST
        vector<double> reflection_intersections;
        
        for(int reflection = 0; reflection < scene_shapes.size(); reflection++)
        {
            reflection_intersections.push_back(scene_shapes.at(reflection)->findIntersection(reflection_ray));
        }
        
        int index_of_closest_shape_with_reflection = testingClosestShapeIndex(reflection_intersections);
        
        if(index_of_closest_shape_with_reflection != -1)
        {
            //REFLECTION RAY DIDN'T INTERSECT WITH ANYTHING ELSE
            if(reflection_intersections.at(index_of_closest_shape_with_reflection) > accuracy)
            {
                Vect reflection_intersection_position = intersection_ray_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_closest_shape_with_reflection)));
                Vect reflection_intersection_ray_direction = reflection_direction;
                
                Color reflection_intersection_color = testGetColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_shapes, index_of_closest_shape_with_reflection, scene_lights, accuracy, ambient_light, shadowed);
                
                final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(closest_shape_color.getColorAlpha()));
            }
        }
    }
    
    for(int light = 0; light < scene_lights.size(); light++)
    {
        Vect light_direction = scene_lights.at(light)->getLightPosition().vectAdd(intersection_ray_position.negative()).normalize();
        
        float cosine_angle = closest_shape_normal.dotProduct(light_direction);
        
        if(cosine_angle > 0)
        {
            //TEST FOR SHADOWS IF ANGLE IS POSITIVE
            bool shadowed = false;
            
            Vect distance_to_light = scene_lights.at(light)->getLightPosition().vectAdd(intersection_ray_position.negative()).normalize();
            float distance_to_light_magnitude = distance_to_light.magnitude();
            
            //RAY HEADED BACK TOWARD LIGHT SOURCE TO DETERMINE WHETHER OR NOT THERE SHOULD BE A SHADOW
            Ray shadow_ray(intersection_ray_position, scene_lights.at(light)->getLightPosition().vectAdd(intersection_ray_position.negative()).normalize());
            
            vector<double> intersections_for_shadows;
            
            for(int o = 0; o < scene_shapes.size() && shadowed == false; o++)
            {
                intersections_for_shadows.push_back(scene_shapes.at(o)->findIntersection(shadow_ray));
            }
            
            for(int c = 0; c < intersections_for_shadows.size(); c++)
            {
                if(intersections_for_shadows.at(c) > accuracy)
                {
                    if(intersections_for_shadows.at(c) <= distance_to_light_magnitude)
                    {
                        shadowed = true;
                    }
                }
                break;
            }
            
            if(shadowed == false)
            {
                final_color = final_color.colorAdd(closest_shape_color.colorMultiply(scene_lights.at(light)->getLightColor()).colorScalar(cosine_angle));
                
                if(closest_shape_color.getColorAlpha() > 0 && closest_shape_color.getColorAlpha() <= 1)
                {
                    //ALPHA [0-1]
                    double dot1 = closest_shape_normal.dotProduct(intersection_ray_direction.negative());
                    
                    Vect scalar1 = closest_shape_normal.vectMult(dot1);
                    Vect add1 = scalar1.vectAdd(intersection_ray_direction);
                    Vect scalar2 = add1.vectMult(2);
                    Vect add2 = intersection_ray_direction.negative().vectAdd(scalar2);
                    Vect reflection_direction = add2.normalize();
                    
                    double specular = reflection_direction.dotProduct(light_direction);
                    
                    if(specular > 0)
                    {
                        specular = pow(specular, 10);
                        final_color = final_color.colorAdd(scene_lights.at(light)->getLightColor().colorScalar(specular * closest_shape_color.getColorAlpha()));
                    }
                    
                }
                
            }
            
        }
        
    }
    
    return final_color.clip();
}

bool testPLYFileType(bool PLY, string inputFileName, string fileType)
{
    
}

bool testNumberTriangles(int num_element_vertex, int size_of_x1_main)
{
    //see if the number of items in x, y, z, etc. equals the number of triangles taken out of the .ply file
    if(num_element_vertex == size_of_x1_main)
    {
        cout << "Test Number of Triangles (testNumberTriangles): PASSED" << endl;
        return true;
    }
    else
    {
        cout << "Test Number of Triangles (testNumberTriangles): FAILED" << endl;
        cout << "Number of elements from PLY file: " << num_element_vertex << endl;
        cout << "Number of elements passed to main function: " << size_of_x1_main << endl;
        return false;
    }
    
}

bool testNumberRays(int num_pixels, Color test_color, int width, int height, double accuracy, double ambient_light )
{
    //set camera location for testing
    //Test case 1:
        //fills the screen
        //if the number of rays equals the number of pixels based on resolution, return true
            //else return false
    int current_pixel;
    double aspect_ratio = (double)width / (double)height;
    
    //Increment if pixel contains shape and is hit by a ray
    int num_pixels_rays_hit = 0;
    
    vector<Shape*> scene_shapes;
    
    //Set camera and light to orthogonal position to be able to calculate number of pixels in a shape
    Vect test_camera_position(0, 0, 1);
    Vect origin(0, 0, 0);
    Vect lookAt(0, 0, 0);
    Vect Y(0, 1, 0);
    Vect difference_between(test_camera_position.getVectX() - lookAt.getVectX(), test_camera_position.getVectY() - lookAt.getVectY(), test_camera_position.getVectZ() - lookAt.getVectZ());
    
    Vect test_camera_direction = difference_between.negative().normalize();
    Vect camera_right = Y.crossProduct(test_camera_direction).normalize();
    Vect camera_down = camera_right.crossProduct(test_camera_direction);
    
    Camera test_scene_camera(test_camera_position, test_camera_direction, camera_right, camera_down);
    
    //Light information
    Vect test_light_position = test_camera_position;
    Light test_scene_light(test_light_position, test_color);
    
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&test_scene_light));
    
    //Set shape Z units away from camera/light -->cube that covers all of image
    Triangle test_triangle1(Vect(10, -10, 0), Vect(-10, 10, 0), Vect(-10, -10, 0), test_color);
    Triangle test_triangle2(Vect(-10, -10, 0), Vect(10, -10, 0), Vect(10, 10, 0), test_color);
    
    scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle1));
    scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle2));
    
    double x_amount;
    double y_amount;
    
    bool shadowed = false;
    
    //Number of rays that hit shape should be equal to number of pixels
    //Very similar to what loops over all pixels in main.cpp
    //Does not consider anti-aliasing
    //Does not consider shadowing
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            current_pixel = y * width + x;
            
            if(width > height)
            {
                //Image is wider than it is tall
                x_amount = ((x + 0.5) / width) * aspect_ratio - (((width - height) / (double)height) / 2);
                y_amount = ((height - y) + 0.5) / height;
            }
            else if(height > width)
            {
                //Image is taller than it is wide
                x_amount = (x + 0.5) / width;
                y_amount = (((height - y) + 0.5) / height) / aspect_ratio - (((height - width) / (double)width) / 2);
            }
            else
            {
                //Image is square
                x_amount = (x + 0.5) / width;
                y_amount = ((height - y) + 0.5) / height;
            }
            
            Vect test_camera_ray_origin = test_scene_camera.getCameraPosition();
            Vect test_camera_ray_direction = test_camera_direction.vectAdd(camera_right.vectMult(x_amount - 0.5).vectAdd(camera_down.vectMult(y_amount - 0.5))).normalize();
            
            Ray camera_ray(test_camera_ray_origin, test_camera_ray_direction);
            
            //Loop through objects in scene (in this specific instance, should be 1, find intersection with camera ray, push value into intersectiosn array
            vector<double> intersections;
            for(int index = 0; index < scene_shapes.size(); index++)
            {
                intersections.push_back(scene_shapes.at(index)->findIntersection(camera_ray));
            }
            
            int index_of_closest_shape = testingClosestShapeIndex(intersections);
            
            //If a pixel is set to -1, break because in this specific test every pixel should be colored/be an object
            if(index_of_closest_shape == -1)
            {
                cout << "Test #1 of Test Number Rays that Hit Object: FAILED" << endl;
                cout << "Test Number Rays that Hit Object: FAILED" << endl;
                cout << "Test #1: shape should take up every pixel" << endl;
                break;
            }
            else
            {
                if(intersections.at(index_of_closest_shape) > accuracy)
                {
                    Vect intersection_ray_position = test_camera_ray_origin.vectAdd(test_camera_ray_direction.vectMult(intersections.at(index_of_closest_shape)));
                    Vect intersection_ray_direction = test_camera_ray_direction;
                    
                    //Color current_color = scene_shapes.at(index_of_closest_shape)->getColor();
                    Color current_color = testGetColorAt(intersection_ray_position, intersection_ray_direction, scene_shapes, index_of_closest_shape, scene_lights, accuracy, ambient_light, shadowed);
                    
                    
                    num_pixels_rays_hit++;
                }

            }
            
        }
    }
    
    if(num_pixels_rays_hit == num_pixels)
    {
        cout << "Test Number Rays that Hit Object: PASSED" << endl;
        return true;
    }
    else
    {
        cout << "Test Number Rays that Hit Object: FAILED" << endl;
        cout << "Number of rays that should have hit the object: " << num_pixels << endl;
        cout << "Number of rays that hit the object: " << num_pixels_rays_hit << endl;
        return false;
    }
    
}

bool testNumberTimesteps(double start_hour, double end_hour, double interval_hour, int num_timesteps_performed)
{
    int num_timesteps_calculated = 0;
    
    num_timesteps_calculated = (int)((end_hour - start_hour) / interval_hour);
    
    if(num_timesteps_performed == num_timesteps_calculated)
    {
        cout << "Test Number of Timesteps: PASSED" << endl;
        return true;
    }
    else
    {
        cout << "Test Number of Timesteps: FAILED" << endl;
        cout << "Number of timesteps actually performed: " << num_timesteps_performed << endl;
        cout << "Number of timesteps that should be performed: " << num_timesteps_calculated << endl;
        return false;
    }
}

bool testRayDistance(int width, int height, Color test_red_light, Color test_white_light)
{
    int num_pixels = width * height;
    int current_pixel;
    
    //Set camera in orthogonal position
    Vect origin(0, 0, 0);
    Vect X(1, 0, 0);
    Vect Y(0, 1, 0);
    Vect Z(0, 0, 1);
    
    Vect test_camera_position(0, 0, -1);
    Vect lookAt(0, 0, 0);
    Vect difference_between(test_camera_position.getVectX() - lookAt.getVectX(), test_camera_position.getVectY() - lookAt.getVectY(), test_camera_position.getVectZ() - lookAt.getVectZ());
    
    Vect test_camera_direction = difference_between.negative().normalize();
    Vect test_camera_right = Y.crossProduct(test_camera_direction).normalize();
    Vect test_camera_down = test_camera_right.crossProduct(test_camera_direction);
    
    Camera test_scene_camera(test_camera_position, test_camera_direction, test_camera_right, test_camera_down);
    
    //Set light in orthogonal position (same place as camera)
    Vect test_light_position(0, 0, -1);
    Light test_scene_light(test_light_position, test_white_light);
    
    //Set shape Z units away from camera/light -->cube that covers all of image
    Triangle test_triangle1(Vect(1, -1, 0), Vect(-1, 1, 0), Vect(-1, -1, 0), test_red_light);
    Triangle test_triangle2(Vect(-1, -1, 0), Vect(1, -1, 0), Vect(1, 1, 0), test_red_light);
    
    //Calculate distance from camera/light to individual pixel of shape
    double x_squared;
    double y_squared;
    double z_squared;
    
    double square;
    double distance;
    
    //Set up ray to go from current_pixel to shape
    //Shoot ray for current_pixel to shape
    //Calculate distance that ray traveled
    
    //If distance from camera/light to shape is equal to distance that the ray traveled, return TRUE
    //Else, return FALSE
    
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            current_pixel = y * width + x;
            
            
            
        }
    }
    
}
/*
 Get width, height, number of pixels in image
 Set camera to orthogonal position
 Set shape to orthogonal position
 Determine distance from camera to shape
 
 Iterate over pixels:
     Determine current pixel
     Initialize ray to shoot to shape
     Shoot ray
     Calculate distance from ray to shape
 
 Check that distance from camera to shape is equal to distance from ray to shape
 */

bool testColorOfPixel()
{
    
}






#endif
