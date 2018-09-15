#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

void writePPFDFile(string output_file_name, vector<double> area_total, vector<double> PPFD_total, int num)
{
    //WRITE OUTPUT FILE WITH PPFD INFO
    ofstream myfile(output_file_name);
    
    if(myfile.is_open())
    {
        //COLUMN TITLES
        myfile << "Triangle Area" << "\t" << "PPFD" << "\t";
        myfile << "\n";
        
        for(int i = 0; i <= PPFD_total.size(); i++)
        {
            //myfile << area_total[i] << "\t" << PPFD_total[i] << "\n";
        }
    }

}







