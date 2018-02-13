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
#include "point.hpp"
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

using namespace std;
using namespace raytrace;

int main(int argc, char *argv[])
{
    extern commandLineArgs cla;
    
    ProcessCommandLine(argc, argv);
    
    readModelFile(cla.threeDModelFile);
    readMeanFile(cla.meansFile);
    readSTDFile(cla.stdFile);
    readVertexFile(cla.vxFile);
    double startHour = cla.startHour;
    double endHour = cla.endHour;
    double intervalHour = cla.intervalHour;
    double rayDistance = cla.rayDistance;
    double leafTransmittance = cla.leafTransmittance;
    double leafReflectivity = cla.leafReflectivity;
    
    //LINK STUFF FROM INPUT FILES TO HERE
    
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
    //CHECK INPUTS HERE->where are they coming from? Need ps?
    climate.climate_calculation_PPFD(cs.LATITUDE, stn, cs.ATMOSPHERE_TRANSMITTANCE, DOY, start, end, interval, cs);
    //RUN CLIMATE MODEL FOR TAIR PREDICTION
    climate.climate_calculation_Tair(cs.LATITUDE, stn, cs.ATMOSPHERE_TRANSMITTANCE, DOY, start, end, interval, cs);
    
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
                    
                    //DOUBLE CHECK -> MIGHT NEED GRID HIT FUNCTION
                    grid->hit(ray, t, i, lightType1);
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
                    grid->hit(ray, t, i, lightType2);
                }
            }
        }
        
        t1 = time(0);

    }

    //LEAF PHOTOSYNTHESIS
    Leaf lps;
    lps.run_FarquharModel(start, end, interval, cs, climate);
    
    //OUTPUT FROM LEAF PHOTOSYNTHESIS
    ofstream myfile(outputFile);
    if(myfile.is_open())
    {
        //TITLES
        myfile << "x1" << "\t" << "y1" << "\t" << "z1" << "\t";
        myfile << "x2" << "\t" << "y2" << "\t" << "z2" << "\t";
        myfile << "x3" << "\t" << "y3" << "\t" << "z3" << "\t";
        myfile << "leafID" << "\t" << "leafL" << "\t" << "pos" << "\t" << "SPAD" << "\t" << "KT" << "\t" << "KR" << "\t" << "NpArea" << "\t" << "facetS" << "\t";
        
        for(int i = 0; i <= num; i++)
        {
            double hour = start + i * interval;
            myfile << hour << "PPFD" << "\t";
            myfile << hour << "Isat" << "\t";
            myfile << hour << "RbcLim" << "\t";
            myfile << hour << "GS" << "\t";
            myfile << hour << "CI" << "\t";
            myfile << hour << "A" << "\t";
        }
        myfile << "\n";
        
        double area;
        int num_triangles = grid->get_triangles().size();
        
        myfile.setf(ios::fixed);
        vector<Triangle*> v = grid->get_triangles();
        vector<Triangle*>::iterator it;
        
        for(it = v.begin(); it != v.end(); it++)
        {
            if((*it)->v0.x > xMin && (*it)->v0.y > yMin && (*it)->v1.x > xMin && (*it)->v1.y > yMin && (*it)->v2.x > xMin && (*it)->v2.y > yMin &&
               (*it)->v0.x < xMax && (*it)->v0.y < yMax && (*it)->v1.x < xMax && (*it)->v1.y < yMax && (*it)->v2.x < xMax && (*it)->v2.y < yMax)
            {
                area = (((*it)->v1 - (*it)->v0) ^ ((*it)->v2 - (*it)->v0)).length() * 0.5;
                
                myfile << setprecision(2) << (*it)->v0.x << "\t"
                << setprecision(2) << (*it)->v0.y << "\t"
                << setprecision(2) << (*it)->v0.z << "\t"
                << setprecision(2) << (*it)->v1.x << "\t"
                << setprecision(2) << (*it)->v1.y << "\t"
                << setprecision(2) << (*it)->v1.z << "\t"
                << setprecision(2) << (*it)->v2.x << "\t"
                << setprecision(2) << (*it)->v2.y << "\t"
                << setprecision(2) << (*it)->v2.z << "\t"
                << setprecision(0) << (*it)->leID << "\t"
                << setprecision(2) << (*it)->leL << "\t"
                << setprecision(2) << (*it)->pos << "\t"
                << setprecision(2) << (*it)->chlSPA << "\t"
                << setprecision(2) << (*it)->kLeafTransmittance << "\t"
                << setprecision(2) << (*it)->kLeafReflectance << "\t"
                << setprecision(2) << (*it)->nitrogenPerA << "\t"
                << setprecision(4) << area << "\t";
                
                vector<double> photonFlux_up_dir = (*it)->photonFlux_up_dir;
                vector<double> photonFlux_up_diff = (*it)->photonFlux_up_diff;
                vector<double> photonFlux_up_scat = (*it)->photonFlux_up_scat;
                vector<double> photonFlux_down_dir = (*it)->photonFlux_down_dir;
                vector<double> photonFlux_down_diff = (*it)->photonFlux_down_diff;
                vector<double> photonFlux_down_scat = (*it)->photonFlux_down_scat;
                
                //LEAF ASSIMILATION RATE
                vector<double> A = (*it)->A;
                vector<double> leafT = (*it)->LEAFT;
                vector<double> GS = (*it)->GS;
                vector<double> CI = (*it)->CI;
                vector<double> PPFDsat = (*it)->PPFDSAT;
                vector<int> isRubiscoLimit = (*it)->isRubiscoLimit;
                
                vector<double>::iterator it1;
                vector<double>::iterator it2;
                vector<double>::iterator it3;
                vector<double>::iterator it4;
                vector<double>::iterator it5;
                vector<double>::iterator it6;
                vector<double>::iterator itA;
                vector<double>::iterator itT;
                vector<double>::iterator itGS;
                vector<double>::iterator itCI;
                vector<double>::iterator itPPFDsat;
                vector<int>::iterator itIsRubiscoLimit;
                
                double area_factor = 1 / (area * 1e-4);
                
                it2 = photonFlux_up_diff.begin();
                it3 = photonFlux_up_scat.begin();
                it4 = photonFlux_down_dir.begin();
                it5 = photonFlux_down_diff.begin();
                it6 = photonFlux_down_scat.begin();
                itA = A.begin();
                itT = leafT.begin();
                itGS = GS.begin();
                itCI = CI.begin();
                itPPFDsat = PPFDsat.begin();
                itIsRubiscoLimit = isRubiscoLimit.begin();
                
                for(it1 = photonFlux_up_dir.begin(); it1 != photonFlux_up_dir.end(); it1++)
                {
                    myfile << setprecision(1) << ( (*it1) + (*it2) + (*it3) + (*it4) + (*it5) + (*it6) ) * area_factor << "\t" << setprecision(1) << (*itPPFDsat) << "\t" << (*itIsRubiscoLimit) << "\t" << setprecision(1) << (*itT) << "\t" << setprecision(2) << (*itGS) << "\t" << setprecision(1) << (*itCI) << "\t" << setprecision(2) << (*itA) << "\t";
                    
                    it2++;
                    it3++;
                    it4++;
                    it5++;
                    it6++;
                    itA++;
                    itT++;
                    itGS++;
                    itCI++;
                    itPPFDsat++;
                    itIsRubiscoLimit++;
                }
                
                myfile << "\n";
                
            }
        }
        myfile.close();
        
    }
    
    cout << "EXIT SUCCESSFUL" << endl;
}
