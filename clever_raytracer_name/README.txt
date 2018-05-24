====INTRODUCTION====


This raytracer is a part of the larger Crops in Silico project, which is aimed at utilizing the combination of various models related to plants to analyze growth based on various combinations of parameters such as amount of sunlight, time of year, nitrogen uptake, amount of carbon, etc. Finding the optimal combination of parameters should provide the best scenario to grow plants with the maximum nutrient capacity.

The goal of clever_raytracer_name is to not only plot out where individual plants are within the worldspace, but also calculate the light intensity at each given point. In the context of the Crops in Silico framework, this information will then get passed to other models to do additional calculations, but it can be used as a stand alone piece of information as well.

====CLEVER_RAYTRACER_NAME====

clever_raytracer_name is one of many models that can be used in combination from the Crops in Silico project. While you can pick and choose which models you use, clever_raytracer_name fits in between the metabollic model and the visualization model that utilizes LPy to render images and update plant geometry. While this is in the context of the Crops in Silico project, clever_raytracer_name can be run on its own as long as the necessary input files are provided.

====COMPILING AND RUNNING THE RAYTACER====

clever_raytracer_name relies on a Makefile to be compiled. A CMakeLists.txt file is provided for the user, making CMake a required piece of software in order to run clever_raytracer_name.

You can either build your files in a separate build folder or simply in the larger clever_raytracer_name folder. Simply typing 'cmake .' will compile the necessary files.

The following are the necessary commands to build clever_raytracer_name:

cmake .
make

This will generate an executable called cis_ray. To run the raytracer itself. In order to run, it is necessary to pass several command line arguments as well. These are listed as follows:

./cis_ray -a <3D model file> -m <means file> -s <std filename> -v <vertex file> -c <polynomial file> -p <parameter file> -o <output file> -d <day of year> -w <whole day> -f <starting hour> -e <ending hour> -i <interval hour> -r <ray distance> -b <minXYZ> -g <maxXYZ> -l <leaf transmittance> -j <leaf reflectivity> -h 
  3D model filename   = -a model.txt      
  means filename      = -m means.txt      
  std filename        = -s std.txt        
  vertice filename    = -v vertices.txt   
  output filename     = -o output.txt     
  starting hour       = -f [0, 23]        
  ending hour         = -e [0, 23]        
  interval hour       = -i 1, 14.5        
  distance from rays  = -r .2, 4          
  leaf transmittance  = -l .9, .5         
  leaf reflectivity   = -j .8, .25        
  help                = -h 

At this point, the only required pieces of information are the 3D model file, the output file, the starting hour, ending hour, interval hour, the distance between rays, the leaf transmittance, and the leaf reflectivity. Examples of reasonable values for these parameters are listed in the table above. Example files for the means, stds, and vertices are also provided in this folder, but are not required or used at this present time. 

As a stand alone system, clever_raytracer_name only takes in a .txt file, but in the context of the Crops in Silico framework as a whole, it can take in .ply, .obj, .txt, etc.

====OUTPUT====

The output of clever_raytracer_name is a single .txt file that contains a single column of information, which is the light intensity for every input triangle that was provided from the 3D model file. 


====MISCELLANEOUS====

Questions, comments, or concerns can be directed to Colleen Heinemann at heinmnn2@illinois.edu



