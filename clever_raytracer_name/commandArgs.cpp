#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "commandArgs.hpp"

//Global instance of the command line arguments structure
commandLineArgs cla;


/********
 * Display usage message
 ********/
inline void DisplayUsage(char *argv[])
{
    fprintf(stderr, "\nUsage:\n");
    fprintf(stderr, "%s -a <3D model file> -m <means file> -s <std filename> -v <vertex file> -c <polynomial file> -p <parameter file> -o <output file> -d <day of year> -w <whole day> -f <starting hour> -e <ending hour> -i <interval hour> -r <ray distance> -b <minXYZ> -g <maxXYZ> -l <leaf transmittance> -j <leaf reflectivity> -h \n"
            "  3D model filename   = -a model.txt      \n"
            "  means filename      = -m means.txt      \n"
            "  std filename        = -s std.txt        \n"
            "  vertice filename    = -v vertices.txt   \n"
            "  output filename     = -o output.txt     \n"
            "  starting hour       = -f [0, 23]        \n"
            "  ending hour         = -e [0, 23]        \n"
            "  interval hour       = -i 1, 14.5        \n"
            "  distance from rays  = -r .2, 4          \n"
            "  leaf transmittance  = -l .9, .5         \n"
            "  leaf reflectivity   = -j .8, .25        \n"
            "  help                = -h \n"
            "\n\n",
            argv[0]);
    
    exit(1);
}

/********
 * Parse 3 values
 ********/
void ParseThreeValues(int values[3], const char *optarg)
{
    char *tk, delimiters[] = ",", opt[100];
    
    strncpy(opt, optarg, sizeof(opt));
    tk = strtok(opt, delimiters);
    values[0] = atoi(tk);
    tk = strtok(NULL, delimiters);
    values[1] = atoi(tk);
    tk = strtok(NULL, delimiters);
    values[2] = atoi(tk);
}

const char *commandLineOptions = "a:m:s:v:o:f:e:i:r:l:j:h";

/********
 * Process command line input
 ********/
void ProcessCommandLine(int argc, char *argv[])
{
    int c;
    
    memset(&cla, 0, sizeof(cla));
    
    //process command line arguments
    optind = 1;
    while((c = getopt(argc, argv, commandLineOptions)) != -1)
    {
        switch(c)
        {
            case 'a':
                cla.threeDModelFile = optarg;
                break;
            case 'm':
                cla.meansFile = optarg;
                break;
            case 's':
                cla.stdFile = optarg;
                break;
            case 'v':
                cla.vxFile = optarg;
                break;
            //case 'o':
            //    cla.outputFile = optarg;
            //    break;
            case 'f':
                cla.startHour = atof(optarg);
                break;
            case 'e':
                cla.endHour = atof(optarg);
                break;
            case 'i':
                cla.intervalHour = atof(optarg);
                break;
            case 'r':
                cla.rayDistance = atof(optarg);
                break;
            case 'l':
                cla.leafTransmittance = atof(optarg);
                break;
            case 'j':
                cla.leafReflectivity = atof(optarg);
                break;
            case 'h':
                DisplayUsage(argv);
                break;
        }
    }
    
    //check for all required arguments
    if(cla.threeDModelFile == 0 || cla.meansFile == 0 || cla.stdFile == 0 || cla.vxFile == 0 /*|| cla.outputFile == 0 ||*/ || cla.startHour == 0 || cla.endHour == 0 || cla.intervalHour == 0 || cla.rayDistance == 0|| cla.leafTransmittance == 0 || cla.leafReflectivity == 0)
    {
        DisplayUsage(argv);
    }
}

