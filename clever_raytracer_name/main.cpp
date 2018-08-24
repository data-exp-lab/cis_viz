#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "vect.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "light.hpp"
#include "source.hpp"


//FIX: change to shapes and be children of that within same file
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "shape.hpp"

//DO MORE COMPREHENSIVE/ACCURATE TIMINGS THIS WAY
#include "timing.hpp"
#include "debug.hpp"



using namespace std;

struct RGB_Type
{
    double r;
    double g;
    double b;
    
};

void savebmp(const char *filename, int w, int h, int dpi, RGB_Type *data)
{
    FILE *f;
    int k = w * h;
    int s = 4 * k;
    
    int filesize = 54 + s;
    
    double factor = 39.375;
    int m = static_cast<int>(factor);
    
    int ppm = dpi * m;
    
    unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
    
    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);
    
    bmpinfoheader[4] = (unsigned char)(w);
    bmpinfoheader[5] = (unsigned char)(w >> 8);
    bmpinfoheader[6] = (unsigned char)(w >> 16);
    bmpinfoheader[7] = (unsigned char)(w >> 24);
    
    bmpinfoheader[8] = (unsigned char)(h);
    bmpinfoheader[9] = (unsigned char)(h >> 8);
    bmpinfoheader[10] = (unsigned char)(h >> 16);
    bmpinfoheader[11] = (unsigned char)(h >> 24);
    
    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s >> 8);
    bmpinfoheader[23] = (unsigned char)(s >> 16);
    bmpinfoheader[24] = (unsigned char)(s >> 24);
    
    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm >> 8);
    bmpinfoheader[27] = (unsigned char)(ppm >> 16);
    bmpinfoheader[28] = (unsigned char)(ppm >> 24);
    
    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm >> 8);
    bmpinfoheader[31] = (unsigned char)(ppm >> 16);
    bmpinfoheader[32] = (unsigned char)(ppm >> 24);
    
    f = fopen(filename, "wb");
    
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    
    for(int i = 0; i < k; i++)
    {
        RGB_Type rgb = data[i];
        
        double red = (data[i].r) * 255;
        double green = (data[i].g) * 255;
        double blue = (data[i].b) * 255;
        
        //unsigned char color[3] = {(int)floor(blue), (int)floor(green), (int)floor(red)};
        unsigned char color[3] = {static_cast<unsigned char>(floor(blue)), static_cast<unsigned char>(floor(green)), static_cast<unsigned char>(floor(red))};
        //unsigned char color[3] = {static_cast<unsigned char>(int)(floor(blue)), static_cast<unsigned char>(int)(floor(green)), static_cast<unsigned char>(int)(floor(red))};
        
        fwrite(color, 1, 3, f);
    }
    
    fclose(f);
    
}

int closestShapeIndex(vector<double> shape_intersections)
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

//FIX
Color getColorAt(Vect intersection_ray_position, Vect intersection_ray_direction, vector<Shape*> scene_shapes, int index_of_closest_shape, vector<Source*> scene_lights, double accuracy, double ambient_light)
{
    Color closest_shape_color = scene_shapes.at(index_of_closest_shape)->getColor();
    Vect closest_shape_normal = scene_shapes.at(index_of_closest_shape)->getNormalAt(intersection_ray_position);
    
    
    Color final_color = closest_shape_color.colorScalar(ambient_light);
    
    //REFLECTION
    //ALPHA [2]
    /*if(closest_shape_color.getColorAlpha() > 0 && closest_shape_color.getColorAlpha() <= 1)
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
        
        int index_of_closest_shape_with_reflection = closestShapeIndex(reflection_intersections);
        
        if(index_of_closest_shape_with_reflection != -1)
        {
            //REFLECTION RAY DIDN'T INTERSECT WITH ANYTHING ELSE
            if(reflection_intersections.at(index_of_closest_shape_with_reflection) > accuracy)
            {
                Vect reflection_intersection_position = intersection_ray_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_closest_shape_with_reflection)));
                Vect reflection_intersection_ray_direction = reflection_direction;
                
                Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_shapes, index_of_closest_shape_with_reflection, scene_lights, accuracy, ambient_light);
                
                final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(closest_shape_color.getColorAlpha()));
            }
        }
        
        
    }*/
    
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

    //return final_color;
    return final_color.clip();
}

int current_pixel;

int main(int argc, char *argvp[])
{
    clock_t total_time_begin;
    clock_t total_time_end;
    
    total_time_begin = clock();
    
    debug("Beginning totalTimer");
    
    initTimer(1, 18000000, 100);
    startTimer( totalTimer );
    
    int dpi = 72;
    
    int width = 640;
    int height = 480;
    
    debug("Width: %d\nHeight: %d", width, height);
    
    double aspect_ratio = (double)width / (double)height;
    double ambient_light = 0.25;
    double accuracy = 0.000001;
    
    bool shadowed = false;
    
    debug("Shadowed = %d", shadowed);
    
    int num_pixels = width * height;
    
    RGB_Type *pixels = new RGB_Type[num_pixels];
    
    Vect origin(0, 0, 0);
    Vect X(1, 0, 0);
    Vect Y(0, 1, 0);
    Vect Z(0, 0, 1);
    
    Vect camera_position(3, 1.5, -12);
    Vect lookAt(0, 0, 0);
    Vect difference_between(camera_position.getVectX() - lookAt.getVectX(), camera_position.getVectY() - lookAt.getVectY(), camera_position.getVectZ() - lookAt.getVectZ());
    
    Vect camera_direction = difference_between.negative().normalize();
    Vect camera_right = Y.crossProduct(camera_direction).normalize();
    Vect camera_down = camera_right.crossProduct(camera_direction);
    
    Camera scene_camera(camera_position, camera_direction, camera_right, camera_down);
    
    Color white_light(1.0, 1.0, 1.0, 0);
    Color green_light(0.5, 1.0, 0.5, 0);
    Color red_light(1.0, 0.0, 0.0, 0);
    Color gray_light(0.5, 0.5, 0.5, 0);
    Color black_light(0.0, 0.0, 0.0, 0);
    Color orange_light(0.9, 0.75, 0.3, 0);
    Color blue_light(0.0, 0.0, 1, 0);
    
    //FIX: Calculate position of sun, becomes new light
    Vect light_position(3, 1.5, -12);
    Light scene_light1(light_position, white_light);
    
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&scene_light1));
    
    //slightly to left and right of direction camera is pointing
    //without anti-aliasing
    double x_amount;
    double y_amount;
    
    //FIX: Replace with scene files of triangle/leaf geometry
    
    
    
    //SCENE OBJECTS
    Sphere scene_sphere1(origin, 1, blue_light);
    Plane scene_plane1(Y, -3, red_light);
    Triangle scene_triangle1(Vect(5, 0, 0), Vect(0, -3, 0), Vect(0, 0, -5), green_light);
    
    vector<Shape*> scene_shapes;
    scene_shapes.push_back(dynamic_cast<Shape*>(&scene_sphere1));
    scene_shapes.push_back(dynamic_cast<Shape*>(&scene_plane1));
    scene_shapes.push_back(dynamic_cast<Shape*>(&scene_triangle1));
    
    debug("Number of shapes in scene: %d", scene_shapes.size());
    
    //return color
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            current_pixel = y * width + x;
            debug("current_pixel: %d", current_pixel);
            
            if(width > height)
            {
                //IMAGE IS WIDER THAN IT IS TALL
                x_amount = ((x + 0.5) / width) * aspect_ratio - (((width - height) / (double)height) / 2);
                y_amount = ((height - y) + 0.5) / height;
            }
            else if(height > width)
            {
                //IMAGE IS TALLER THAN IT IS WIDE
                x_amount = (x + 0.5) / width;
                y_amount = (((height - y) + 0.5) / height) / aspect_ratio - (((height - width) / (double)width) / 2);
            }
            else
            {
                //IMAGE IS SQUARE
                x_amount = (x + 0.5) / width;
                y_amount = ((height - y) + 0.5) / height;
            }
            
            Vect camera_ray_origin = scene_camera.getCameraPosition();
            Vect camera_ray_direction = camera_direction.vectAdd(camera_right.vectMult(x_amount - 0.5).vectAdd(camera_down.vectMult(y_amount - 0.5))).normalize();
            
            Ray camera_ray(camera_ray_origin, camera_ray_direction);
            
            vector<double> intersections;
            
            //LOOP THROUGH OBJECTS IN SCENE, FIND INTERSECTION WITH CAMERA RAY, PUSH VALUE INTO INTERSECTIONS ARRAY
            for(int index = 0; index < scene_shapes.size(); index++)
            {
                intersections.push_back(scene_shapes.at(index)->findIntersection(camera_ray));
                debug("intersections: %d", intersections[index]);
            }
            
            int index_of_closest_shape = closestShapeIndex(intersections);
            debug("index_of_closest_shape: %d", index_of_closest_shape);
            
            //cout << "intersection[" << x << "][" << y << "]: " << index_of_closest_shape << endl;
            
            //FIX: make this only enabled when doing testing
            //Test that color changes
            /*pixels[current_pixel].r = 23;
            pixels[current_pixel].g = 250;
            pixels[current_pixel].b = 50;*/
            
            if(index_of_closest_shape == -1)
            {
                //SET BACKGROUND OF IMAGE
                pixels[current_pixel].r = 0;
                pixels[current_pixel].g = 0;
                pixels[current_pixel].b = 0;
                debug("Set pixels[%d] to black", pixels[current_pixel]);
            }
            else
            {
                //INDEX CORRESPONDS TO SHAPE IN SCENE
                if(intersections.at(index_of_closest_shape) > accuracy)
                {
                    debug("intersections[%d] > accuracy", intersections.at(index_of_closest_shape));
                    //DETERMINE POSITION AND DIRECTION VECTORS AT POINT OF INTERSECTION
                    Vect intersection_ray_position = camera_ray_origin.vectAdd(camera_ray_direction.vectMult(intersections.at(index_of_closest_shape)));
                    
                    Vect intersection_ray_direction = camera_ray_direction;
                    
                    if(shadowed == true)
                    {
                        Color intersection_color = getColorAt(intersection_ray_position, intersection_ray_direction, scene_shapes, index_of_closest_shape, scene_lights, accuracy, ambient_light);
                    
                        pixels[current_pixel].r = intersection_color.getColorRed();
                        pixels[current_pixel].g = intersection_color.getColorGreen();
                        pixels[current_pixel].b = intersection_color.getColorBlue();
                        debug("color of pixel[%d] is: [%d][%d][%d]", pixels[current_pixel], intersection_color.getColorRed(), intersection_color.getColorGreen(), intersection_color.getColorBlue());
                    }
                    else
                    {
                    
                        Color current_color = scene_shapes.at(index_of_closest_shape)->getColor();
                        
                        pixels[current_pixel].r = current_color.getColorRed();
                        pixels[current_pixel].g = current_color.getColorGreen();
                        pixels[current_pixel].b = current_color.getColorBlue();
                        debug("color of pixel[%d] is: [%d][%d][%d]", pixels[current_pixel], current_color.getColorRed(), current_color.getColorGreen(), current_color.getColorBlue());
                    }
                    
                }
                
            }
            
        }
        
    }
    
    savebmp("test.bmp", width, height, dpi, pixels);
    debug("saved image");
    
    delete[] pixels;
    
    total_time_end = clock();
    
    float total_time = ((float)total_time_end - (float)total_time_begin) / 1000000;
    cout << "Total time: " << total_time << " seconds" << endl;
    
    endTimer(0, totalTimer, 1, "main");

    //SAVE TIMINGS TO FILE
    const char *filename = "test.txt";
    saveTimers(filename);
    
    debug("end totalTimer");
    
    return 0;
}
















