#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <string.h>

using namespace std;

class Reader
{
public:
private:
    string name;
    
    Reader(string file_name) : name(file_name) {};
};

void readGeometryFilePLY2(string geometry_file_name, vector<float>& x_main, vector<float>& y_main, vector<float>& z_main, vector<float>& red_main, vector<float>& green_main, vector<float>& blue_main, vector<float>& num_vertices_to_connect_main, vector<float>& vertex1_main, vector<float>& vertex2_main, vector<float>& vertex3_main)
{
    
    //Read in line of data
    //Determine how many pieces of data there are in the line (either 6 or 4)
        //Max number will be either 6 or 4
        //Should be 6 until hitting connectivity section and the number drops to 4
    //If max number is 6, push line of data into geometry_file_x_y_z_r_g_b
    //Determine size of geometry_file_x_y_z_r_g_b --> equals number of lines with 6 pieces of data
    
    string current_line;
    float number;
    vector<string> tokens;
    
    ifstream geometry_file(geometry_file_name);
    
    if(!geometry_file.is_open())
    {
        cout << "Error opening input file" << endl;
        exit (1);
    }
    
    //FIX: BYPASS THE HEADER OF PLY FILE
    
    //while(!geometry_file.eof())
    while(geometry_file >> number)
    {
        //PULL IN NEXT LINE OF DATA
        //getline(geometry_file, current_line);
        //geometry_file >> number;
        cout << "number: " << number << endl;
        
        
        
        /*//SPLIT CURRENT_LINE INTO INDIVIDUAL PIECES OF DATA
        stringstream line_to_split(current_line);
        string temp;
        
        while(getline(line_to_split, temp, '\t'))
        {
            tokens.push_back(temp);
            
            
        }

        tokens.clear();
    */}
    
    
}

void readGeometryFilePLY(string geometry_file_name, vector<float>& x_main, vector<float>& y_main, vector<float>& z_main, vector<float>& red_main, vector<float>& green_main, vector<float>& blue_main, vector<float>& num_vertices_to_connect_main, vector<float>& vertex1_main, vector<float>& vertex2_main, vector<float>& vertex3_main)
{
    const int NUM_COLUMNS_X_Y_Z_R_G_B = 6;
    const int NUM_COLUMNS_CONNECTIVITY = 4;
    
    int nums_previous_line = 0;
    int nums_current_line = 0;
    
    vector<vector <float> > geometry_file_x_y_z_r_g_b;
    vector<vector <float> > geometry_file_connectivity;
    ifstream geometry_file(geometry_file_name);
    
    if(geometry_file.is_open())
    {
        float num;
        
        vector<float> numbers_in_line;
        float line_number;
        
        while(geometry_file >> num)
        {
            numbers_in_line.push_back(num);
            cout << "numbers_in_line.size(): " << numbers_in_line.size() << endl;
            for(int i = 0; i < numbers_in_line.size(); i++)
            {
                cout << "numbers_in_line[" << i << "]: " << numbers_in_line[i] << endl;
            }
            
            if(numbers_in_line.size() == NUM_COLUMNS_X_Y_Z_R_G_B)
            {
                geometry_file_x_y_z_r_g_b.push_back(numbers_in_line);
                numbers_in_line.clear();
                
                line_number++;
                cout << "line_number: " << line_number << endl;
            }
            /*else if(numbers_in_line.size() == NUM_COLUMNS_CONNECTIVITY)
            {
                geometry_file_connectivity.push_back(numbers_in_line);
                numbers_in_line.clear();
            }*/
            
        }
    }
    
    
    vector<float> x;
    vector<float> y;
    vector<float> z;
    vector<float> red;
    vector<float> green;
    vector<float> blue;
    
    vector<float> num_vertices_to_connect;
    vector<float> vertex1;
    vector<float> vertex2;
    vector<float> vertex3;
    
    int column_to_input = 1;
    vector<float> column;
    
    //X, Y, Z, R, G, B VALUES
    //FIX: convert r, g, b values to range [0, 1]
    /*for(int j = 0; j < NUM_COLUMNS_X_Y_Z_R_G_B; j++)
    {
        for(int i = 0; i < geometry_file_x_y_z_r_g_b.size(); i++)
        {
            column.push_back(geometry_file_x_y_z_r_g_b[i][column_to_input - 1]);
            //cout << " column.size(): " << column.size() << endl;
        }
        if((column_to_input - 1) == 0)
        {
            x = column;
            column.clear();
        }
        if((column_to_input - 1) == 1)
        {
            y = column;
            column.clear();
        }
        if((column_to_input - 1) == 2)
        {
            z = column;
            column.clear();
        }
        if((column_to_input - 1) == 3)
        {
            red = column;
            column.clear();
        }
        if((column_to_input - 1) == 4)
        {
            green = column;
            column.clear();
        }
        if((column_to_input - 1) == 5)
        {
            blue = column;
            column.clear();
        }
        
        column_to_input++;
    }*/
    
    /*for(int k = 0; k < NUM_COLUMNS_CONNECTIVITY; k++)
    {
        for(int l = 0; l < geometry_file_connectivity.size(); l++)
        {
            column.push_back(geometry_file_connectivity[l][column_to_input - 1]);
        }
        if((column_to_input - 1) == 0)
        {
            num_vertices_to_connect = column;
        }
        if((column_to_input - 1) == 1)
        {
            vertex1 = column;
        }
        if((column_to_input - 1) == 2)
        {
            vertex2 = column;
        }
        if((column_to_input - 1) == 3)
        {
            vertex3 = column;
        }
        
        column_to_input++;
        
    }*/
    
    geometry_file.close();
    
    x_main = x;
    y_main = y;
    z_main = z;
    red_main = red;
    green_main = green;
    blue_main = blue;
    /*num_vertices_to_connect_main = num_vertices_to_connect;
    vertex1_main = vertex1;
    vertex2_main = vertex2;
    vertex3_main = vertex3;
*/
    //FIX: free memory here for temp vectors
    
}

void readGeometryFileTXT(string geometry_file_name, vector<float>& x1_main, vector<float>& y1_main, vector<float>& z1_main, vector<float>& x2_main, vector<float>& y2_main, vector<float>& z2_main, vector<float>& x3_main, vector<float>& y3_main, vector<float>& z3_main)
{
    const int NUM_COLUMNS = 18;
    
    vector<vector <float> > geometry_file_data;
    ifstream geometry_file(geometry_file_name);
    
    //DETERMINE HOW MANY LINES OF DATA ARE IN GEOMETRY FILE
    if(geometry_file.is_open())
    {
        float num;
        
        vector<float> numbers_in_line;
        
        while(geometry_file >> num)
        {
            numbers_in_line.push_back(num);
            
            if(numbers_in_line.size() == NUM_COLUMNS)
            {
                geometry_file_data.push_back(numbers_in_line);
                numbers_in_line.clear();
            }
        }
    }
    
    //GEOMETRY FILE
    vector<float> x1;
    vector<float> y1;
    vector<float> z1;
    vector<float> x2;
    vector<float> y2;
    vector<float> z2;
    vector<float> x3;
    vector<float> y3;
    vector<float> z3;
    
    int column_to_input = 1;
    vector<float> column;
    
    for(int j = 0; j < NUM_COLUMNS; j++)
    {
        for(int i = 0; i < geometry_file_data.size(); i++)
        {
            column.push_back(geometry_file_data[i][column_to_input - 1]);
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
        
        column_to_input++;
    }
    
    geometry_file.close();
    
    x1_main = x1;
    y1_main = y1;
    z1_main = z1;
    x2_main = x2;
    y2_main = y2;
    z2_main = z2;
    x3_main = x3;
    y3_main = y3;
    z3_main = z3;
    
    //FIX: free memory here for temp vectors
    
}


