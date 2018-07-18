/*
 INFO FROM FASTTRACER
 
 *set up command line argumented for whatever is taken in from the command line input stuff
     *importing data -> setup importData function
 
 *climate information
     *atmospheric stuff -> geographic location, time of year to get light intensity as an input (need to calculate that beforehand)
 *photosynthesis information
 *time information
 *set up individual leaf -->build triangles and mesh
 *set up canopy -->build triangles and mesh
 *set up initial ray tracing stuff
 *run ray tracer based on given parameters (direct light, diffuse light, etc.) for given time (full 24 hour day)
 *do calculations for leaf photosynthesis --> run soybean model Matlab file from Venkat
     *Farquhar model for calculations?
     ****calculate direction of sun from time of day information
 *do calcuations for full canopy photosynthesis
     *Farquhar model for calculations?
 *output information to a file
 
 ./fastTracer day hour minX maxX minY maxY minZ maxZ nearestDistance 3DModelFile outputFile leaf_t leaf_r modelType isWholeDay startHour endHour intervalHour silence parameterFile useThermalModel useStomataModel useWeatherDatafile weatherFile
 
 input to raytracing is the first 9 columns --> building triangles and mesh, etc.
 calculate ight intensity of each individual triangle (leaf model) --> use light intensity for each triangle to calculate photosynthesis for each triangle
 photosynthesis information will go back into the metabolic model from leaf model
 -light intensity; area(size) of each piece
 -metabolic stuff related to growth rate (can be taken in once we start doing growth of the system)
 -atmospheric model will be input
 
 */

/*
 * width, height -> in order to actually render an image
 *return a color
 *save the image
 *dpi = 72
 *index_value = y * width + x; -->use to determine the x,y coordinates of individual pixel
 
 
 int aspectRatio = (double)width / (double) height;
 
 *n = total number of pixels in the image (w * h)
 *RGBType *pixels = new RBGType[n];
 /////////
 Vect X(1, 0, 0);
 Vect Y(0, 1, 0);
 Vect Z(0, 0, 1);
 
 //instance of camera
 //direction of the camera: lookAt
 Vect diff_btw(eye.getVectX - lookAt.getVectX, eye.getVectY - lookAt.getVectY, eye.getVectZ - lookAt.getVectZ);
 Vect camPos(3, 1.5, -4); //arbitrarily set to some point in space
 Vect camDir = diff_btw.negative().normalize();
 Vect camRight = Y.crossProduct(camDir).normalize();
 Vect camDown = camRight.crossProduct(camDir);
 Camera scene_camera(camPos, camDir, CamRight, CamDown);
 
 //LIGHT
 Color white_light(1.0, 1.0, 1.0, 0.0);
 Color green_light(0.5, 1.0, 0.5, 0.3);
 Color gray_light(0.5, 0.5, 0.5, 0.0);
 Color black_light(0.0, 0.0, 0.0, 0.0);
 Color blue_light(0.0, 0.0, 1.0, 0.0);
 
 //PUT LIGHT TO THE SIDE
 //NEED TO CHANGE TO BE EQUAL TO WHEREVER THE SUN IS
 Vect light_position(-7, 10, -10);
 Light source(light_position, white_light);
 vector<Source*> light_sources;
 light_sources.push_back(dynamic_cast<Source*>(&source));
 
 //SPHERE
 raytrace::Vect origin(0, 0, 0);
 Sphere sphere_in_scene(origin, 1.0, green_light);
 
 //PLANE
 Plane plane_in_scene(Y, -1, blue_light);
 double x_amount;
 double y_amount;
 
 //STACK OBJECTS SO YOU CAN INDEX THEM (PUT IN ARRAY)
 vector<Object*> scene_objects;
 scene_objects.push_back(dynamic_cast<Object*>(&sphere_in_scene);
 scene_objects.push_back(dynamic_cast<Object*>(&plane_in_scene);
 
 //RAYS
 Vect camera_ray_origin = scene_camera.getCameraPosition();
 Vect camera_ray_direction = camDir.vectAdd(camRight.vectMult(x_amount - 0.5).vectAdd(camDir.vectMult(y_amount - 0.5))).normalize();
 
 Ray camera_ray(camera_ray_origin, camera_ray_direction);
 
 vector<double> intersections;
 
 //INSIDE OF FOR LOOPS FOR ITERATING OVER EVERY PIXEL
 for(int object_index = 0; object_index < scene_objects.size(); object_index++)
 {
    intersections.push_back(scene_objects.at(object_index)->findIntersection(camera_ray));
 }
 
 int closestObject = closestObjectIndex(intersections);
 if(closestObject == -1)
 {
     pixels[i].r = 0;
     pixels[i].g = 0;
     pixels[i].b = 0;
 }
 else
 {
    //INDEX CORRESPONDS TO OBJECT IN SCENE
    Color thisColor = scene_objects.at(closestObject)->getColor();
 
     pixels[i].r = thisColor.getColorRed();
     pixels[i].g = thisColor.getColorGreen();
     pixels[i].b = thisColor.getColorBlue();
 }
 //SAME THING AS ABOVE BUT WITH SHADOWS
 if(closestObject == -1)
 {
 pixels[i].r = 0;
 pixels[i].g = 0;
 pixels[i].b = 0;
 }
 else
 {
    if(intersections.at(closestObject) > accuracy)
    {
    //DETERMINE POSITION AND DIRECTION VECTORS AT POINT OF INTERSECTION
     raytrace::Vect intersection_position = camera_ray_origin.vectAdd(camera_ray_direction.vectMult(intersections.at(closestObject)));
     raytrace::Vect intersection_ray_direction = camera_ray_direction;
 
     //INDEX CORRESPONDS TO OBJECT IN SCENE
     //Color thisColor = scene_objects.at(closestObject)->getColor();
    Color intersection_color = getColorAt(intersection_position, intersection_ray_direction, scene_objects, closestObject, light_sources, accuracy, ambientLight);
 
     //pixels[i].r = thisColor.getColorRed();
     //pixels[i].g = thisColor.getColorGreen();
     //pixels[i].b = thisColor.getColorBlue();
     pixels[i].r = intersection_color.getColorRed();
     pixels[i].g = intersection_color.getColorGreen();
     pixels[i].b = intersection_color.getColorBlue();
     }
 }
 
 //QUANTITIES OF LIGHT
 double ambientLight = 0.2;
 double accuracy = 0.000001;
 
 
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <functional>

#include "commandArgs.hpp"
#include "io.hpp"
#include "ray.hpp"
#include "reader.hpp"
#include "camera.hpp"
#include "climate.hpp"
#include "color.hpp"
//#include "compound.hpp"
//#include "equations.hpp"
//#include "constants.hpp"
#include "leaf.hpp"
#include "light.hpp"
//#include "scene.hpp"
//#include "stomata.hpp"
#include "triangle.hpp"
#include "normal.hpp"
#include "grid.hpp"
#include "shape.hpp"
#include "writer.hpp"
#include "object.hpp"

using namespace std;
using namespace raytrace;



struct RGBType
{
    double r;
    double g;
    double b;
};

void savebmp(const char *filename, int w, int h, int dpi, RGBType *data)
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
    
    bmpfileheader[ 2] = (unsigned char)(filesize);
    bmpfileheader[ 3] = (unsigned char)(filesize>>8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);
    
    bmpinfoheader[ 4] = (unsigned char)(w);
    bmpinfoheader[ 5] = (unsigned char)(w>>8);
    bmpinfoheader[ 6] = (unsigned char)(w>>16);
    bmpinfoheader[ 7] = (unsigned char)(w>>24);
    
    bmpinfoheader[ 8] = (unsigned char)(h);
    bmpinfoheader[ 9] = (unsigned char)(h>>8);
    bmpinfoheader[10] = (unsigned char)(h>>16);
    bmpinfoheader[11] = (unsigned char)(h>>24);
    
    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s>>8);
    bmpinfoheader[23] = (unsigned char)(s>>16);
    bmpinfoheader[24] = (unsigned char)(s>>24);
    
    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm>>8);
    bmpinfoheader[27] = (unsigned char)(ppm>>16);
    bmpinfoheader[28] = (unsigned char)(ppm>>24);
    
    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm>>8);
    bmpinfoheader[31] = (unsigned char)(ppm>>16);
    bmpinfoheader[32] = (unsigned char)(ppm>>24);
    
    f = fopen(filename, "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    
    for(int i = 0; i < k; i++)
    {
        RGBType rgb = data[i];
        
        double red = (data[i].r) * 255;
        double green = (data[i].g) * 255;
        double blue = (data[i].b) * 255;
        
        //unsigned char color[3] = {(int)floor(blue), (int)floor(green), (int)floor(red)};
        unsigned char color[3] = {static_cast<unsigned char>(floor(blue)), static_cast<unsigned char>(floor(green)), static_cast<unsigned char>(floor(red))};

        
        fwrite(color, 1, 3, f);
    }
    
    fclose(f);
}

int closestObjectIndex(vector<double> intersections)
{
    //RETURN INDEX OF OBJECT CLOSEST TO CAMERA
    int smallest_index;
    
    if(intersections.size() == 0)
    {
        return -1;
    }
    else if(intersections.size() == 1)
    {
        if(intersections.at(0) > 0)
        {
            //IF INTERSECTION GREATER THAN 0, IS MINIMUM INDEX
            return 0; //index in vector array --> starting at 0
        }
        else
        {
            //INTERSECTION VALUE IS NEGATIVE (RAY MISSED)
            return -1;
        }
    }
    else
    {
        //DETERMINE CLOSEST TO CAMERA
        
        //FIND MAX VALUE IN VECTOR
        double max = 0;
        for(int i = 0; i < intersections.size(); i++)
        {
            if(max < intersections.at(i))
            {
                max = intersections.at(i);
            }
        }
        
        //STARTING AT MAX, FIND MIN POSITIVE VALUE
        if(max > 0)
        {
            //ONLY WANT POSITIVE INTERSECTION POINTS
            for(int j = 0; j < intersections.size(); j++)
            {
                if(intersections.at(j) < 0 && intersections.at(j) <= max)
                {
                    max = intersections.at(j);
                    smallest_index = j;
                }
            }
            
            return smallest_index;
        }
        else
        {
            //ALL INTERSECTIONS WERE NEGATIVE
            return -1;
        }
        
    }
    
}

Color getColorAt(raytrace::Vect intersection_position, raytrace::Vect intersection_ray_direction, vector<Object*> scene_objects, int closestObject, vector<Source*> light_sources, double accuracy, double ambientLight)
{
    Color closestObjectColor = scene_objects.at(closestObject)->getColor();
    raytrace::Vect closestObjectNormal = scene_objects.at(closestObject)->getNormalAt(intersection_position);
    Color final_color = closestObjectColor.colorScalar(ambientLight);
    
    for(int i = 0; i < light_sources.size(); i++)
    {
        raytrace::Vect light_direction = light_sources.at(i)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
        
        float cosine_angle = closestObjectNormal.dotProduct(light_direction);
        
        if(cosine_angle > 0)
        {
            //TEST FOR SHADOWS
            bool shadowed = false;
            
            raytrace::Vect distance_to_light = light_sources.at(i)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
            float distance_to_light_magnitude = distance_to_light.magnitude();
            
            //SHADOWS
            raytrace::Ray shadow_ray(intersection_position, light_sources.at(i)->getLightPosition().vectAdd(intersection_position.negative()).normalize());
            
            vector<double> secondary_intersections;
            for(int o = 0; o < scene_objects.size(); o++)
            {
                if(shadowed == false)
                {
                    secondary_intersections.push_back(scene_objects.at(o)->findIntersection(shadow_ray));
                }
                
            }
            
            for(int c = 0; c < secondary_intersections.size(); c++)
            {
                if(secondary_intersections.at(c) > accuracy)
                {
                    if(secondary_intersections.at(c) <= distance_to_light_magnitude)
                    {
                        shadowed = true;
                    }
                }
                break;
            }
            if(shadowed == false)
            {
                final_color = final_color.colorAdd(closestObjectColor.colorMult(light_sources.at(i)->getLightColor()).colorScalar(cosine_angle));
                
                if(closestObjectColor.getColorSpecial() > 0 && closestObjectColor.getColorSpecial() <= 1)
                {
                    //SPECIAL REFERS TO SHININESS IN THIS CASE: [0, 1]
                    double dot1 = closestObjectNormal.dotProduct(intersection_ray_direction.negative());
                    raytrace::Vect scalar1 = closestObjectNormal.vectMult(dot1);
                    raytrace::Vect add1 = scalar1.vectAdd(intersection_ray_direction);
                    raytrace::Vect scalar2 = add1.vectMult(2);
                    raytrace::Vect add2 = intersection_ray_direction.negative().vectAdd(scalar2);
                    raytrace::Vect reflection_direction = add2.normalize();
                    
                    double specular = reflection_direction.dotProduct(light_direction);
                    if(specular > 0)
                    {
                        specular = pow(specular, 10);
                        final_color = final_color.colorAdd(light_sources.at(i)->getLightColor().colorScalar(specular * closestObjectColor.getColorSpecial()));
                    }
                }
                
            }
            
        }
    }
    
    return final_color.clip();
}

int main(int argc, char *argv[])
{
    extern commandLineArgs cla;
    
    ProcessCommandLine(argc, argv);
    
    //MEAN FILE
    vector<float> branch_ID_main;
    vector<float> node_main;
    vector<float> branch_angle_main;
    vector<float> pet1_angle_main;
    vector<float> pet2_angle_main;
    vector<float> mid_if_angle_main;
    vector<float> left_angle_main;
    vector<float> right_angle_main;
    vector<float> middle_angle_main;
    vector<float> internode_main;
    vector<float> pet1_main;
    vector<float> pet2_main;
    vector<float> lat1L_main;
    vector<float> lat1W_main;
    vector<float> lat2L_main;
    vector<float> lat2W_main;
    vector<float> midL_main;
    vector<float> midW_main;
    
    //MODEL FILE
    vector<float> x1_main;
    vector<float> y1_main;
    vector<float> z1_main;
    vector<float> x2_main;
    vector<float> y2_main;
    vector<float> z2_main;
    vector<float> x3_main;
    vector<float> y3_main;
    vector<float> z3_main;
    vector<float> leafID_main;
    vector<float> leafL_main;
    vector<float> position_main;
    vector<float> chlSPAD_main;
    vector<float> kt_main;
    vector<float> kr_main;
    vector<float> n_per_area_main;
    vector<float> facet_area_main;
    vector<float> seven_h_main;
    
    //STD FILE
    vector<float> branch_ID_std_main;
    vector<float> node_std_main;
    vector<float> branch_angle_std_main;
    vector<float> pet1_angle_std_main;
    vector<float> pet2_angle_std_main;
    vector<float> mid_if_angle_std_main;
    vector<float> left_angle_std_main;
    vector<float> right_angle_std_main;
    vector<float> middle_angle_std_main;
    vector<float> internode_std_main;
    vector<float> pet1_std_main;
    vector<float> pet2_std_main;
    vector<float> lat1L_std_main;
    vector<float> lat1W_std_main;
    vector<float> lat2L_std_main;
    vector<float> lat2W_std_main;
    vector<float> midL_std_main;
    vector<float> midW_std_main;
    
    //VERTEX FILE
    vector<float> DOY_main;
    vector<float> vertex_mean_for_stem_main;
    vector<float> vertex_std_for_stem_main;
    vector<float> sensescense_add_main;
    vector<float> ambient_vc_max_mean_main;
    vector<float> ambient_jmax_mean_main;
    vector<float> vertex_br1_main;
    vector<float> vertex_br2_main;
    vector<float> vertex_br3_main;
    vector<float> vertex_br4_main;
    vector<float> vertex_br5_main;
    vector<float> vertex_br6_main;
    
    //readMeanFile(cla.meansFile, ref(branch_ID_main), ref(node_main), ref(branch_angle_main), ref(pet1_angle_main), ref(pet2_angle_main), ref(mid_if_angle_main), ref(left_angle_main), ref(right_angle_main), ref(middle_angle_main), ref(internode_main), ref(pet1_main), ref(pet2_main), ref(lat1L_main), ref(lat1W_main), ref(lat2L_main), ref(lat2W_main), ref(midL_main), ref(midW_main));
#ifdef CISRUN
    CisAsciiArrayInput in_model("rtr_canopy_structure");
    CisAsciiTableOutput out_environ("rtr_environ", "%lf\t%lf\t%lf\t%lf\t%lf\n");
    while (1) {
        double *x1, *x2, *x3, *y1, *y2, *y3, *z1, *z2, *z3;
        int flag = in_model.recv(9, &x1, &y1, &z1, &x2, &y2, &z2, &x3, &y3, &z3);
        if (flag < 0) {
            printf("raytracer: End of input.\n");
            break;
        }
        printf("raytracer: Received new canopy with %d triangles.\n", flag);
        int irow;
        // This can be done by setting the vector reference if the vectors are of doubles
        for (irow = 0; irow < flag; irow++) {
            x1_main.push_back(x1[irow]);
            y1_main.push_back(y1[irow]);
            z1_main.push_back(z1[irow]);
            x2_main.push_back(x2[irow]);
            y2_main.push_back(y2[irow]);
            z2_main.push_back(z2[irow]);
            x3_main.push_back(x3[irow]);
            y3_main.push_back(y3[irow]);
            z3_main.push_back(z3[irow]);
        }
        free(x1); free(x2); free(x3);
        free(y1); free(y2); free(y3);
        free(z1); free(z2); free(z3);
#else
    readModelFile(cla.threeDModelFile, ref(x1_main), ref(y1_main), ref(z1_main), ref(x2_main), ref(y2_main), ref(z2_main), ref(x3_main), ref(y3_main), ref(z3_main), ref(leafID_main), ref(leafL_main), ref(position_main), ref(chlSPAD_main), ref(kt_main), ref(kr_main), ref(n_per_area_main), ref(facet_area_main), ref(seven_h_main));
#endif
    //readSTDFile(cla.stdFile, ref(branch_ID_std_main), ref(node_std_main), ref(branch_angle_std_main), ref(pet1_angle_std_main), ref(pet2_angle_std_main), ref(mid_if_angle_std_main), ref(left_angle_std_main), ref(right_angle_std_main), ref(middle_angle_std_main), ref(internode_std_main), ref(pet1_std_main), ref(pet2_std_main), ref(lat1L_std_main), ref(lat1W_std_main), ref(lat2L_std_main), ref(lat2W_std_main), ref(midL_std_main), ref(midW_std_main));
    //readVertexFile(cla.vxFile, ref(DOY_main), ref(vertex_mean_for_stem_main), ref(vertex_std_for_stem_main), ref(sensescense_add_main), ref(ambient_vc_max_mean_main), ref(ambient_jmax_mean_main), ref(vertex_br1_main), ref(vertex_br2_main), ref(vertex_br3_main), ref(vertex_br4_main), ref(vertex_br5_main), ref(vertex_br6_main));
    double startHour = cla.startHour;
    double endHour = cla.endHour;
    double intervalHour = cla.intervalHour;
    double rayDistance = cla.rayDistance;
    double leafTransmittance = cla.leafTransmittance;
    double leafReflectivity = cla.leafReflectivity;
        
    //VARIABLES FOR RENDERING IMAGE/DOING SILHOUETTE TESTS
    int DPI = 72;
    int width = 640;
    int height = 480;
    int n = width * height;
    
    RGBType *pixels = new RGBType[n];
    
    double aspectRatio = (double)width / (double) height;
    
    double ambientLight = 0.2;
    double accuracy = 0.000001;
        
    raytrace::Vect origin(0, 0, 0);
    raytrace::Vect X(1, 0, 0);
    raytrace::Vect Y(0, 1, 0);
    raytrace::Vect Z(0, 0, 1);
        
    //TEST SPHERE
        //EVERYTHING HERE IS FOR TESTING PURPOSES
        raytrace::Vect newSphere(1.75, 0, 0);
        //raytrace::Vect newSphere(0, 0, 0);
        
        raytrace::Vect cameraPosition(-3, 1.5, -4);
        
        raytrace::Vect lookAt(0, 0, 0);
        raytrace::Vect differenceBetweenCameraAndLookAt(cameraPosition.getVectX() - lookAt.getVectX(), cameraPosition.getVectY() - lookAt.getVectY(), cameraPosition.getVectZ() - lookAt.getVectZ());
        
        raytrace::Vect cameraDirection = differenceBetweenCameraAndLookAt.negative().normalize();
        raytrace::Vect cameraRight = Y.crossProduct(cameraDirection).normalize();
        raytrace::Vect cameraDown = cameraRight.crossProduct(cameraDirection);
        Camera scene_camera(cameraPosition, cameraDirection, cameraRight, cameraDown);
        
        Color whiteLight(1.0, 1.0, 1.0, 0);
        Color greenLight(0.5, 1.0, 0.5, 0);
        Color blueLight(0.0, 0.0, 1.0, 0);
        Color redLight(1.0, 0.0, 0.0, 0);
        
        //ARBITRARY LIGHT POSITION
        raytrace::Vect lightPosition(-7, 10, -10);
        
        Light sceneLight(lightPosition, whiteLight);
        
        vector<Source*> lightSources;
        lightSources.push_back(dynamic_cast<Source*>(&sceneLight));
        
        raytrace::Vect A(4.528552, 0.063698, -0.076053);
        raytrace::Vect B(4.401397, 0.491405, -0.060543);
        raytrace::Vect C(4.065627, 0.801094, -0.045114);
        
        //SCENE OBJECTS
        Sphere sceneSphere(origin, 1, greenLight);
        Sphere sceneSphere2(newSphere, .5, blueLight);
        Triangle2 sceneTriangle(A, B, C, greenLight);
        
        Plane scenePlane(Y, -1, redLight);
        
        //GATHER ALL OBJECTS INTO A VECTOR
        vector<Object*> sceneObjects;
        sceneObjects.push_back(dynamic_cast<Object*>(&sceneTriangle));

        //sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere));
        //sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere2));
        sceneObjects.push_back(dynamic_cast<Object*>(&scenePlane));
        
        int current;
        double xAmount;
        double yAmount;
        
        for(int x = 0; x < width; x++)
        {
            for(int y = 0; y < height; y++)
            {
                current = y * width + x;
                
                //START WITH NO ANTI-ALIASING
                if(width > height)
                {
                    //IMAGE IS WIDER THAN IT IS TALL
                    xAmount = ((x + 0.5) / width) * aspectRatio - (((width - height) / (double)height) / 2);
                    yAmount = ((height - y) + .5) / height;
                }
                else if(height > width)
                {
                    //IMAGE IS TALLER THAN IT IS WIDE
                    xAmount = (x + .5) / width;
                    yAmount = (((height - y) + .5) / height) / aspectRatio - (((height - width) / (double)width) / 2);
                }
                else
                {
                    //IMAGE IS SQUARE
                    xAmount = (x + .5) / width;
                    yAmount = ((height - y) + .5) / height;
                }
                
                raytrace::Vect camera_ray_origin = scene_camera.getCameraPosition();
                raytrace::Vect camera_ray_direction = cameraDirection.vectAdd(cameraRight.vectMult(xAmount - 0.5).vectAdd(cameraDown.vectMult(yAmount - 0.5))).normalize();
                
                raytrace::Ray cameraRay(camera_ray_origin, camera_ray_direction);
                
                vector<double> intersections;
                for(int index = 0; index < sceneObjects.size(); index++)
                {
                    intersections.push_back(sceneObjects.at(index)->findIntersection(cameraRay));
                }
                int closestObject = closestObjectIndex(intersections);
                if(closestObject == -1)
                {
                    pixels[current].r = 0;
                    pixels[current].g = 0;
                    pixels[current].b = 0;
                }
                else
                {
                    //INDEX CORRESPONDS TO OBJECT IN SCENE
                    Color thisColor = sceneObjects.at(closestObject)->getColor();
                    
                    pixels[current].r = thisColor.getColorRed();
                    pixels[current].g = thisColor.getColorGreen();
                    pixels[current].b = thisColor.getColorBlue();
                }
                
                //IF INDEX CORRESPONDS TO OBJECT IN SCENE
                if(intersections.at(closestObject) > accuracy)
                {
                    //DETERMINE POSITION AND DIRECTION VECTORS AT POINT OF INTERSECTION
                    raytrace::Vect intersection_position = camera_ray_origin.vectAdd(camera_ray_origin.vectMult(intersections.at(closestObject)));
                    raytrace::Vect intersection_ray_direction = camera_ray_direction;
                    
                    Color intersection_Color = getColorAt(intersection_position, intersection_ray_direction, sceneObjects, closestObject, lightSources, accuracy, ambientLight);
                    
                }
                
            }
        }
        
        
        
        savebmp("test.bmp", width, height, DPI, pixels);
        
        
    ///////
    Parameters ps;
    Constants cs;
    cs.NEAREST_RAY_DISTANCE = rayDistance;
    
    //CLIMATE
    Climate climate;
    double stn = cs.TIME_SOLAR_NOON;
    double start = startHour + cs.START_MINUTE / 60;
    double end = endHour + cs.END_MINUTE / 60;
    double interval = intervalHour + cs.INTERVAL_MINUTE / 60;

    int leafID = 1;
    double CLAI = 0.1;
    double PPFD = 1000;
    ps.prepare(leafID, CLAI, PPFD);

    float DOY_val = 450;
    float leafL = 1; // This is a complete guess
    float n_per_area = 1; // This is complete guess
    float position = 0.0;
    //SET FOR EVERY TIMESTEP CALCULATED
    int num = (int)((end - start) / interval);
    for (int i = 0; i <= num; i++)
    {
        DOY_main.push_back(DOY_val);
    }
        
        
    /*//DRAW A-Q CURVES UNDER THREE CO2 CONDITIONS
    char X[] = "PPFD";
    double x_start = 1;
    double x_end = 2001;
    char Y[] = "A";
    char T[] = "CO2";
    double t[] = {100, 370, 550};
    Leaf lc = Leaf();
    
    double *Y_ptr = new double[150];
    
    std::cout << "before runCurves" << std::endl;
    Y_ptr = lc.runCurves(cs, ps, X, x_start, x_end, Y, T, t);
    std::cout << "after runCurves" << std::endl;

    std::cout << "before climate" << std::endl;*/
    
    //CALCULATE FROM ATMOSPHERE TRANSMITTANCE
    climate.climate_calculation_PPFD(cs.LATITUDE, stn, cs.ATMOSPHERE_TRANSMITTANCE, DOY_main, start, end, interval, ps, cs);
    //RUN CLIMATE MODEL FOR TAIR PREDICTION
    climate.climate_calculation_Tair(cs.LATITUDE, stn, cs.ATMOSPHERE_TRANSMITTANCE, DOY_main, start, end, interval, ps, cs);

    //SET UP THE GRID
    Grid* grid = new Grid(cs.ignore_PPFD_threshold * cs.NEAREST_RAY_DISTANCE * cs.NEAREST_RAY_DISTANCE * 1e-4);

    
    double light_nearest_distance = cs.NEAREST_RAY_DISTANCE;
    double xMax = cs.X_MAX;
    double xMin = cs.X_MIN;
    double yMax = cs.Y_MAX;
    double yMin = cs.Y_MIN;
    double zMax = cs.Z_MAX;
    double zMin = cs.Z_MIN;
        
    for (int i = 0; i < x1_main.size(); i++)
    {
        Point a(x1_main[i], y1_main[i], z1_main[i]);
        Point b(x2_main[i], y2_main[i], z2_main[i]);
        Point c(x3_main[i], y3_main[i], z3_main[i]);
        Triangle* iTri = new Triangle(a, b, c, leafID, leafL, position, CLAI,
                                      leafTransmittance, leafReflectivity,
                                      n_per_area, start, end, interval);
        grid->add_triangle(iTri);
    }
    grid->setup_cells(Point(xMin, yMin, zMin), Point(xMax, yMax, zMax));

    //int num = (int)((end - start) / interval);
    for(int i = 0; i <= num; i++)
    {
        double hour = start + i * interval;
        
        double light_d_x = climate.direct_light_d_list[i].x;
        double light_d_y = climate.direct_light_d_list[i].y;
        double light_d_z = climate.direct_light_d_list[i].z;
        
        double direct_light_ppfd = climate.ppfd_direct_list[i];
        double diffuse_light_ppfd = climate.ppfd_diffuse_list[i];
        
        time_t t1 = time(0);
        
        Ray ray;
        double t = cs.kHugeValue;
        double dir_pf = direct_light_ppfd * light_nearest_distance * light_nearest_distance * 1e-4;
        ray.direction = raytrace::Vect(light_d_x, light_d_y, light_d_z);
        ray.photonFlux2 = dir_pf;
        double x;
        double y;
        
        //TRACE RAYS --> DIRECT LIGHT
        double light_max_x_stop = xMax - light_nearest_distance;
        double light_max_y_stop = yMax - light_nearest_distance;
        //DIRECT LIGHT
        int lightType1 = 1;
        
        if(ray.photonFlux2 > 0)
        {
            for(x = xMin; x < light_max_x_stop; x += light_nearest_distance)
            {
                for(y = yMin; y < light_max_y_stop; y += light_nearest_distance)
                {
                    ray.origin = Point(x, y, zMax);
                    
                    grid->hit(ray, t, i, lightType1, cs);
                }
            }
        }
        
        t1 = time(0);
        
        double dif_pf = diffuse_light_ppfd * light_nearest_distance * light_nearest_distance * 1e-4;
        ray.photonFlux2 = dif_pf;
        
        //TRACE RAYS --> DIFFUSE LIGHT
        light_max_x_stop = xMax - light_nearest_distance;
        light_max_y_stop = yMax - light_nearest_distance;
        //DIFFUSE LIGHT
        int lightType2 = 2;
        
        if(ray.photonFlux2 > 0)
        {
            for(x = xMin; x < light_max_x_stop; x += light_nearest_distance)
            {
                for(y = yMin; y < light_max_y_stop; y += light_nearest_distance)
                {
                    ray.origin = Point(x, y, zMax);
                    ray.direction = raytrace::Vect(true);
                    grid->hit(ray, t, i, lightType2, cs);
                }
            }
        }
        
        t1 = time(0);

//#ifdef CISRUN
        // langmm: Output light intensity (PPFD?) for each triangle
        vector<Triangle*> v = grid->get_triangles();
        vector<Triangle*>::iterator it;
        for(it = v.begin(); it != v.end(); it++) {
            double area = (((*it)->v1 - (*it)->v0) ^ ((*it)->v2 - (*it)->v0)).length() * 0.5;
            double area_factor = 1 / (area * 1e-4);
            
            vector<double> photonFlux_up_dir = (*it)->photonFlux_up_dir;
            vector<double> photonFlux_up_diff = (*it)->photonFlux_up_diff;
            vector<double> photonFlux_up_scat = (*it)->photonFlux_up_scat;
            vector<double> photonFlux_down_dir = (*it)->photonFlux_down_dir;
            vector<double> photonFlux_down_diff = (*it)->photonFlux_down_diff;
            vector<double> photonFlux_down_scat = (*it)->photonFlux_down_scat;
            // Select the time we are on
            double it1 = photonFlux_up_dir[i];
            double it2 = photonFlux_up_diff[i];
            double it3 = photonFlux_up_scat[i];
            double it4 = photonFlux_down_dir[i];
            double it5 = photonFlux_down_diff[i];
            double it6 = photonFlux_down_scat[i];
            // TOTAL PPFD FROM DIRECT, DIFFUSE, AND SCATTERED LIGHT
            double PPFD_tot = (it1 + it2 + it3 + it4 + it5 + it6) * area_factor;
            printf("raytracer: area = %lf, PPFD_tot = %lf\n", area, PPFD_tot);
             //printf("\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n", it1, it2, it3, it4, it5, it6);
            // langmm: These are hard coded, but should be moved to separate file
            double CO2 = 280.0;
            double GRNC = 1.0;
            double GRNT = 0.0;
           /* int flag = out_environ.send(1, CO2, PPFD_tot, climate.TAIR[i], GRNC, GRNT);
            if (flag < 0) {
                printf("raytracer: Error sending output light intensity.");
                error_code = -1;
                break;
            }*/
        }
//#endif
 //   }
//#ifdef CISRUN
    }  // Close the while loop
//#endif

        
        
        
    /*//LEAF PHOTOSYNTHESIS
    Leaf lps;
    lps.run_FarquharModel(grid, start, end, interval, ps, cs, climate);
    std::cout << "after farquharmodel" << std::endl;

    std::cout << "before writing output" << std::endl;

    //OUTPUT TO FILE FROM LEAF PHOTOSYNTHESIS
    //const char *output_model_file_name = "output_model_file_name";
    //writeModelFile(output_model_file_name, xMax, xMin, yMax, yMin, zMax, zMin);
    std::cout << "after writing output" << std::endl;
*/
    /*writeModelFile(output_model_file_name, num, start, interval, grid, xMax, xMin, yMax, yMin, zMax, zMin);
    */
    /*const char *photosynthesis_rate_file_name = "photosynthesis_rate_file_name";
    writePhotosyntheticFile(photosynthesis_rate_file_name, grid);
    */
    
    //const char *metabolite_concentrations_file_name = "metabolite_concentrations_file_name";
    
    
    cout << "EXIT SUCCESSFUL" << endl;
}
