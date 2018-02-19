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

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "commandArgs.hpp"
#include "io.hpp"
#include "ray.hpp"
#include "reader.hpp"
//#include "camera.hpp"
#include "climate.hpp"
//#include "color.hpp"
//#include "compound.hpp"
//#include "equations.hpp"
#include "constants.hpp"
#include "leaf.hpp"
//#include "light.hpp"
//#include "scene.hpp"
//#include "stomata.hpp"
#include "triangle.hpp"
#include "normal.hpp"
#include "grid.hpp"
#include "writer.hpp"

using namespace std;
using namespace raytrace;

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

    readMeanFile(cla.meansFile, ref(branch_ID_main), ref(node_main), ref(branch_angle_main), ref(pet1_angle_main), ref(pet2_angle_main), ref(mid_if_angle_main), ref(left_angle_main), ref(right_angle_main), ref(middle_angle_main), ref(internode_main), ref(pet1_main), ref(pet2_main), ref(lat1L_main), ref(lat1W_main), ref(lat2L_main), ref(lat2W_main), ref(midL_main), ref(midW_main));
    readModelFile(cla.threeDModelFile, ref(x1_main), ref(y1_main), ref(z1_main), ref(x2_main), ref(y2_main), ref(z2_main), ref(x3_main), ref(y3_main), ref(z3_main), ref(leafID_main), ref(leafL_main), ref(position_main), ref(chlSPAD_main), ref(kt_main), ref(kr_main), ref(n_per_area_main), ref(facet_area_main), ref(seven_h_main));
    readSTDFile(cla.stdFile, ref(branch_ID_std_main), ref(node_std_main), ref(branch_angle_std_main), ref(pet1_angle_std_main), ref(pet2_angle_std_main), ref(mid_if_angle_std_main), ref(left_angle_std_main), ref(right_angle_std_main), ref(middle_angle_std_main), ref(internode_std_main), ref(pet1_std_main), ref(pet2_std_main), ref(lat1L_std_main), ref(lat1W_std_main), ref(lat2L_std_main), ref(lat2W_std_main), ref(midL_std_main), ref(midW_std_main));
    readVertexFile(cla.vxFile, ref(DOY_main), ref(vertex_mean_for_stem_main), ref(vertex_std_for_stem_main), ref(sensescense_add_main), ref(ambient_vc_max_mean_main), ref(ambient_jmax_mean_main), ref(vertex_br1_main), ref(vertex_br2_main), ref(vertex_br3_main), ref(vertex_br4_main), ref(vertex_br5_main), ref(vertex_br6_main));
    double startHour = cla.startHour;
    double endHour = cla.endHour;
    double intervalHour = cla.intervalHour;
    double rayDistance = cla.rayDistance;
    double leafTransmittance = cla.leafTransmittance;
    double leafReflectivity = cla.leafReflectivity;
    
    Parameters ps;
    Constants cs;
    
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
    
    //DRAW A-Q CURVES UNDER THREE CO2 CONDITIONS
    char X[] = "PPFD";
    double x_start = 1;
    double x_end = 2001;
    char Y[] = "A";
    char T[] = "CO2";
    double t[] = {100, 370, 550};
    Leaf lc = Leaf();
    
    double *Y_ptr = new double[150];
    Y_ptr = lc.runCurves(cs, ps, X, x_start, x_end, Y, T, t);
    
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
    
    grid->setup_cells(Point(xMin, yMin, zMin), Point(xMax, yMax, zMax));
    
    int num = (int)((end - start) / interval);
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

    }

    //LEAF PHOTOSYNTHESIS
    Leaf lps;
    lps.run_FarquharModel(grid, start, end, interval, ps, cs, climate);
    
    //OUTPUT TO FILE FROM LEAF PHOTOSYNTHESIS
    const char *output_model_file_name = "output_model_file_name";
    writeModelFile(output_model_file_name, num, start, interval, grid, xMax, xMin, yMax, yMin, zMax, zMin);
    
    const char *photosynthesis_rate_file_name = "photosynthesis_rate_file_name";
    
    
    const char *metabolite_concentrations_file_name = "metabolite_concentrations_file_name";
    
    
    cout << "EXIT SUCCESSFUL" << endl;
}
