#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <valarray>

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

#ifdef RUN_TESTS
    #include "tests.hpp"
#endif

#include "commandArgs.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "constants.hpp"
#include "parameters.hpp"
#include "equations.hpp"
#include "normal.hpp"

//#include "accel.hpp"
//#include "bvh.hpp"
#include "bbox.hpp"
//#include "bbox_accel.hpp"
#include "mesh.hpp"

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

//Color getColorAt(Vect intersection_ray_position, Vect intersection_ray_direction, vector<Shape*> scene_shapes, int index_of_closest_shape, vector<Source*> scene_lights, double accuracy, double ambient_light, bool shadowed)
Color getColorAt(Vect intersection_ray_position, Vect intersection_ray_direction, vector<Triangle*> scene_shapes, int index_of_closest_shape, vector<Source*> scene_lights, double accuracy, double ambient_light, bool shadowed)
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
                
                Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_shapes, index_of_closest_shape_with_reflection, scene_lights, accuracy, ambient_light, shadowed);
                
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
            //for(int o = 2; o < 3 && shadowed == false; o++)
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
    //ProfilerStart("cis_ray.prof");
    
    clock_t total_time_begin;
    clock_t total_time_end;
    
    total_time_begin = clock();
    
    debug("Beginning totalTimer");
    
    initTimer(1, 18000000, 100);
    startTimer( totalTimer );
    
    extern commandLineArgs cla;
    
    ProcessCommandLine(argc, argv);
    
    int dpi = 72;
    
    //int width = 640;
    //int height = 480;
    int width = cla.imageWidth;
    int height = cla.imageHeight;
    int anti_aliasing_depth = cla.antiAliasingDepth;
    double anti_aliasing_threshold = 0.1;
    
    debug("Width: %d\nHeight: %d", width, height);
    
    double aspect_ratio = (double)width / (double)height;
    double ambient_light = 0.25;
    double accuracy = 0.000001;
    
    string fileType = ".txt";
    string inputFileName = cla.geometryFile;
    
    bool shadowed = true;
    bool PLY = true;
    
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
    
    int num_element_vertex;
    int num_element_face;
    
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
    
    //FIX: MAKE RELATIVE TO COORDINATES; I.E., WHEN SMALLER, USE A SMALLER BUFFER SO SHAPES ARE GOING TO BE SEEN
    //SET UP FIELD WITH MIN AND MAX FOR X, Y, Z DIRECTIONS
    float min_x = 0;
    float max_x = 0;
    float min_y = 0;
    float max_y = 0;
    float min_z = 0;
    float max_z = 0;
    
    float buffer = 20.0;
    
    float min_x_with_buffer;
    float max_x_with_buffer;
    float min_y_with_buffer;
    float max_y_with_buffer;
    float min_z_with_buffer;
    float max_z_with_buffer;
    
    if(PLY == true)
    {
        //READ IN THE GEOMETRY OF THE PLANT FROM PLY FILE
        readGeometryFilePLY(cla.geometryFile, ref(x_main), ref(y_main), ref(z_main), ref(red_main), ref(green_main), ref(blue_main), ref(num_vertices_to_connect_main), ref(vertex1_main), ref(vertex2_main), ref(vertex3_main), ref(min_x), ref(max_x), ref(min_y), ref(max_y), ref(min_z), ref(max_z), ref(num_element_vertex), ref(num_element_face));
    }
    else
    {
        //READ IN THE GEOMETRY OF THE PLANT FROM TXT FILE
        readGeometryFileTXT(cla.geometryFile, ref(x1_main), ref(y1_main), ref(z1_main), ref(x2_main), ref(y2_main), ref(z2_main), ref(x3_main), ref(y3_main), ref(z3_main));
    }
    int size_of_x_main = x_main.size();
    debug("num_element_face: %d", num_element_face);
    
    if(PLY == true)
    {
        min_x_with_buffer = min_x - buffer;
        max_x_with_buffer = max_x + buffer;
        min_y_with_buffer = min_y - buffer;
        max_y_with_buffer = max_y + buffer;
        min_z_with_buffer = min_z - buffer;
        max_z_with_buffer = max_z + buffer;
        
        /*cout << "min_x_with_buffer: " << min_x_with_buffer << endl;
        cout << "max_x_with_buffer: " << max_x_with_buffer << endl;
        cout << "min_y_with_buffer: " << min_y_with_buffer << endl;
        cout << "max_y_with_buffer: " << max_y_with_buffer << endl;
        cout << "min_z_with_buffer: " << min_z_with_buffer << endl;
        cout << "max_z_with_buffer: " << max_z_with_buffer << endl;*/
        
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
    
    //FIX: DOUBLE CHECK IF WE NEED THESE OR NOT
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
    
    Vect origin(0, 0, 0);
    Vect X(1, 0, 0);
    Vect Y(0, 1, 0);
    Vect Z(0, 0, 1);
    //3, 1.5, -12
    Vect camera_position(0, -1, -25);
    //Vect camera_position(0, 0, -30);
    Vect lookAt(0, 0, 0);
    Vect difference_between(camera_position.getVectX() - lookAt.getVectX(), camera_position.getVectY() - lookAt.getVectY(), camera_position.getVectZ() - lookAt.getVectZ());
    
    Vect camera_direction = difference_between.negative().normalize();
    //cout << "CAMERA DIRECTION" << endl;
    //cout << "X: " << camera_direction.getVectX() << " Y: " << camera_direction.getVectY() << " Z: " << camera_direction.getVectZ() << endl;
    Vect camera_right = Y.crossProduct(camera_direction).normalize();

    Vect camera_down = camera_right.crossProduct(camera_direction);
    //cout << "CAMERA DOWN" << endl;
    //cout << "X: " << camera_down.getVectX() << " Y: " << camera_down.getVectY() << " Z: " << camera_down.getVectZ() << endl;
    
    Camera scene_camera(camera_position, camera_direction, camera_right, camera_down);
    
    Color white_light(1.0, 1.0, 1.0, 0);
    Color green_light(0.5, 1.0, 0.5, 0);
    Color red_light(1.0, 0.0, 0.0, 0);
    Color gray_light(0.5, 0.5, 0.5, 0);
    Color black_light(0.0, 0.0, 0.0, 0);
    Color orange_light(0.9, 0.75, 0.3, 0);
    Color blue_light(0.0, 0.0, 1, 0);
    Color yellow_light(1, 1, 0, 0);
    Color purple_light(.5, 0.0, .5, 0);
    
    //FIX: Calculate position of sun, becomes new light
    //Vect light_position(0, -1, -25);
    Vect light_position = camera_position;
    //Vect light_position(0, 0, -30);
    //Vect light_position(-7, 10, -10);
    Light scene_light1(light_position, white_light);
    
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&scene_light1));

    double x_amount;
    double y_amount;
    
    vector<Shape*> scene_shapes;
    vector<Triangle*> scene_triangles;

    //SCENE OBJECTS
    /*Sphere scene_sphere1(origin, 1, blue_light);
    Plane scene_plane1(Y, -1, red_light);
    Triangle scene_triangle1(Vect(5, 0, 0), Vect(0, -3, 0), Vect(0, 0, -5), green_light);
    Triangle pyramid_triangle0(Vect(1.0, 0.0, 0.0), Vect(0.0, 0.0, 0.0), Vect(0.0, 1.0, 0.0), red_light);
    Triangle pyramid_triangle1(Vect(1.0, 0.0, 0.0), Vect(0.0, 1.0, 0.0), Vect(1.0, 1.0, 0.0), green_light);
    Triangle pyramid_triangle2(Vect(0.0, 0.0, 0.0), Vect(1.0, 0.0, 0.0), Vect(0.5, 0.5, 1.6), blue_light);
    Triangle pyramid_triangle3(Vect(0.0, 0.0, 0.0), Vect(0.5, 0.5, 1.6), Vect(0.0, 1.0, 0.0), purple_light);
    Triangle pyramid_triangle4(Vect(0.0, 1.0, 0.0), Vect(0.5, 0.5, 1.6), Vect(1.0, 1.0, 0.0), orange_light);
    Triangle pyramid_triangle5(Vect(1.0, 0.0, 0.0), Vect(1.0, 1.0, 0.0), Vect(0.5, 0.5, 1.6), yellow_light);*/
    
    //scene_shapes.push_back(dynamic_cast<Shape*>(&scene_sphere1));
    //scene_shapes.push_back(dynamic_cast<Shape*>(&scene_plane1));
    //scene_shapes.push_back(dynamic_cast<Shape*>(&scene_triangle1));
    /*scene_shapes.push_back(dynamic_cast<Shape*>(&pyramid_triangle0));
    scene_shapes.push_back(dynamic_cast<Shape*>(&pyramid_triangle1));
    scene_shapes.push_back(dynamic_cast<Shape*>(&pyramid_triangle2));
    scene_shapes.push_back(dynamic_cast<Shape*>(&pyramid_triangle3));
    scene_shapes.push_back(dynamic_cast<Shape*>(&pyramid_triangle4));
    scene_shapes.push_back(dynamic_cast<Shape*>(&pyramid_triangle5));*/
    
    /////////////////
    
    Vect point1;
    Vect point2;
    Vect point3;
    float point1_x_coord;
    float point1_y_coord;
    float point1_z_coord;
    float point2_x_coord;
    float point2_y_coord;
    float point2_z_coord;
    float point3_x_coord;
    float point3_y_coord;
    float point3_z_coord;
    Triangle triangle_test;
    Color triangle_color;
    
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
    
    Color point1_color;
    Color point2_color;
    Color point3_color;
    
    int red_color_index;
    int green_color_index;
    int blue_color_index;
    
    double tri_red_average;
    double tri_green_average;
    double tri_blue_average;
    
    float point1_red;
    float point1_green;
    float point1_blue;
    float point2_red;
    float point2_green;
    float point2_blue;
    float point3_red;
    float point3_green;
    float point3_blue;
    
    double triangle_area;
    double area_factor;
    vector<double> triangle_areas;
   
    if(PLY == true)
    {
#ifdef RUN_TESTS
        //testNumberTriangles(num_element_vertex, size_of_x_main);
#endif
        
        for(int i = 0; i < num_element_face; i++)
        {
            debug("TRIANGLE %d", i);
            
            num_vertices = num_vertices_to_connect_main[i];
            if(num_vertices == 3)
            {
                vertex1_index = vertex1_main[i];
                vertex2_index = vertex2_main[i];
                vertex3_index = vertex3_main[i];
                debug("vertex1_index: %d", vertex1_index);
                debug("vertex2_index: %d", vertex2_index);
                debug("vertex3_index: %d", vertex3_index);
                
                point1_x_coord = x_main[vertex1_index];
                point1_y_coord = y_main[vertex1_index];
                point1_z_coord = z_main[vertex1_index];
                debug("x_main[vertex1_index]: %d", x_main[vertex1_index]);
                debug("y_main[vertex1_index]: %d", y_main[vertex1_index]);
                debug("z_main[vertex1_index]: %d", z_main[vertex1_index]);
                point1.setVectX(point1_x_coord);
                point1.setVectY(point1_y_coord);
                point1.setVectZ(point1_z_coord);
                debug("point1.setVectX(): %d", point1.getVectX());
                debug("point1.setVectY(): %d", point1.getVectY());
                debug("point1.setVectZ(): %d", point1.getVectZ());

                point2_x_coord = x_main[vertex2_index];
                point2_y_coord = y_main[vertex2_index];
                point2_z_coord = z_main[vertex2_index];
                debug("x_main[vertex2_index]: %d", x_main[vertex2_index]);
                debug("y_main[vertex2_index]: %d", y_main[vertex2_index]);
                debug("z_main[vertex2_index]: %d", z_main[vertex2_index]);
                point2.setVectX(point2_x_coord);
                point2.setVectY(point2_y_coord);
                point2.setVectZ(point2_z_coord);
                debug("point2.setVectX(): %d", point2.getVectX());
                debug("point2.setVectY(): %d", point2.getVectY());
                debug("point2.setVectZ(): %d", point2.getVectZ());

                point3_x_coord = x_main[vertex3_index];
                point3_y_coord = y_main[vertex3_index];
                point3_z_coord = z_main[vertex3_index];
                debug("x_main[vertex3_index]: %d", x_main[vertex3_index]);
                debug("y_main[vertex3_index]: %d", y_main[vertex3_index]);
                debug("z_main[vertex3_index]: %d", z_main[vertex3_index]);
                point3.setVectX(point3_x_coord);
                point3.setVectY(point3_y_coord);
                point3.setVectZ(point3_z_coord);
                debug("point3.setVectX(): %d", point3.getVectX());
                debug("point3.setVectY(): %d", point3.getVectY());
                debug("point3.setVectZ(): %d", point3.getVectZ());

                //IF R/G/B ARE OUT OF 255, CONVERT TO OUT OF 1
                if(red_main[vertex1_index] > 1.0)
                {
                    point1_red = red_main[vertex1_index] / 256;
                }
                else
                {
                    point1_red = red_main[vertex1_index];
                }
                
                if(green_main[vertex1_index] > 1.0)
                {
                    point1_green = green_main[vertex1_index] / 256;
                }
                else
                {
                    point1_green = green_main[vertex1_index];
                }
                
                if(blue_main[vertex1_index] > 1.0)
                {
                    point1_blue = blue_main[vertex1_index] / 256;
                }
                else
                {
                    point1_blue = blue_main[vertex1_index];
                }
                
                if(red_main[vertex2_index] > 1.0)
                {
                    point2_red = red_main[vertex2_index] / 256;
                }
                else
                {
                    point2_red = red_main[vertex2_index];
                }
                
                if(green_main[vertex2_index] > 1.0)
                {
                    point2_green = green_main[vertex2_index] / 256;
                }
                else
                {
                    point2_green = green_main[vertex2_index];
                }
                
                if(blue_main[vertex2_index] > 1.0)
                {
                    point2_blue = blue_main[vertex2_index] / 256;
                }
                else
                {
                    point2_blue = blue_main[vertex2_index];
                }
                
                if(red_main[vertex3_index] > 1.0)
                {
                    point3_red = red_main[vertex3_index] / 256;
                }
                else
                {
                    point3_red = red_main[vertex3_index];
                }
                
                if(green_main[vertex3_index] > 1.0)
                {
                    point3_green = green_main[vertex3_index] / 256;
                }
                else
                {
                    point3_green = green_main[vertex3_index];
                }
                
                if(blue_main[vertex3_index] > 1.0)
                {
                    point3_blue = blue_main[vertex3_index] / 256;
                }
                else
                {
                    point3_blue = blue_main[vertex3_index];
                }

                //COLOR
                point1_color.setColorRed(point1_red);
                point1_color.setColorGreen(point1_green);
                point1_color.setColorBlue(point1_blue);
                point2_color.setColorRed(point2_red);
                point2_color.setColorGreen(point2_green);
                point2_color.setColorBlue(point2_blue);
                point3_color.setColorRed(point3_red);
                point3_color.setColorGreen(point3_green);
                point3_color.setColorBlue(point3_blue);
                
                tri_red_average = (point1_color.getColorRed() + point2_color.getColorRed() + point3_color.getColorRed()) / 3;
                tri_green_average = (point1_color.getColorGreen() + point2_color.getColorGreen() + point3_color.getColorGreen()) / 3;
                tri_blue_average = (point1_color.getColorBlue() + point2_color.getColorBlue() + point3_color.getColorBlue()) / 3;
                
                triangle_color.setColorRed(tri_red_average);
                triangle_color.setColorGreen(tri_green_average);
                triangle_color.setColorBlue(tri_blue_average);
                
                triangle_area = ((point2 - point1) ^ (point3 - point1)).length() * 0.5;
                debug("triangle_area: %d", triangle_area);
                triangle_areas.push_back(triangle_area);
                
                scene_triangles.push_back(new Triangle(point1, point2, point3, triangle_color));
                
            } //end of if (num_vertices == 3)
            //FIX: change once set up to do things other than triangles (if necessary)
            else
            {
                printf("Cannot construct a triangle from given information\n");
                return 0;
            }
       } //end of for loop
    }//end of if(PLY == true)
    else
    {
        //FIX: COME BACK AND CREATE TRIANGLES THE SAME WAY AS IN TRIANGLES FROM PLY FILES
        //TXT FILES
        for(int i = 0; i < x1_main.size(); i++)
        {
            Vect point1(x1_main[i], y1_main[i], z1_main[i]);
            Vect point2(x2_main[i], y2_main[i], z2_main[i]);
            Vect point3(x3_main[i], y3_main[i], z3_main[i]);
            
            cout << "point1: " << point1.getVectX() << " " << point1.getVectY() << " " << point1.getVectZ() << endl;
            Triangle newTri(point1, point2, point3, leafID, leafL, position, CLAI, leafTransmittance, leafReflectivity, n_per_area, start, end, intervalHour);

            //scene_shapes.push_back(dynamic_cast<Shape*>(&newTri));
        }
    }
    cout << "scene_triangles.size(): " << scene_triangles.size() << endl;
    debug("Number of shapes in scene: %d", scene_triangles.size());
 
    //CALCULATE PPFD FROM ATMOSPHERIC TRANSMITTANCE
    //climate.climate_calculation_PPFD(cs.LATITUDE, stn, cs.ATMOSPHERIC_TRANSMITTANCE, DOY_main, start, end, interval, ps, cs);
    
    vector<double> area_total;
    vector<double> PPFD_totals;
    
    for(int i = 0; i < triangle_areas.size(); i++)
    {
        debug("triangle_area[%d]: %d", i, triangle_area[i]);
    }
    
    double area_factor_temp;
    vector<double> area_factors;
    //CALCULATE AREA_FACTOR TO CALCULATE PPFD LATER
    for(int i = 0; i < triangle_areas.size(); i++)
    {
        area_factor_temp =  1 / (triangle_areas[i] * 1e-4);
        area_factors.push_back(area_factor_temp);
    }
    
    int testing_timestep = 0;
    double individual_ppfd = 0;

    //RETURN COLOR PER PIXEL
    for(int timestep = 0; timestep < num; timestep++)
    {
        cout << "timestep = " << timestep << endl;
        debug("timestep: %d", timestep);
        
        double hour = start + timestep * interval;
        cout << "hour = " << hour << endl;
        debug("hour: %d", hour);
        
        //area_total.clear();
        //PPFD_total.clear();
        //double light_d_x = climate.direct_light_d_list[timestep].x;
        //double light_d_y = climate.direct_light_d_list[timestep].y;
        //double light_d_z = climate.direct_light_d_list[timestep].z;
        
        //double direct_light_ppfd = climate.ppfd_direct_light[timestep];
        //FIX: MAKE DIFFUSE STUFF WORK
        //double diffuse_light_ppfd = climate.ppfd_diffuse_light[timestep];
        
        //double dir_pf = direct_light_ppfd * 1e-4; //ADD IN LIGHT_NEAREST_DISTANCE
        //double dif_pf = diffuse_light_ppfd * 1e-4; //ADD IN LIGHT_NEAREST_DISTANCE

        //TRIANGLES ARE IN SCENE_SHAPES ALREADY
        /*vector<Shape*>::iterator it;
        for(it = scene_shapes.begin(); it != scene_shapes.end(); it++)
        {
            double triangle_area = (((*it)->B - (*it)->A) ^ ((*it)->C - (*it)->A)).length() * 0.5;
            double area_factor = 1 / (triangle_area * 1e-4);
            
            vector<double> photonFlux_up_dir = (*it)->photonFlux_up_dir;
            //vector<double> photonFlux_up_diff = (*it)->photonFlux_up_diff;
            //vector<double> photonFlux_up_scat = (*it)->photonFlux_up_scat;
            //vector<double> photonFlux_down_dir = (*it)->photonFlux_down_dir;
            //vector<double> photonFlux_down_diff = (*it)->photonFlux_down_diff;
            //vector<double> photonFlux_down_scat = (*it)->photonFlux_down_scat;
            
            //SELECT PPFD FOR GIVEN TIME
            ////////////double it1 = photonFlux_up_dir[timestep];
            //double it2 = photonFlux_up_diff[timestep];
            //double it3 = photonFlux_up_scat[timestep];
            //double it4 = photonFlux_down_dir[timestep];
            //double it5 = photonFlux_down_diff[timestep];
            //double it6 = photonFlux_down_scat[timestep];
            
            // TOTAL PPFD FROM DIRECT, DIFFUSE, AND SCATTERED LIGHT
            //double PPFD_tot = (it1 + it2 + it3 + it4 + it5 + it6) * area_factor;
            //double PPFD_tot = it1 * area_factor;
            
            //area_total.push_back(triangle_area);
            //PPFD_total.push_back(PPFD_tot);
        }*/
        valarray<float> PPFD_dir_light_per_triangle(0., width * height);
        int test_for_greater_than_minus_1 = 0;
        
        float tNear = INFINITY;
        int anti_aliasing_index;
        double tempRed;
        double tempGreen;
        double tempBlue;
#ifdef RUN_TESTS
        //testNumberRays(num_pixels, red_light, width, height, accuracy, ambient_light);
        //testNumberRays_antiAliasing(num_pixels, red_light, width, height, accuracy, ambient_light, anti_aliasing_depth);
        
        //testNumberRays_antiAliasing_cube(16, red_light, 4, 4, accuracy, ambient_light, anti_aliasing_depth);
        //testRayDistance(width, height, red_light, white_light);
//        testBoundingBox(num_pixels, red_light, width, height, accuracy, ambient_light);
#endif

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
                        //cout << "current_pixel: " << current_pixel << endl;
                        for(int index = 0; index < scene_triangles.size(); index++)
                        {
                            //intersections.push_back(scene_triangles.at(index)->findIntersection(camera_ray));
                            intersections.push_back(scene_triangles.at(index)->intersect(camera_ray, tNear));
                            debug("intersections: %d", intersections[index]);
                            //cout << "intersections[" << current_pixel << "]: " << intersections[index] << endl;
                            if(intersections[index] > -1)
                            {
                                test_for_greater_than_minus_1++;
                            }
                        }
                        
                        int index_of_closest_shape = closestShapeIndex(intersections);
                        debug("index_of_closest_shape: %d", index_of_closest_shape);
                        
                        if(index_of_closest_shape == -1)
                        {
                            //SET BACKGROUND OF IMAGE
                            tempRed[anti_aliasing_index] = 0;
                            tempGreen[anti_aliasing_index] = 0;
                            tempBlue[anti_aliasing_index] = 0;
                            debug("Set pixels[%d] to black", pixels[current_pixel]);
                            PPFD_dir_light_per_triangle[current_pixel] = 0.;
                        }
                        else
                        {
                            //INDEX CORRESPONDS TO SHAPE IN SCENE
                            if(intersections.at(index_of_closest_shape) > accuracy)
                            {
                                climate.calculate_PPFD_direct_light(current_pixel, ref(PPFD_dir_light_per_triangle), cs, ref(individual_ppfd));
                                debug("intersections[%d] > accuracy", intersections.at(index_of_closest_shape));
                                
                                //DETERMINE POSITION AND DIRECTION VECTORS AT POINT OF INTERSECTION
                                Vect intersection_ray_position = camera_ray_origin.vectAdd(camera_ray_direction.vectMult(intersections.at(index_of_closest_shape)));
                                
                                Vect intersection_ray_direction = camera_ray_direction;
                                
                                if(shadowed == true)
                                {
                                    Color intersection_color = getColorAt(intersection_ray_position, intersection_ray_direction, scene_triangles, index_of_closest_shape, scene_lights, accuracy, ambient_light, shadowed);
                                    
                                    tempRed[anti_aliasing_index] = intersection_color.getColorRed();
                                    tempGreen[anti_aliasing_index] = intersection_color.getColorGreen();
                                    tempBlue[anti_aliasing_index] = intersection_color.getColorBlue();
                                    debug("color of pixel[%d] is: [%d][%d][%d]", pixels[current_pixel], intersection_color.getColorRed(), intersection_color.getColorGreen(), intersection_color.getColorBlue());
                                }
                                else
                                {
                                    Color current_color = scene_triangles.at(index_of_closest_shape)->getColor();
                                    
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
        
        cout << "PPFD_dir_light_per_triangle SUM: " << PPFD_dir_light_per_triangle.sum() << endl;
        cout << "test_for_greater_than_minus_1: " << test_for_greater_than_minus_1 << endl;
        
        //FIX: make dynamic for more than one triangle and PPFD
        area_factor = 1 / (triangle_areas[1] * 1e-4);
        double PPFD_total = PPFD_dir_light_per_triangle.sum() * area_factor;
        PPFD_totals.push_back(individual_ppfd);
        //for(int i = 0; i < PPFD_dir_light_per_triangle.size(); i++)
        //{
        //    cout << "PPFD_dir_light_per_triangle[" << i << "]: " << PPFD_dir_light_per_triangle[i] << endl;
        //}
        for(int i = 0; i < PPFD_totals.size(); i++)
        {
            cout << "PPFD_totals[" << i << "]: " << PPFD_totals[i] << endl;
        }
        
        savebmp("test.bmp", width, height, dpi, pixels);
        debug("saved image");
        
        //OUTPUT FILE
        //FIX: IF CLA OUTPUT FILE NAME HAS FILE EXTENSION, REMOVE IT
        std::ostringstream output_string;
        output_string<<cla.outputFile<<"_"<<hour<<"_hour"<<fileType;
        string output_file_name = output_string.str();
        
        cout << "output_file: " << output_file_name << endl;
        writePPFDFile(output_file_name, triangle_areas, PPFD_totals, num);
        
        testing_timestep++;
        
    }//END TIMESTEP

#ifdef RUN_TESTS
    //testNumberTimesteps(startHour, endHour, interval, testing_timestep);
#endif
    
    delete[] pixels;
    
    total_time_end = clock();
    
    float total_time = ((float)total_time_end - (float)total_time_begin) / 1000000;
    cout << "Total time: " << total_time << " seconds" << endl;
    
    endTimer(0, totalTimer, 1, "main");

    //SAVE TIMINGS TO FILE
    const char *filename = "timing.txt";
    saveTimers(filename);
    
    debug("end totalTimer");
    
    cout << "TRIANGLE HITS:   " << Triangle::triangle_hits << endl;
    cout << "TRIANGLE MISSES: " << Triangle::triangle_misses << endl;
    cout << "BBOX HITS:       " << Triangle::bbox_hits << endl;
    cout << "BBOX MISSES:     " << Triangle::bbox_misses << endl;

    cout << "EXIT SUCCESSFUL" << endl;

    return 0;
} //end of main()


