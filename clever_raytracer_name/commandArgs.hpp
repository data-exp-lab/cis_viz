/*
 * Set up the command line arguments necessary to run the ray tracer for the canopy and leaf models
 
 ./fastTracer day hour minX maxX minY maxY minZ maxZ nearestDistance 3DModelFile outputFile leaf_t leaf_r modelType isWholeDay startHour endHour intervalHour silence parameterFile useThermalModel useStomataModel useWeatherDatafile weatherFile
 
     COMMANDLINE ARGUMENTS
 *   Day
 *   Hour
 *   isWholeDay
 *   startHour
 *   endHour
 *   intervalHour
 *   MinX
 *   MaxX
 *   MinY
 *   MaxY
 *   MinZ
 *   MaxZ
 *   nearestDistance
 *   3DModelFile
 *   meansFile
 *   stdFile
 *   vxFile
 *   polynomialsFile
 *   parameterFile
 *   outputFile
 *    leafTransmittance
 *    leafReflectivity
 */

typedef struct
{
    const char *threeDModelFile;    //input 3DModelFile
    const char *meansFile;          //input with information to generate triangles
    const char *stdFile;            //input with information to generate triangles
    const char *vxFile;             //input for vertices
    const char *outputFile;         //output file to write out information
    int day;                        //starting day of simulation
    double startHour;               //if not whole day, starting hour of simulation
    double endHour;                 //if not whole day, ending hour of simulation
    double intervalHour;            //how often to update the time
    double rayDistance;             //distance between rays
    double leafTransmittance;       //how much light is transmitted through leaf
    double leafReflectivity;        //how much light is reflected off of leaf
    
} commandLineArgs;

void ProcessCommandLine(int argc, char *argv[]);
