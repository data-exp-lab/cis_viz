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

#include "commandArgs.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "constants.hpp"
#include "parameters.hpp"
#include "equations.hpp"
#include "normal.hpp"

#include "climate.hpp"



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
        
        unsigned char color[3] = {static_cast<unsigned char>(floor(blue)), static_cast<unsigned char>(floor(green)), static_cast<unsigned char>(floor(red))};
        
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

int current_pixel;

int main(int argc, char *argv[])
{
    clock_t total_time_begin;
    clock_t total_time_end;
    
    total_time_begin = clock();
    
    debug("Beginning totalTimer");
    
    initTimer(1, 18000000, 100);
    startTimer( totalTimer );
    
    extern commandLineArgs cla;
    
    ProcessCommandLine(argc, argv);
    
    int dpi = 72;
    
    int width = 640;
    int height = 480;
    int anti_aliasing_depth = cla.antiAliasingDepth;
    double anti_aliasing_threshold = 0.1;
    
    debug("Width: %d\nHeight: %d", width, height);
    
    double aspect_ratio = (double)width / (double)height;
    double ambient_light = 0.25;
    double accuracy = 0.000001;
    
    string fileType = ".txt";
    
    bool shadowed = true;
    bool PLY = false;
    
    debug("Shadowed = %d", shadowed);
    
    int num_pixels = width * height;
    
    RGB_Type *pixels = new RGB_Type[num_pixels];
    
    //GEOMETRY FILE
    vector<float> x1_main;
    vector<float> y1_main;
    vector<float> z1_main;
    vector<float> x2_main;
    vector<float> y2_main;
    vector<float> z2_main;
    vector<float> x3_main;
    vector<float> y3_main;
    vector<float> z3_main;
    
    double startHour = cla.startHour;
    double endHour = cla.endHour;
    double intervalHour = cla.intervalHour;
    double leafTransmittance = cla.leafTransmittance;
    double leafReflectivity = cla.leafReflectivity;
    double leafAbsorbed;
    
    vector<float> DOY_main;
    
    if(leafTransmittance + leafReflectivity > 1)
    {
        cout << "Transmittance and Reflectivity cannot be greater than 1" << endl;
        debug("Transmittance and Reflectivity cannot be greater than 1");
        debug("Transmittance: %d", leafTransmittance);
        debug("Reflectivity: %d", leafReflectivity);
        return 0;
    }
    else
    {
        leafAbsorbed = 1 - leafTransmittance - leafReflectivity;
        debug("Transmittance: %d", leafTransmittance);
        debug("Reflectivity: %d", leafReflectivity);
        debug("Absorbed: %d", leafAbsorbed);
    }
    
    //FOR PLY FILES
    vector<float> x_main;
    vector<float> y_main;
    vector<float> z_main;
    vector<float> red_main;
    vector<float> green_main;
    vector<float> blue_main;
    
    //FOR PLY FILES --> CONNECTIVITY
    vector<float> num_vertices_to_connect_main;
    vector<float> vertex1_main;
    vector<float> vertex2_main;
    vector<float> vertex3_main;
    
    if(PLY == true)
    {
        //READ IN THE GEOMETRY OF THE PLANT FROM PLY FILE
        readGeometryFilePLY(cla.geometryFile, ref(x_main), ref(y_main), ref(z_main), ref(red_main), ref(green_main), ref(blue_main), ref(num_vertices_to_connect_main), ref(vertex1_main), ref(vertex2_main), ref(vertex3_main));
    }
    else
    {
        //READ IN THE GEOMETRY OF THE PLANT FROM TXT FILE
        readGeometryFileTXT(cla.geometryFile, ref(x1_main), ref(y1_main), ref(z1_main), ref(x2_main), ref(y2_main), ref(z2_main), ref(x3_main), ref(y3_main), ref(z3_main));
    }
    
    Parameters ps;
    Constants cs;
    //cs.NEAREST_RAY_DISTANCE = rayDistance;
    
    //CLIMATE INFO
    Climate climate;
    double stn = cs.TIME_SOLAR_NOON;
    double start = startHour + cs.START_MINUTE / 60;
    double end = endHour + cs.END_MINUTE / 60;
    double interval = intervalHour + cs.INTERVAL_MINUTE / 60;
    
    int leafID = 1;
    double CLAI = 0.1;
    double PPFD = 1000;
    ps.prepare(leafID, CLAI, PPFD);
    
    //FIX: MAKE DOY VALUE VARIABLE, NOT CONSTANT AT 150
    float DOY_val = 150;
    //FIX: TOTAL GUESS HERE
    float leafL = 1;
    //FIX: TOTAL GUESS HERE
    float n_per_area = 1;
    
    float position = 0.0;
    
    //FOR EVERY TIME STEP CALCULATED
    //FIX: DOUBLE CHECK THAT THIS IS CORRECT
    int num = (int)((end - start) / interval);
    for(int i = 0; i <= num; i++)
    {
        DOY_main.push_back(DOY_val);
    }
    
    //CALCULATE PPFD FROM ATMOSPHERIC TRANSMITTANCE
    climate.climate_calculation_PPFD(cs.LATITUDE, stn, cs.ATMOSPHERIC_TRANSMITTANCE, DOY_main, start, end, interval, ps, cs);
    
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
    Vect light_position(-7, 10, -10);
    Light scene_light1(light_position, white_light);
    
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&scene_light1));

    double x_amount;
    double y_amount;

    //SCENE OBJECTS
    Sphere scene_sphere1(origin, 1, blue_light);
    Plane scene_plane1(Y, -1, red_light);
    Triangle scene_triangle1(Vect(5, 0, 0), Vect(0, -3, 0), Vect(0, 0, -5), green_light);
    //Triangle scene_triangle1(Vect(-1.25, 0, 7.78), Vect(-1.45, 0, 7.63), Vect(-1.28, 0, 7.07), green_light);
    
    vector<Shape*> scene_shapes;
    scene_shapes.push_back(dynamic_cast<Shape*>(&scene_sphere1));
    scene_shapes.push_back(dynamic_cast<Shape*>(&scene_plane1));
    scene_shapes.push_back(dynamic_cast<Shape*>(&scene_triangle1));
    //scene_shapes.push_back(dynamic_cast<Shape*>(&plant_triangle1));
    
    if(PLY == true)
    {
        //SET UP CONNECTIVITY OF POINTS INTO TRIANGLES
        int num_vertices;
        int vertex1_index;
        int vertex2_index;
        int vertex3_index;
        
        float x_for_point1;
        float y_for_point1;
        float z_for_point1;
        float x_for_point2;
        float y_for_point2;
        float z_for_point2;
        float x_for_point3;
        float y_for_point3;
        float z_for_point3;
        
        cout << "x_main.size(): " << x_main.size() << endl;
        for(int i = 0; i < x_main.size(); i++)
        {
            cout << "x_main[" << i << "]: " << x_main[i] << endl;
        }
        
        for(int i = 0; i < x_main.size(); i++)
        {
            num_vertices = num_vertices_to_connect_main[i];
            if(num_vertices == 3)
            {
                vertex1_index = vertex1_main[i];
                vertex2_index = vertex2_main[i];
                vertex3_index = vertex3_main[i];

                Vect point1(x_main[vertex1_index], y_main[vertex1_index], z_main[vertex1_index]);
                Vect point2(x_main[vertex2_index], y_main[vertex2_index], z_main[vertex2_index]);
                Vect point3(x_main[vertex3_index], y_main[vertex3_index], z_main[vertex3_index]);
                
                cout << "point1: " << point1.getVectX() << " " << point1.getVectY() << " " << point1.getVectZ() << endl;
                Triangle newTri(point1, point2, point3, leafID, leafL, position, CLAI, leafTransmittance, leafReflectivity, n_per_area, start, end, intervalHour);
                
                scene_shapes.push_back(dynamic_cast<Shape*>(&newTri));
            }
            //FIX: change once set up to do things other than triangles (if necessary)
            else
            {
                printf("Cannot construct a triangle from given information");
                return 0;
            }
        }
        
    }
    else
    {
        //FIX: IF USING TXT FILE FOR GEOMETRY
        for(int i = 0; i < x1_main.size(); i++)
        {
            Vect point1(x1_main[i], y1_main[i], z1_main[i]);
            Vect point2(x2_main[i], y2_main[i], z2_main[i]);
            Vect point3(x3_main[i], y3_main[i], z3_main[i]);
            
            cout << "point1: " << point1.getVectX() << " " << point1.getVectY() << " " << point1.getVectZ() << endl;
            Triangle newTri(point1, point2, point3, leafID, leafL, position, CLAI, leafTransmittance, leafReflectivity, n_per_area, start, end, intervalHour);

            scene_shapes.push_back(dynamic_cast<Shape*>(&newTri));
        }
    }
    cout << "scene_shapes.size(): " << scene_shapes.size() << endl;
    
    debug("Number of shapes in scene: %d", scene_shapes.size());
    
    vector<double> area_total;
    vector<double> PPFD_total;
    
    //FIX: DO THIS FOR EVERY TIMESTEP
    //RETURN COLOR PER PIXEL
    for(int timestep = 0; timestep <= num; timestep++)
    {
        cout << "timestep = " << timestep << endl;
        debug("timestep: %d", timestep);
        
        double hour = start + timestep * interval;
        cout << "hour = " << hour << endl;
        debug("hour: %d", hour);
        
        area_total.clear();
        PPFD_total.clear();
        
        double light_d_x = climate.direct_light_d_list[timestep].x;
        double light_d_y = climate.direct_light_d_list[timestep].y;
        double light_d_z = climate.direct_light_d_list[timestep].z;
        
        double direct_light_ppfd = climate.ppfd_direct_light[timestep];
        //FIX: MAKE DIFFUSE STUFF WORK
        //double diffuse_light_ppfd = climate.ppfd_diffuse_light[timestep];
        
        double dir_pf = direct_light_ppfd * 1e-4; //ADD IN LIGHT_NEAREST_DISTANCE
        //double dif_pf = diffuse_light_ppfd * 1e-4; //ADD IN LIGHT_NEAREST_DISTANCE
        
        //TRIANGLES ARE IN SCENE_SHAPES ALREADY
        vector<Triangle*>::iterator it;
        /*for(int j = 0; j < scene_shapes.size(); j++)
        {
            double triangle_area = (((*it)->B - (*it)->A) ^ ((*it)->C - (*it)->A)).length() * 0.5;
            double area_factor = 1 / (triangle_area * 1e-4);
            
            vector<double> photonFlux_up_dir = (*it)->photonFlux_up_dir;
            vector<double> photonFlux_up_diff = (*it)->photonFlux_up_diff;
            vector<double> photonFlux_up_scat = (*it)->photonFlux_up_scat;
            vector<double> photonFlux_down_dir = (*it)->photonFlux_down_dir;
            vector<double> photonFlux_down_diff = (*it)->photonFlux_down_diff;
            vector<double> photonFlux_down_scat = (*it)->photonFlux_down_scat;
            
            //SELECT PPFD FOR GIVEN TIME
            double it1 = photonFlux_up_dir[timestep];
            double it2 = photonFlux_up_diff[timestep];
            double it3 = photonFlux_up_scat[timestep];
            double it4 = photonFlux_down_dir[timestep];
            double it5 = photonFlux_down_diff[timestep];
            double it6 = photonFlux_down_scat[timestep];
            
            // TOTAL PPFD FROM DIRECT, DIFFUSE, AND SCATTERED LIGHT
            double PPFD_tot = (it1 + it2 + it3 + it4 + it5 + it6) * area_factor;
            
            area_total.push_back(triangle_area);
            PPFD_total.push_back(PPFD_tot);
        }*/
        
        int anti_aliasing_index;
        double tempRed;
        double tempGreen;
        double tempBlue;
        
        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < height; y++)
            {
                current_pixel = y * width + x;
                debug("current_pixel: %d", current_pixel);
                
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
                            //NO ANTI-ALIASING
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
                            /*pixels[current_pixel].r = 0;
                            pixels[current_pixel].g = 0;
                            pixels[current_pixel].b = 0;*/
                            tempRed[anti_aliasing_index] = 0;
                            tempGreen[anti_aliasing_index] = 0;
                            tempBlue[anti_aliasing_index] = 0;
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
                                    
                                    //pixels[current_pixel].r = intersection_color.getColorRed();
                                    //pixels[current_pixel].g = intersection_color.getColorGreen();
                                    //pixels[current_pixel].b = intersection_color.getColorBlue();
                                    tempRed[anti_aliasing_index] = intersection_color.getColorRed();
                                    tempGreen[anti_aliasing_index] = intersection_color.getColorGreen();
                                    tempBlue[anti_aliasing_index] = intersection_color.getColorBlue();
                                    debug("color of pixel[%d] is: [%d][%d][%d]", pixels[current_pixel], intersection_color.getColorRed(), intersection_color.getColorGreen(), intersection_color.getColorBlue());
                                }
                                else
                                {
                                    Color current_color = scene_shapes.at(index_of_closest_shape)->getColor();
                                    
                                    //pixels[current_pixel].r = current_color.getColorRed();
                                    //pixels[current_pixel].g = current_color.getColorGreen();
                                    //pixels[current_pixel].b = current_color.getColorBlue();
                                    tempRed[anti_aliasing_index] = current_color.getColorRed();
                                    tempGreen[anti_aliasing_index] = current_color.getColorGreen();
                                    tempBlue[anti_aliasing_index] = current_color.getColorBlue();
                                    debug("color of pixel[%d] is: [%d][%d][%d]", pixels[current_pixel], current_color.getColorRed(), current_color.getColorGreen(), current_color.getColorBlue());
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
        }
        
        savebmp("test.bmp", width, height, dpi, pixels);
        debug("saved image");
        
        //OUTPUT FILE
        //const char *output_file_name = cla.outputFile;
        std::ostringstream output_string;
        output_string<<cla.outputFile<<"_"<<hour<<"_hour"<<fileType;
        string output_file_name = output_string.str();
        
        cout << "output_file: " << output_file_name << endl;
        
        writePPFDFile(output_file_name, area_total, PPFD_total, num);
        
        pixels.clear();
    }
    
    delete[] pixels;
    
    total_time_end = clock();
    
    float total_time = ((float)total_time_end - (float)total_time_begin) / 1000000;
    cout << "Total time: " << total_time << " seconds" << endl;
    
    endTimer(0, totalTimer, 1, "main");

    //SAVE TIMINGS TO FILE
    const char *filename = "timing.txt";
    saveTimers(filename);
    
    debug("end totalTimer");
    
    cout << "EXIT SUCCESSFUL" << endl;
    return 0;
}


