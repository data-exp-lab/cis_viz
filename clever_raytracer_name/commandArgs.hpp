
typedef struct
{
    const char *geometryFile;       //input 3D geometry
    const char *outputFile;         //output file
    int day;                        //day of the year to start simulation
    double startHour;               //if not whole day, starting hour of simulation
    double endHour;                 //if not whole day, ending hour of simulation
    double intervalHour;            //how often to update time
    //double rayDistance;           //distance between rays
    double leafTransmittance;       //how much light is let through the leaf
    double leafReflectivity;        //how much light is reflected off the leaf
    int antiAliasingDepth;          //factor for anti aliasing
} commandLineArgs;

void ProcessCommandLine(int arc, char *argv[]);
