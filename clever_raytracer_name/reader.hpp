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
void readMeanFile(string mean_file_name, vector<float>& branch_ID_main, vector<float>& node_main, vector<float>& branch_angle_main, vector<float>& pet1_angle_main, vector<float>& pet2_angle_main, vector<float>& mid_if_angle_main, vector<float>& left_angle_main, vector<float>& right_angle_main, vector<float>& middle_angle_main, vector<float>& internode_main, vector<float>& pet1_main, vector<float>& pet2_main, vector<float>& lat1L_main, vector<float>& lat1W_main,vector<float>& lat2L_main, vector<float>& lat2W_main, vector<float>& midL_main, vector<float>& midW_main)
{
    const int NUM_COLUMNS = 18;
    
    vector<vector <float> > mean_file_data;
    
    ifstream mean_file(mean_file_name);
    
    //DETERMINE HOW MANY LINES OF DATA ARE IN THE MEANS_FILE
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
    
    branch_ID_main = branch_ID;
    node_main = node;
    branch_angle_main = branch_angle;
    pet1_angle_main = pet1_angle;
    pet2_angle_main = pet2_angle;
    mid_if_angle_main = mid_if_angle;
    left_angle_main = left_angle;
    right_angle_main = right_angle;
    middle_angle_main = middle_angle;
    internode_main = internode;
    pet1_main = pet1;
    pet2_main = pet2;
    lat1L_main = lat1L;
    lat1W_main = lat1W;
    lat2L_main = lat2L;
    lat2W_main = lat2W;
    midL_main = midL;
    midW_main = midW;
};

//MODEL FILE
void readModelFile(string model_file_name, vector<float>& x1_main, vector<float>& y1_main, vector<float>& z1_main, vector<float>& x2_main, vector<float>& y2_main, vector<float>& z2_main, vector<float>& x3_main, vector<float>& y3_main, vector<float>& z3_main/*, vector<float>& leafID_main, vector<float>& leafL_main, vector<float>& position_main, vector<float>& chlSPAD_main, vector<float>& kt_main, vector<float>& kr_main, vector<float>& n_per_area_main, vector<float>& facet_area_main, vector<float>& seven_h_main*/)
{
    const int NUM_COLUMNS = 9;
    
    vector<vector <float> > model_file_data;
    
    ifstream model_file(model_file_name);
    
    //DETERMINE HOW MANY LINES OF DATA ARE IN THE MEANS_FILE
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
    //vector<float> leafID;
    //vector<float> leafL;
    //vector<float> position;
    //vector<float> chlSPAD;
    //vector<float> kt;
    //vector<float> kr;
    //vector<float> n_per_area;
    //vector<float> facet_area;
    //vector<float> seven_h;
    
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
        /*if((column_to_input - 1) == 9)
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
        }*/
        
        column_to_input++;
    }
    
    model_file.close();
    
    x1_main = x1;
    y1_main = y1;
    z1_main = z1;
    x2_main = x2;
    y2_main = y2;
    z2_main = z2;
    x3_main = x3;
    y3_main = y3;
    z3_main = z3;
    //leafID_main = leafID;
    //leafL_main = leafL;
    //position_main = position;
    //chlSPAD_main = chlSPAD;
    //kt_main = kt;
    //kr_main = kr;
    //n_per_area_main = n_per_area;
    //facet_area_main = facet_area;
    //seven_h_main = seven_h;
};

//STD FILE
void readSTDFile(string std_file_name, vector<float>& branch_ID_std_main, vector<float>& node_std_main, vector<float>& branch_angle_std_main, vector<float>& pet1_angle_std_main, vector<float>& pet2_angle_std_main, vector<float>& mid_if_angle_std_main, vector<float>& left_angle_std_main, vector<float>& right_angle_std_main, vector<float>& middle_angle_std_main, vector<float>& internode_std_main, vector<float>& pet1_std_main, vector<float>& pet2_std_main, vector<float>& lat1L_std_main, vector<float>& lat1W_std_main, vector<float>& lat2L_std_main, vector<float>& lat2W_std_main, vector<float>& midL_std_main, vector<float>& midW_std_main)
{
    const int NUM_COLUMNS = 18;
    
    vector<vector <float> > std_file_data;
    
    ifstream std_file(std_file_name);
    
    //DETERMINE HOW MANY LINES OF DATA ARE IN THE MEANS_FILE
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

    branch_ID_std_main = branch_ID_std;
    node_std_main = node_std;
    branch_angle_std_main = branch_angle_std;
    pet1_angle_std_main = pet1_angle_std;
    pet2_angle_std_main = pet1_angle_std;
    mid_if_angle_std_main = mid_if_angle_std;
    left_angle_std_main = left_angle_std;
    right_angle_std_main = right_angle_std;
    middle_angle_std_main = middle_angle_std;
    internode_std_main = internode_std;
    pet1_std_main = pet1_std;
    pet2_std_main = pet2_std;
    lat1L_std_main = lat1L_std;
    lat1W_std_main = lat1W_std;
    lat2L_std_main = lat2L_std;
    lat2W_std_main = lat2W_std;
    midL_std_main = midL_std;
    midW_std_main = midW_std;

};

//VERTEX FILE
//void readVertexFile(string vertex_file_name)
void readVertexFile(string vertex_file_name, vector<float>& DOY_main, vector<float>& vertex_mean_for_stem_main, vector<float>& vertex_std_for_stem_main, vector<float>& sensescense_add_main, vector<float>& ambient_vc_max_mean_main, vector<float>& ambient_jmax_mean_main, vector<float>& vertex_br1_main, vector<float>& vertex_br2_main, vector<float>& vertex_br3_main, vector<float>& vertex_br4_main, vector<float>& vertex_br5_main, vector<float>& vertex_br6_main)
{
    const int NUM_COLUMNS = 12;
    
    vector<vector <float> > vertex_file_data;
    
    ifstream vertex_file(vertex_file_name);
    
    //DETERMINE HOW MANY LINES OF DATA ARE IN THE MEANS_FILE
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
    
    DOY_main = DOY;
    vertex_mean_for_stem_main = vertex_mean_for_stem;
    vertex_std_for_stem_main = vertex_std_for_stem;
    sensescense_add_main = sensescense_add;
    ambient_vc_max_mean_main = ambient_vc_max_mean;
    ambient_jmax_mean_main = ambient_jmax_mean;
    vertex_br1_main = vertex_br1;
    vertex_br2_main = vertex_br2;
    vertex_br3_main = vertex_br3;
    vertex_br4_main = vertex_br4;
    vertex_br5_main = vertex_br5;
    vertex_br6_main = vertex_br6;
    
};
