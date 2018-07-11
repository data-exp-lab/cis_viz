#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "grid.hpp"
#include "normal.hpp"
#include "ray.hpp"
#include "point.hpp"

using namespace std;

template <typename T>
class Writer
{
private:
    string name;
    vector<T> data;
public:
    
    
};

//write photosynthetic rate

//write metabolites

void writeModelFile(string output_model_file_name, int num, double start, double interval, Grid* grid, double xMax, double xMin, double yMax, double yMin, double zMax, double zMin)
//void writeModelFile(string output_model_file_name, double xMax, double xMin, double yMax, double yMin, double zMax, double zMin)
{
    
    //OUTPUT FROM LEAF PHOTOSYNTHESIS
    ofstream myfile(output_model_file_name);
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
                
                //it1 = photonFlux_up_dir.begin();
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
    
};

void writePhotosyntheticFile(string photosynthetic_file_name, Grid* grid)
{
    ofstream myfile(photosynthetic_file_name);
    if(myfile.is_open())
    {
        myfile.setf(ios::fixed);
        
        double area;
        
        int num_triangles = grid->get_triangles().size();
        vector<Triangle*> v = grid->get_triangles();
        vector<Triangle*>::iterator it;
        
        area = (((*it)->v1 - (*it)->v0) ^ ((*it)->v2 - (*it)->v0)).length() * 0.5;
        
        for(it = v.begin(); it != v.end(); it++)
        {
            //TITLE
            myfile << "Photosynthesis Rate" << "\t";
            
            vector<double> PPFDsat = (*it)->PPFDSAT;
            vector<double>::iterator itPPFDsat;
            
            vector<double> photonFlux_up_dir = (*it)->photonFlux_up_dir;
            vector<double> photonFlux_up_diff = (*it)->photonFlux_up_diff;
            vector<double> photonFlux_up_scat = (*it)->photonFlux_up_scat;
            vector<double> photonFlux_down_dir = (*it)->photonFlux_down_dir;
            vector<double> photonFlux_down_diff = (*it)->photonFlux_down_diff;
            vector<double> photonFlux_down_scat = (*it)->photonFlux_down_scat;
            
            vector<double>::iterator it1;
            vector<double>::iterator it2;
            vector<double>::iterator it3;
            vector<double>::iterator it4;
            vector<double>::iterator it5;
            vector<double>::iterator it6;
            
            double area_factor = 1 / (area * 1e-4);
            
            //itPPFDsat = PPFDsat.begin();
            it1 = photonFlux_up_dir.begin();
            it2 = photonFlux_up_diff.begin();
            it3 = photonFlux_up_scat.begin();
            it4 = photonFlux_down_dir.begin();
            it5 = photonFlux_down_diff.begin();
            it6 = photonFlux_down_scat.begin();
            
            for(itPPFDsat = PPFDsat.begin(); itPPFDsat != PPFDsat.end(); itPPFDsat++)
            {
                //OUTPUT TOTAL PPFD FROM DIRECT, DIFFUSE, AND SCATTERED LIGHT
                myfile << ((*it1) + (*it2) + (*it3) + (*it4) + (*it5) + (*it6)) * area_factor;
                
                it1++;
                it2++;
                it3++;
                it4++;
                it5++;
                it6++;
                itPPFDsat++;
            }

            myfile << "\n";
        }
    }
    
    myfile.close();
    
};

void writeMetaboliteFile()
{
    
};
