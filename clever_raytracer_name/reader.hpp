#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>

using namespace std;

class Reader
{
private:
public:
    string name;

    Reader(string file_name) : name(file_name) {};
};

//MEAN FILE
void readMeanFile(string mean_file_name)
{
    const int NUM_COLUMNS = 18;
    
    vector<vector <float> > mean_file_data;
    
    ifstream mean_file(mean_file_name);
    
    //determine how many lines of data are in the means_file
    if(mean_file.is_open())
    {
        float num;
        
        vector<float> numbers_in_line;
        
        while(mean_file >> num)
        {
            numbers_in_line.push_back(num);
            
            if(numbers_in_line.size() == NUM_COLUMNS)
            {
                mean_file_data.push_back(numbers_in_line);
                numbers_in_line.clear();
            }
        }
    }
    
    vector<float> branch_ID;
    vector<float> node;
    vector<float> branch_angle;
    vector<float> pet1_angle;
    vector<float> pet2_angle;
    vector<float> mid_if_angle;
    vector<float> left_angle;
    vector<float> right_angle;
    vector<float> middle_angle;
    vector<float> internode;
    vector<float> pet1;
    vector<float> pet2;
    vector<float> lat1L;
    vector<float> lat1W;
    vector<float> lat2L;
    vector<float> lat2W;
    vector<float> midL;
    vector<float> midW;
    
    int column_to_input = 1;
    vector<float> column;
    
    for(int j = 0; j < NUM_COLUMNS; j++)
    {
        for(int i = 0; i < mean_file_data.size(); i++)
        {
            column.push_back(mean_file_data[i][column_to_input - 1]);
        }
        if((column_to_input - 1) == 0)
        {
            branch_ID = column;
            column.clear();
        }
        if((column_to_input - 1) == 1)
        {
            node = column;
            column.clear();
        }
        if((column_to_input - 1) == 2)
        {
            branch_angle = column;
            column.clear();
        }
        if((column_to_input - 1) == 3)
        {
            pet1_angle = column;
            column.clear();
        }
        if((column_to_input - 1) == 4)
        {
            pet2_angle = column;
            column.clear();
        }
        if((column_to_input - 1) == 5)
        {
            mid_if_angle = column;
            column.clear();
        }
        if((column_to_input - 1) == 6)
        {
            left_angle = column;
            column.clear();
        }
        if((column_to_input - 1) == 7)
        {
            right_angle = column;
            column.clear();
        }
        if((column_to_input - 1) == 8)
        {
            middle_angle = column;
            column.clear();
        }
        if((column_to_input - 1) == 9)
        {
            internode = column;
            column.clear();
        }
        if((column_to_input - 1) == 10)
        {
            pet1 = column;
            column.clear();
        }
        if((column_to_input - 1) == 11)
        {
            pet2 = column;
            column.clear();
        }
        if((column_to_input - 1) == 12)
        {
            lat1L = column;
            column.clear();
        }
        if((column_to_input - 1) == 13)
        {
            lat1W = column;
            column.clear();
        }
        if((column_to_input - 1) == 14)
        {
            lat2L = column;
            column.clear();
        }
        if((column_to_input - 1) == 15)
        {
            lat2W = column;
            column.clear();
        }
        if((column_to_input - 1) == 16)
        {
            midL = column;
            column.clear();
        }
        if((column_to_input - 1) == 17)
        {
            midW = column;
            column.clear();
        }
        
        column_to_input++;
    }
    
    
    mean_file.close();
};

//MODEL FILE
void readModelFile(string model_file_name)
{
    const int NUM_COLUMNS = 18;
    
    vector<vector <float> > model_file_data;
    
    ifstream model_file(model_file_name);
    
    //determine how many lines of data are in the means_file
    if(model_file.is_open())
    {
        float num;
        
        vector<float> numbers_in_line;
        
        while(model_file >> num)
        {
            numbers_in_line.push_back(num);
            
            if(numbers_in_line.size() == NUM_COLUMNS)
            {
                model_file_data.push_back(numbers_in_line);
                numbers_in_line.clear();
            }
        }
    }
    
    //MODEL FILE
    vector<float> x1;
    vector<float> y1;
    vector<float> z1;
    vector<float> x2;
    vector<float> y2;
    vector<float> z2;
    vector<float> x3;
    vector<float> y3;
    vector<float> z3;
    vector<float> leafID;
    vector<float> leafL;
    vector<float> position;
    vector<float> chlSPAD;
    vector<float> kt;
    vector<float> kr;
    vector<float> n_per_area;
    vector<float> facet_area;
    vector<float> seven_h;
    
    int column_to_input = 1;
    vector<float> column;
    
    for(int j = 0; j < NUM_COLUMNS; j++)
    {
        for(int i = 0; i < model_file_data.size(); i++)
        {
            column.push_back(model_file_data[i][column_to_input - 1]);
        }
        if((column_to_input - 1) == 0)
        {
            x1 = column;
            column.clear();
        }
        if((column_to_input - 1) == 1)
        {
            y1 = column;
            column.clear();
        }
        if((column_to_input - 1) == 2)
        {
            z1 = column;
            column.clear();
        }
        if((column_to_input - 1) == 3)
        {
            x2 = column;
            column.clear();
        }
        if((column_to_input - 1) == 4)
        {
            y2 = column;
            column.clear();
        }
        if((column_to_input - 1) == 5)
        {
            z2 = column;
            column.clear();
        }
        if((column_to_input - 1) == 6)
        {
            x3 = column;
            column.clear();
        }
        if((column_to_input - 1) == 7)
        {
            y3 = column;
            column.clear();
        }
        if((column_to_input - 1) == 8)
        {
            z3 = column;
            column.clear();
        }
        if((column_to_input - 1) == 9)
        {
            leafID = column;
            column.clear();
        }
        if((column_to_input - 1) == 10)
        {
            leafL = column;
            column.clear();
        }
        if((column_to_input - 1) == 11)
        {
            position = column;
            column.clear();
        }
        if((column_to_input - 1) == 12)
        {
            chlSPAD = column;
            column.clear();
        }
        if((column_to_input - 1) == 13)
        {
            kt = column;
            column.clear();
        }
        if((column_to_input - 1) == 14)
        {
            kr = column;
            column.clear();
        }
        if((column_to_input - 1) == 15)
        {
            n_per_area = column;
            column.clear();
        }
        if((column_to_input - 1) == 16)
        {
            facet_area = column;
            column.clear();
        }
        if((column_to_input - 1) == 17)
        {
            seven_h = column;
            column.clear();
        }
        
        column_to_input++;
    }
    
    model_file.close();
    
};

//STD FILE
void readSTDFile(string std_file_name)
{
    const int NUM_COLUMNS = 18;
    
    vector<vector <float> > std_file_data;
    
    ifstream std_file(std_file_name);
    
    //determine how many lines of data are in the means_file
    if(std_file.is_open())
    {
        float num;
        
        vector<float> numbers_in_line;
        
        while(std_file >> num)
        {
            numbers_in_line.push_back(num);
            
            if(numbers_in_line.size() == NUM_COLUMNS)
            {
                std_file_data.push_back(numbers_in_line);
                numbers_in_line.clear();
            }
        }
    }
    
    vector<float> branch_ID_std;
    vector<float> node_std;
    vector<float> branch_angle_std;
    vector<float> pet1_angle_std;
    vector<float> pet2_angle_std;
    vector<float> mid_if_angle_std;
    vector<float> left_angle_std;
    vector<float> right_angle_std;
    vector<float> middle_angle_std;
    vector<float> internode_std;
    vector<float> pet1_std;
    vector<float> pet2_std;
    vector<float> lat1L_std;
    vector<float> lat1W_std;
    vector<float> lat2L_std;
    vector<float> lat2W_std;
    vector<float> midL_std;
    vector<float> midW_std;
    
    int column_to_input = 1;
    vector<float> column;
    
    for(int j = 0; j < NUM_COLUMNS; j++)
    {
        for(int i = 0; i < std_file_data.size(); i++)
        {
            column.push_back(std_file_data[i][column_to_input - 1]);
        }
        if((column_to_input - 1) == 0)
        {
            branch_ID_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 1)
        {
            node_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 2)
        {
            branch_angle_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 3)
        {
            pet1_angle_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 4)
        {
            pet2_angle_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 5)
        {
            mid_if_angle_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 6)
        {
            left_angle_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 7)
        {
            right_angle_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 8)
        {
            middle_angle_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 9)
        {
            internode_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 10)
        {
            pet1_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 11)
        {
            pet2_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 12)
        {
            lat1L_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 13)
        {
            lat1W_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 14)
        {
            lat2L_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 15)
        {
            lat2W_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 16)
        {
            midL_std = column;
            column.clear();
        }
        if((column_to_input - 1) == 17)
        {
            midW_std = column;
            column.clear();
        }
        
        column_to_input++;
    }
    
    std_file.close();
};

//VERTEX FILE
void readVertexFile(string vertex_file_name)
{
    const int NUM_COLUMNS = 12;
    
    vector<vector <float> > vertex_file_data;
    
    ifstream vertex_file(vertex_file_name);
    
    //determine how many lines of data are in the means_file
    if(vertex_file.is_open())
    {
        float num;
        
        vector<float> numbers_in_line;
        
        while(vertex_file >> num)
        {
            numbers_in_line.push_back(num);
            
            if(numbers_in_line.size() == NUM_COLUMNS)
            {
                vertex_file_data.push_back(numbers_in_line);
                numbers_in_line.clear();
            }
        }
    }
    
    vector<float> DOY;
    vector<float> vertex_mean_for_stem;
    vector<float> vertex_std_for_stem;
    vector<float> sensescense_add;
    vector<float> ambient_vc_max_mean;
    vector<float> ambient_jmax_mean;
    vector<float> vertex_br1;
    vector<float> vertex_br2;
    vector<float> vertex_br3;
    vector<float> vertex_br4;
    vector<float> vertex_br5;
    vector<float> vertex_br6;
    
    int column_to_input = 1;
    vector<float> column;
    
    for(int j = 0; j < NUM_COLUMNS; j++)
    {
        for(int i = 0; i < vertex_file_data.size(); i++)
        {
            column.push_back(vertex_file_data[i][column_to_input - 1]);
        }
        if((column_to_input - 1) == 0)
        {
            DOY = column;
            column.clear();
        }
        if((column_to_input - 1) == 1)
        {
            vertex_mean_for_stem = column;
            column.clear();
        }
        if((column_to_input - 1) == 2)
        {
            vertex_std_for_stem = column;
            column.clear();
        }
        if((column_to_input - 1) == 3)
        {
            sensescense_add = column;
            column.clear();
        }
        if((column_to_input - 1) == 4)
        {
            ambient_vc_max_mean = column;
            column.clear();
        }
        if((column_to_input - 1) == 5)
        {
            ambient_jmax_mean = column;
            column.clear();
        }
        if((column_to_input - 1) == 6)
        {
            vertex_br1 = column;
            column.clear();
        }
        if((column_to_input - 1) == 7)
        {
            vertex_br2 = column;
            column.clear();
        }
        if((column_to_input - 1) == 8)
        {
            vertex_br3 = column;
            column.clear();
        }
        if((column_to_input - 1) == 9)
        {
            vertex_br4 = column;
            column.clear();
        }
        if((column_to_input - 1) == 10)
        {
            vertex_br5 = column;
            column.clear();
        }
        if((column_to_input - 1) == 11)
        {
            vertex_br6 = column;
            column.clear();
        }
        
        column_to_input++;
    }
    
    vertex_file.close();
    
};
