#ifndef __TEST__
#define __TEST__

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "vect.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "light.hpp"
#include "source.hpp"
#include "shape.hpp"

#include "bbox.hpp"

#include "timing.hpp"



using namespace std;

#ifdef RUN_TESTS

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

/*
 * Beginning of actual tests here
 */

bool testPLYFileType(bool PLY, string inputFileName, string fileType)
{
    
}

bool testNumberTriangles(int num_element_vertex, int size_of_x1_main)
{
    //see if the number of items in x, y, z, etc. equals the number of triangles taken out of the .ply file
    cout << "RUNNING TEST #1: testNumberTriangles" << endl;
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
    //TIMING INFORMATION
    initTimer(1, 18000000, 100);
    startTimer( testNumberRays_Timer );
    clock_t total_time_begin;
    clock_t total_time_end;
    total_time_begin = clock();
    
    cout << "RUNNING TEST #2: testNumberRays" << endl;
    //set camera location for testing
    //Test case 1:
        //fills the screen
        //if the number of rays equals the number of pixels based on resolution, return true
            //else return false
    int current_pixel;
    double aspect_ratio = (double)width / (double)height;
    
    //Increment if pixel contains shape and is hit by a ray
    int num_pixels_rays_hit = 0;
    int num_pixels_total = width * height;
    
    vector<Shape*> scene_shapes;
    
    //Set camera and light to orthogonal position to be able to calculate number of pixels in a shape
    Vect test_camera_position(0, 0, -1);
    Vect origin(0, 0, 0);
    Vect lookAt(0, 0, 0);
    Vect Y(0, 1, 0);
    Vect difference_between(test_camera_position.getVectX() - lookAt.getVectX(), test_camera_position.getVectY() - lookAt.getVectY(), test_camera_position.getVectZ() - lookAt.getVectZ());
    
    Vect test_camera_direction = difference_between.negative().normalize();
    Vect camera_right = Y.crossProduct(test_camera_direction).normalize();
    Vect camera_down = camera_right.crossProduct(test_camera_direction);
    
    Camera test_scene_camera(test_camera_position, test_camera_direction, camera_right, camera_down);
    
    //Light information
    Vect test_light_position(-200000000, 1000, -20000000); //= test_camera_position;
    Light test_scene_light(test_light_position, test_color);
    
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&test_scene_light));
    
    //Set shape Z units away from camera/light -->cube that covers all of image
    Triangle test_triangle1(Vect(1000000000, -100000000, 0), Vect(-1000000000, 1000000000, 0), Vect(-1000000000, -10000000000, 0), test_color);
    //Triangle test_triangle2(Vect(-10, -10, 0), Vect(10, -10, 0), Vect(10, 10, 0), test_color);
    
    scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle1));
    //scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle2));
    
    double x_amount;
    double y_amount;
    
    bool shadowed = false;
    double tempRed;
    double tempGreen;
    double tempBlue;
    
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
    
    endTimer(0, testNumberRays_Timer, 10, "testNumberRays");
    total_time_end = clock();
    
    float total_time = ((float)total_time_end - (float)total_time_begin) / 1000000;
    cout << "testNumberRays() TOTAL TIME: " << total_time << " seconds" << endl;
    
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

struct test_RGB_Type
{
    double r;
    double g;
    double b;
    
};

bool testNumberRays_antiAliasing(int num_pixels, Color test_color, int width, int height, double accuracy, double ambient_light, int anti_aliasing_depth)
{
    //TIMING INFORMATION
    initTimer(1, 18000000, 100);
    startTimer( testNumberRays_antiAliasing_Timer );
    clock_t total_time_begin;
    clock_t total_time_end;
    total_time_begin = clock();
    
    cout << "RUNNING TEST #3: testNumberRays_antiAliasing" << endl;
    cout << "ANTI ALIASING FACTOR: " << anti_aliasing_depth << endl;
    //TEST 1: Do 1 triangles like testNumberRays() to see computation time increase
    //TEST 2: Do with two triangles together to see if anti-aliasing solves the problem of partial rays
    test_RGB_Type *pixels = new test_RGB_Type[num_pixels];
    int current_pixel;
    double aspect_ratio = (double)width / (double)height;
    
    //Increment if pixel contains shape and is hit by a ray
    int num_pixels_rays_hit = 0;
    
    int num_pixels_total = width * height;
    
    vector<Shape*> scene_shapes;
    
    //Set camera and light to orthogonal position to be able to calculate number of pixels in a shape
    Vect test_camera_position(0, 0, -1);
    Vect origin(0, 0, 0);
    Vect lookAt(0, 0, 0);
    Vect Y(0, 1, 0);
    Vect difference_between(test_camera_position.getVectX() - lookAt.getVectX(), test_camera_position.getVectY() - lookAt.getVectY(), test_camera_position.getVectZ() - lookAt.getVectZ());
    
    Vect test_camera_direction = difference_between.negative().normalize();
    Vect camera_right = Y.crossProduct(test_camera_direction).normalize();
    Vect camera_down = camera_right.crossProduct(test_camera_direction);
    
    Camera test_scene_camera(test_camera_position, test_camera_direction, camera_right, camera_down);
    
    //Light information
    Vect test_light_position(-200000000, 1000, -20000000); //= test_camera_position;
    Light test_scene_light(test_light_position, test_color);
    
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&test_scene_light));
    
    //Set shape Z units away from camera/light -->cube that covers all of image
    Triangle test_triangle1(Vect(1000000000, -100000000, 0), Vect(-1000000000, 1000000000, 0), Vect(-1000000000, -10000000000, 0), test_color);
    //Triangle test_triangle2(Vect(-10, -10, 0), Vect(10, -10, 0), Vect(10, 10, 0), test_color);
    
    scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle1));
    //scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle2));
    
    double x_amount;
    double y_amount;
    
    bool shadowed = false;
    double tempRed;
    double tempGreen;
    double tempBlue;
    int anti_aliasing_index;
    
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            current_pixel = y * width + x;
            
            //STARTING WITH A BLANK PIXEL
            double tempRed[anti_aliasing_depth * anti_aliasing_depth];
            double tempGreen[anti_aliasing_depth * anti_aliasing_depth];
            double tempBlue[anti_aliasing_depth * anti_aliasing_depth];
            
            for(int anti_aliasing_x = 0; anti_aliasing_x < anti_aliasing_depth; anti_aliasing_x++)
            {
                for(int anti_aliasing_y = 0; anti_aliasing_y < anti_aliasing_depth; anti_aliasing_y++)
                {
                    anti_aliasing_index = anti_aliasing_y * anti_aliasing_depth + anti_aliasing_x;
                    
                    //CREATE RAY FROM CAMERA TO CURRENT PIXEL
                    if(anti_aliasing_depth == 1)
                    {
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
                    }
                    else
                    {
                        //ANTI-ALIASING
                        if(width > height)
                        {
                            //IMAGE IS WIDER THAN IT IS TALL
                            x_amount = ((x + (double)anti_aliasing_x / ((double) anti_aliasing_depth - 1)) / width) * aspect_ratio - (((width - height) / (double)height) / 2);
                            y_amount = ((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height;
                        }
                        else if(height > width)
                        {
                            //IMAGE IS TALLER THAN IT IS WIDE
                            x_amount = (x + (double)anti_aliasing_x / ((double) anti_aliasing_depth - 1)) / width;
                            y_amount = (((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height) / aspect_ratio - (((height - width) / (double)width) / 2);
                        }
                        else
                        {
                            //IMAGE IS SQUARE
                            x_amount = (x + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / width;
                            y_amount = ((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height;
                        }
                    }
                    
                    Vect test_camera_ray_origin = test_scene_camera.getCameraPosition();
                    Vect test_camera_ray_direction = test_camera_direction.vectAdd(camera_right.vectMult(x_amount - 0.5).vectAdd(camera_down.vectMult(y_amount - 0.5))).normalize();
                    
                    Ray camera_ray(test_camera_ray_origin, test_camera_ray_direction);
                    
                    vector<double> intersections;
                    
                    //LOOP THROUGH OBJECTS IN SCENE, FIND INTERSECTION WITH CAMERA RAY, PUSH VALUE INTO INTERSECTIONS ARRAY
                    for(int index = 0; index < scene_shapes.size(); index++)
                    {
                        intersections.push_back(scene_shapes.at(index)->findIntersection(camera_ray));
                    }
                    
                    int index_of_closest_shape = testingClosestShapeIndex(intersections);
                    
                    if(index_of_closest_shape == -1)
                    {
                        cout << "Test #2 of Test Number Rays that Hit Object: FAILED" << endl;
                        cout << "Test Number Rays that Hit Object: FAILED" << endl;
                        cout << "Test #2: shape should take up every pixel" << endl;
                        break;
                    }
                    else
                    {
                        //INDEX CORRESPONDS TO SHAPE IN SCENE
                        if(intersections.at(index_of_closest_shape) > accuracy)
                        {
                            //DETERMINE POSITION AND DIRECTION VECTORS AT POINT OF INTERSECTION
                            Vect intersection_ray_position = test_camera_ray_origin.vectAdd(test_camera_ray_direction.vectMult(intersections.at(index_of_closest_shape)));
                            
                            Vect intersection_ray_direction = test_camera_ray_direction;
                            
                            Color current_color = testGetColorAt(intersection_ray_position, intersection_ray_direction, scene_shapes, index_of_closest_shape, scene_lights, accuracy, ambient_light, shadowed);//scene_shapes.at(index_of_closest_shape)->getColor();
                            
                            tempRed[anti_aliasing_index] = current_color.getColorRed();
                            tempGreen[anti_aliasing_index] = current_color.getColorGreen();
                            tempBlue[anti_aliasing_index] = current_color.getColorBlue();
                            
                            
                            }
                        }
                    }
                }
                
                //AVERAGE PIXEL COLORS
                double totalRed = 0;
                double totalGreen = 0;
                double totalBlue = 0;
                
                for(int iRed = 0; iRed < anti_aliasing_depth * anti_aliasing_depth; iRed++)
                {
                    totalRed = totalRed + tempRed[iRed];
                }
                for(int iGreen = 0; iGreen < anti_aliasing_depth * anti_aliasing_depth; iGreen++)
                {
                    totalGreen = totalGreen + tempGreen[iGreen];
                }
                for(int iBlue = 0; iBlue < anti_aliasing_depth * anti_aliasing_depth; iBlue++)
                {
                    totalBlue = totalBlue + tempBlue[iBlue];
                }
                
                double avgRed = totalRed / (anti_aliasing_depth * anti_aliasing_depth);
                double avgGreen = totalGreen / (anti_aliasing_depth * anti_aliasing_depth);
                double avgBlue = totalBlue / (anti_aliasing_depth * anti_aliasing_depth);
                
                pixels[current_pixel].r = avgRed;
                pixels[current_pixel].g = avgGreen;
                pixels[current_pixel].b = avgBlue;
            
                num_pixels_rays_hit++;
            }
        }
   // }
    
    endTimer(0, testNumberRays_antiAliasing_Timer, 11, "testNumberRays_antiAliasing");
    total_time_end = clock();
    
    float total_time = ((float)total_time_end - (float)total_time_begin) / 1000000;
    cout << "testNumberRays_antiAliasing() TOTAL TIME: " << total_time << " seconds" << endl;
    
    if(num_pixels_rays_hit == num_pixels)
    {
        cout << "Test Number Rays that Hit Object with Anti Aliasing: PASSED" << endl;
        return true;
    }
    else
    {
        cout << "Test Number Rays that Hit Object with Anti Aliasing: FAILED" << endl;
        cout << "Number of rays that should have hit the object: " << num_pixels << endl;
        cout << "Number of rays that hit the object: " << num_pixels_rays_hit << endl;
        return false;
    }
}

bool testNumberRays_antiAliasing_cube(int num_pixels, Color test_color, int width, int height, double accuracy, double ambient_light, int anti_aliasing_depth)
{
    //TIMING INFORMATION
    initTimer(1, 18000000, 100);
    startTimer( testNumberRays_antiAliasing_cube_Timer );
    clock_t total_time_begin;
    clock_t total_time_end;
    total_time_begin = clock();
    
    cout << "RUNNING TEST #7: testNumberRays_antiAliasing_cube" << endl;
    cout << "ANTI ALIASING FACTOR: " << anti_aliasing_depth << endl;
    
    test_RGB_Type *pixels = new test_RGB_Type[num_pixels];
    int current_pixel;
    double aspect_ratio = (double)width / (double)height;
    
    //Increment if pixel contains shape and is hit by a ray
    int num_pixels_rays_hit = 0;
    
    int num_pixels_total = width * height;
    
    vector<Shape*> scene_shapes;
    
    //Set camera and light to orthogonal position to be able to calculate number of pixels in a shape
    Vect test_camera_position(0, 0, -1);
    Vect origin(0, 0, 0);
    Vect lookAt(0, 0, 0);
    Vect Y(0, 1, 0);
    Vect difference_between(test_camera_position.getVectX() - lookAt.getVectX(), test_camera_position.getVectY() - lookAt.getVectY(), test_camera_position.getVectZ() - lookAt.getVectZ());
    
    Vect test_camera_direction = difference_between.negative().normalize();
    Vect camera_right = Y.crossProduct(test_camera_direction).normalize();
    Vect camera_down = camera_right.crossProduct(test_camera_direction);
    
    Camera test_scene_camera(test_camera_position, test_camera_direction, camera_right, camera_down);
    
    //Light information
    Vect test_light_position(0, 0, -1); //= test_camera_position;
    Light test_scene_light(test_light_position, test_color);
    
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&test_scene_light));
    
    //Set shape Z units away from camera/light -->cube that covers all of image
    Triangle test_triangle1(Vect(100000, -100000, 0), Vect(-100000, 100000, 0), Vect(-100000, -100000, 0), test_color);
    Triangle test_triangle2(Vect(-100000, -100000, 0), Vect(100000, -100000, 0), Vect(100000, 100000, 0), test_color);
    
    scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle1));
    scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle2));
    
    double x_amount;
    double y_amount;
    
    bool shadowed = false;
    double tempRed;
    double tempGreen;
    double tempBlue;
    int anti_aliasing_index;
    
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            current_pixel = y * width + x;
            
            //STARTING WITH A BLANK PIXEL
            double tempRed[anti_aliasing_depth * anti_aliasing_depth];
            double tempGreen[anti_aliasing_depth * anti_aliasing_depth];
            double tempBlue[anti_aliasing_depth * anti_aliasing_depth];
            
            for(int anti_aliasing_x = 0; anti_aliasing_x < anti_aliasing_depth; anti_aliasing_x++)
            {
                for(int anti_aliasing_y = 0; anti_aliasing_y < anti_aliasing_depth; anti_aliasing_y++)
                {
                    anti_aliasing_index = anti_aliasing_y * anti_aliasing_depth + anti_aliasing_x;
                    
                    //CREATE RAY FROM CAMERA TO CURRENT PIXEL
                    if(anti_aliasing_depth == 1)
                    {
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
                    }
                    else
                    {
                        //ANTI-ALIASING
                        if(width > height)
                        {
                            //IMAGE IS WIDER THAN IT IS TALL
                            x_amount = ((x + (double)anti_aliasing_x / ((double) anti_aliasing_depth - 1)) / width) * aspect_ratio - (((width - height) / (double)height) / 2);
                            y_amount = ((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height;
                        }
                        else if(height > width)
                        {
                            //IMAGE IS TALLER THAN IT IS WIDE
                            x_amount = (x + (double)anti_aliasing_x / ((double) anti_aliasing_depth - 1)) / width;
                            y_amount = (((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height) / aspect_ratio - (((height - width) / (double)width) / 2);
                        }
                        else
                        {
                            //IMAGE IS SQUARE
                            x_amount = (x + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / width;
                            y_amount = ((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height;
                        }
                    }
                    
                    Vect test_camera_ray_origin = test_scene_camera.getCameraPosition();
                    Vect test_camera_ray_direction = test_camera_direction.vectAdd(camera_right.vectMult(x_amount - 0.5).vectAdd(camera_down.vectMult(y_amount - 0.5))).normalize();
                    
                    Ray camera_ray(test_camera_ray_origin, test_camera_ray_direction);
                    
                    vector<double> intersections;
                    
                    //LOOP THROUGH OBJECTS IN SCENE, FIND INTERSECTION WITH CAMERA RAY, PUSH VALUE INTO INTERSECTIONS ARRAY
                    for(int index = 0; index < scene_shapes.size(); index++)
                    {
                        intersections.push_back(scene_shapes.at(index)->findIntersection(camera_ray));
                    }
                    
                    int index_of_closest_shape = testingClosestShapeIndex(intersections);
                    
                    if(index_of_closest_shape == -1)
                    {
                        cout << "Test Number Rays that Hit Object: FAILED" << endl;
                        cout << "Pixel: " << current_pixel << endl;
                        break;
                    }
                    else
                    {
                        //INDEX CORRESPONDS TO SHAPE IN SCENE
                        if(intersections.at(index_of_closest_shape) > accuracy)
                        {
                            //DETERMINE POSITION AND DIRECTION VECTORS AT POINT OF INTERSECTION
                            Vect intersection_ray_position = test_camera_ray_origin.vectAdd(test_camera_ray_direction.vectMult(intersections.at(index_of_closest_shape)));
                            
                            Vect intersection_ray_direction = test_camera_ray_direction;
                            
                            Color current_color = testGetColorAt(intersection_ray_position, intersection_ray_direction, scene_shapes, index_of_closest_shape, scene_lights, accuracy, ambient_light, shadowed);//scene_shapes.at(index_of_closest_shape)->getColor();
                            
                            tempRed[anti_aliasing_index] = current_color.getColorRed();
                            tempGreen[anti_aliasing_index] = current_color.getColorGreen();
                            tempBlue[anti_aliasing_index] = current_color.getColorBlue();
                         
                            num_pixels_rays_hit++;


                        }
                    }

                }

            }
            
            //AVERAGE PIXEL COLORS
            double totalRed = 0;
            double totalGreen = 0;
            double totalBlue = 0;
            
            for(int iRed = 0; iRed < anti_aliasing_depth * anti_aliasing_depth; iRed++)
            {
                totalRed = totalRed + tempRed[iRed];
            }
            for(int iGreen = 0; iGreen < anti_aliasing_depth * anti_aliasing_depth; iGreen++)
            {
                totalGreen = totalGreen + tempGreen[iGreen];
            }
            for(int iBlue = 0; iBlue < anti_aliasing_depth * anti_aliasing_depth; iBlue++)
            {
                totalBlue = totalBlue + tempBlue[iBlue];
            }
            
            double avgRed = totalRed / (anti_aliasing_depth * anti_aliasing_depth);
            double avgGreen = totalGreen / (anti_aliasing_depth * anti_aliasing_depth);
            double avgBlue = totalBlue / (anti_aliasing_depth * anti_aliasing_depth);
            
            pixels[current_pixel].r = avgRed;
            pixels[current_pixel].g = avgGreen;
            pixels[current_pixel].b = avgBlue;
            
        }
    }

    endTimer(0, testNumberRays_antiAliasing_cube_Timer, 12, "testNumberRays_antiAliasing_cube");
    total_time_end = clock();
    
    float total_time = ((float)total_time_end - (float)total_time_begin) / 1000000;
    cout << "testNumberRays_antiAliasing_cube() TOTAL TIME: " << total_time << " seconds" << endl;
    
    if(num_pixels_rays_hit == num_pixels)
    {
        cout << "Test Number Rays that Hit Object with Anti Aliasing with Cube: PASSED" << endl;
        return true;
    }
    else
    {
        cout << "Test Number Rays that Hit Object with Anti Aliasing with Cube: FAILED" << endl;
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
    cout << "TEST #4: testRayDistance" << endl;
    
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
    cout << "TEST #5: testColorOfPixel" << endl;
    
}

bool testBoundingBox(int num_pixels, Color test_color, int width, int height, double accuracy, double ambient_light)
{
    cout << "TEST #6: testBoundingBox" << endl;
    
    //TIMING INFORMATION
    initTimer(1, 18000000, 100);
    startTimer( testBoundingBox_Timer );
    clock_t total_time_begin;
    clock_t total_time_end;
    total_time_begin = clock();
    
    test_RGB_Type *pixels = new test_RGB_Type[num_pixels];
    int current_pixel;
    double aspect_ratio = (double)width / (double)height;
    
    int num_pixels_total = width * height;
    
    vector<Shape*> scene_shapes;
    
    //Set camera and light to orthogonal position to be able to calculate number of pixels in a shape
    Vect test_camera_position(0, 0, -1);
    Vect origin(0, 0, 0);
    Vect lookAt(0, 0, 0);
    Vect Y(0, 1, 0);
    Vect difference_between(test_camera_position.getVectX() - lookAt.getVectX(), test_camera_position.getVectY() - lookAt.getVectY(), test_camera_position.getVectZ() - lookAt.getVectZ());
    
    Vect test_camera_direction = difference_between.negative().normalize();
    Vect camera_right = Y.crossProduct(test_camera_direction).normalize();
    Vect camera_down = camera_right.crossProduct(test_camera_direction);
    
    Camera test_scene_camera(test_camera_position, test_camera_direction, camera_right, camera_down);
    
    //Light information
    Vect test_light_position(0, 0, -1); //= test_camera_position;
    Light test_scene_light(test_light_position, test_color);
    
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&test_scene_light));
    
    Triangle test_triangle1(Vect(0, 0, 0), Vect(0, 1, 1), Vect(1, 0, 0), test_color);
    
    scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle1));
    //scene_shapes.push_back(dynamic_cast<Shape*>(&test_triangle2));
    
    //BBox test_bbox;
    //test_bbox.setBBoxXMin(0.);
    //test_bbox.setBBoxYMin(0.);
    //test_bbox.setBBoxZMin(0.);
    //test_bbox.setBBoxXMax(1.);
    //test_bbox.setBBoxYMax(1.);
    //test_bbox.setBBoxZMax(1.);
    //test_bbox.x0 = 0;
    //test_bbox.x1 = 1;
    //test_bbox.y0 = 0;
    //test_bbox.y1 = 1;
    //test_bbox.z0 = 0;
    //test_bbox.z1 = 1;
    
    /*cout << "BBox X Min: " << test_bbox.getBBoxXMin() << endl;
    cout << "BBox Y Min: " << test_bbox.getBBoxYMin() << endl;
    cout << "BBox Z Min: " << test_bbox.getBBoxZMin() << endl;
    cout << "BBox X Max: " << test_bbox.getBBoxXMax() << endl;
    cout << "BBox Y Max: " << test_bbox.getBBoxYMax() << endl;
    cout << "BBox Z Max: " << test_bbox.getBBoxZMax() << endl;
    */
    double x_amount;
    double y_amount;
    
    bool shadowed = false;
    double tempRed;
    double tempGreen;
    double tempBlue;
    int anti_aliasing_index;
    int anti_aliasing_depth = 1;
    
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            current_pixel = y * width + x;
            
            //STARTING WITH A BLANK PIXEL
            double tempRed[anti_aliasing_depth * anti_aliasing_depth];
            double tempGreen[anti_aliasing_depth * anti_aliasing_depth];
            double tempBlue[anti_aliasing_depth * anti_aliasing_depth];
            
            for(int anti_aliasing_x = 0; anti_aliasing_x < anti_aliasing_depth; anti_aliasing_x++)
            {
                for(int anti_aliasing_y = 0; anti_aliasing_y < anti_aliasing_depth; anti_aliasing_y++)
                {
                    anti_aliasing_index = anti_aliasing_y * anti_aliasing_depth + anti_aliasing_x;
                    
                    //CREATE RAY FROM CAMERA TO CURRENT PIXEL
                    if(anti_aliasing_depth == 1)
                    {
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
                    }
                    else
                    {
                        //ANTI-ALIASING
                        if(width > height)
                        {
                            //IMAGE IS WIDER THAN IT IS TALL
                            x_amount = ((x + (double)anti_aliasing_x / ((double) anti_aliasing_depth - 1)) / width) * aspect_ratio - (((width - height) / (double)height) / 2);
                            y_amount = ((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height;
                        }
                        else if(height > width)
                        {
                            //IMAGE IS TALLER THAN IT IS WIDE
                            x_amount = (x + (double)anti_aliasing_x / ((double) anti_aliasing_depth - 1)) / width;
                            y_amount = (((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height) / aspect_ratio - (((height - width) / (double)width) / 2);
                        }
                        else
                        {
                            //IMAGE IS SQUARE
                            x_amount = (x + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / width;
                            y_amount = ((height - y) + (double)anti_aliasing_x / ((double)anti_aliasing_depth - 1)) / height;
                        }
                    }
                    
                    Vect test_camera_ray_origin = test_scene_camera.getCameraPosition();
                    Vect test_camera_ray_direction = test_camera_direction.vectAdd(camera_right.vectMult(x_amount - 0.5).vectAdd(camera_down.vectMult(y_amount - 0.5))).normalize();
                    
                    //Ray camera_ray(test_camera_ray_origin, test_camera_ray_direction);
                    Vect rayOri(-0.5, 0, 0.5);
                    Vect rayDir(1, 0, 0);
                    Ray camera_ray(rayOri, rayDir);
                    
                    vector<double> intersections;
                    
                    //LOOP THROUGH OBJECTS IN SCENE, FIND INTERSECTION WITH CAMERA RAY, PUSH VALUE INTO INTERSECTIONS ARRAY
                    for(int index = 0; index < scene_shapes.size(); index++)
                    {
                        intersections.push_back(scene_shapes.at(index)->findIntersection(camera_ray));
                    }
                    
                    int index_of_closest_shape = testingClosestShapeIndex(intersections);
                    
                    if(index_of_closest_shape == -1)
                    {
                        tempRed[anti_aliasing_index] = 0;
                        tempGreen[anti_aliasing_index] = 0;
                        tempBlue[anti_aliasing_index] = 0;
                    }
                    else
                    {
                        //INDEX CORRESPONDS TO SHAPE IN SCENE
                        if(intersections.at(index_of_closest_shape) > accuracy)
                        {
                            //DETERMINE POSITION AND DIRECTION VECTORS AT POINT OF INTERSECTION
                            Vect intersection_ray_position = test_camera_ray_origin.vectAdd(test_camera_ray_direction.vectMult(intersections.at(index_of_closest_shape)));
                            
                            Vect intersection_ray_direction = test_camera_ray_direction;
                            
                            Color current_color = testGetColorAt(intersection_ray_position, intersection_ray_direction, scene_shapes, index_of_closest_shape, scene_lights, accuracy, ambient_light, shadowed);//scene_shapes.at(index_of_closest_shape)->getColor();
                            
                            tempRed[anti_aliasing_index] = current_color.getColorRed();
                            tempGreen[anti_aliasing_index] = current_color.getColorGreen();
                            tempBlue[anti_aliasing_index] = current_color.getColorBlue();
                        }
                    }
                    
                }
                
            }
            
            //AVERAGE PIXEL COLORS
            double totalRed = 0;
            double totalGreen = 0;
            double totalBlue = 0;
            
            for(int iRed = 0; iRed < anti_aliasing_depth * anti_aliasing_depth; iRed++)
            {
                totalRed = totalRed + tempRed[iRed];
            }
            for(int iGreen = 0; iGreen < anti_aliasing_depth * anti_aliasing_depth; iGreen++)
            {
                totalGreen = totalGreen + tempGreen[iGreen];
            }
            for(int iBlue = 0; iBlue < anti_aliasing_depth * anti_aliasing_depth; iBlue++)
            {
                totalBlue = totalBlue + tempBlue[iBlue];
            }
            
            double avgRed = totalRed / (anti_aliasing_depth * anti_aliasing_depth);
            double avgGreen = totalGreen / (anti_aliasing_depth * anti_aliasing_depth);
            double avgBlue = totalBlue / (anti_aliasing_depth * anti_aliasing_depth);
            
            pixels[current_pixel].r = avgRed;
            pixels[current_pixel].g = avgGreen;
            pixels[current_pixel].b = avgBlue;
            
        }
    }
    
    endTimer(0, testBoundingBox_Timer, 13, "testBoundingBox");
    total_time_end = clock();
    
    float total_time = ((float)total_time_end - (float)total_time_begin) / 1000000;
    cout << "testBoundingBox() TOTAL TIME: " << total_time << " seconds" << endl;
    
    /*if(num_pixels_rays_hit == num_pixels)
    {
        cout << "Test Number Rays that Hit Object with Anti Aliasing with Cube: PASSED" << endl;
        return true;
    }
    else
    {
        cout << "Test Number Rays that Hit Object with Anti Aliasing with Cube: FAILED" << endl;
        cout << "Number of rays that should have hit the object: " << num_pixels << endl;
        cout << "Number of rays that hit the object: " << num_pixels_rays_hit << endl;
        return false;
    }*/
    
    
    
}


//OUTPUT INFO ABOUT WHAT TESTS WERE PASSED AND WHAT ONES WEREN'T
void testsPassedFailed()
{
    
}

//OUTPUT INFO ABOUT WHAT EACH TEST IS DESIGNED TO DO AND WHAT ITS OUTPUT SHOULD BE
void testInfo()
{
    //TEST 1: Test number of triangles
    //TEST 2: Test number of rays when entire image should be covered by shape
    //TEST 3: Test number of rays when entire image should be covered by shape with anti aliasing
    //TEST 7: Test number of rays with cube as shape covering entire image with anti aliasing
}


#endif

#endif
