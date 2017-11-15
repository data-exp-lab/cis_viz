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
//#include "climate.hpp"
//#include "color.hpp"
//#include "compound.hpp"
//#include "equations.hpp"
//#include "constants.hpp"
//#include "leaf.hpp"
//#include "light.hpp"
//#include "scene.hpp"
//#include "stomata.hpp"
//#include "triangle.hpp"

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
    
    cout << "EXIT SUCCESSFUL" << endl;
}
