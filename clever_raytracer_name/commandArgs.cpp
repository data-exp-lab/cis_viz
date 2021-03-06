#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "commandArgs.hpp"

//GLOBAL INSTANCE OF COMMAND LINE ARGUMENTS STRUCTURE
commandLineArgs cla;

//DISPLAY USAGE MESSAGE
inline void DisplayUsage(char *argv[])
{
    fprintf(stderr, "\nUsage:\n");
    fprintf(stderr, "%s -g <geometry file> -o <output file> -f <starting hour> -e <ending hour> -i <interval hour> -l <leaf transmittance> -j <leaf reflectivity> -h \n"
            "  Geometry filename   = -g geometry.txt   \n"
            "  output filename     = -o output.txt     \n"
            "  starting hour       = -f [0, 23]        \n"
            "  ending hour         = -e [0, 23]        \n"
            "  interval hour       = -i 1, 14.5        \n"
            "  leaf transmittance  = -l .9, .5         \n"
            "  leaf reflectivity   = -j .8, .25        \n"
            "  image width         = -w 640            \n"
            "  image height        = -h 480            \n"
            "  anti aliasing       = -a 1, 2, 3        \n"
            "  help                = -z \n"
            "\n\n",
            argv[0]);
    exit(1);
}

const char *commandLineOptions = "g:o:f:e:i:l:j:w:h:a:z";

//PROCESS COMMAND LINE INPUT
void ProcessCommandLine(int argc, char *argv[])
{
    int c;
    
    memset(&cla, 0, sizeof(cla));
    
    optind = 1;
    
    while((c = getopt(argc, argv, commandLineOptions)) != -1)
    {
        switch(c)
        {
            case 'g':
                cla.geometryFile = optarg;
                break;
            case 'o':
                cla.outputFile = optarg;
                break;
            case 'f':
                cla.startHour = atof(optarg);
                break;
            case 'e':
                cla.endHour = atof(optarg);
                break;
            case 'i':
                cla.intervalHour = atof(optarg);
                break;
            case 'l':
                cla.leafTransmittance = atof(optarg);
                break;
            case 'j':
                cla.leafReflectivity = atof(optarg);
                break;
            case 'w':
                cla.imageWidth = atof(optarg);
                break;
            case 'h':
                cla.imageHeight = atof(optarg);
                break;
            case 'a':
                cla.antiAliasingDepth = atof(optarg);
                break;
            case 'z':
                DisplayUsage(argv);
                break;
        }
    }
    
    //CHECK FOR ALL REQUIRED ARGUMENTS
    if(cla.geometryFile == 0 || cla.outputFile == 0 || cla.startHour == 0 || cla.endHour == 0 || cla.intervalHour == 0 || cla.leafTransmittance == 0 || cla.leafReflectivity == 0 || cla.imageWidth == 0 || cla.imageHeight == 0 || cla.antiAliasingDepth == 0)
    {
        DisplayUsage(argv);
    }
}
