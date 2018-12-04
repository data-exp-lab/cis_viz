#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <functional>
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

void readGeometryFilePLY(string geometry_file_name, vector<float>& x_main, vector<float>& y_main, vector<float>& z_main, vector<float>& red_main, vector<float>& green_main, vector<float>& blue_main, vector<float>& num_vertices_to_connect_main, vector<float>& vertex1_main, vector<float>& vertex2_main, vector<float>& vertex3_main, float& min_x, float& max_x, float& min_y, float& max_y, float& min_z, float& max_z, int& num_element_vertex, int& num_element_face)
{
    /*
     *  Read through header -> set vertices to number of vertices
     *                      -> set triangles to number of faces
     *  Calculate number of lines in the header
     *          -Look for end_header line by looping through lines of header
     *  Read in line number 1 of vertices section
     *          -Calculate how many elements are in it (should be 6 -> x, y, z, r, g, b)
     *  Start at first line of vertices section (number of lines in header + 1)
     *          -Loop through number of lines of vertices
     *          -Put x, y, z, r, g, b in respective vectors to be passed back to the main function
     *  Read in line number 1 of faces/triangles section
                -Calculate how many elements are in it ( should be 4 -> number vertices, vertex1, vertex2, vertex3)
     *  Start at first line of faces section (number of lines in header + number of lines in vertices section + 1)
     *          -Loop through number of faces
     *          -Put number vertices, vertex1, vertex2, vertex3 in respective vectors to be passed
                    back to the main function
    */
    
    float temp_min_x;
    float temp_max_x;
    float temp_min_y;
    float temp_max_y;
    float temp_min_z;
    float temp_max_z;
    float temp_min = 0;
    float temp_max = 0;
    
    int num_lines_vertices;
    int num_lines_triangles;
    vector<float> numbers_in_line_vertices;
    vector<float> numbers_in_line_triangles;
    
    const int NUM_COLUMNS_X_Y_Z_R_G_B = 6;
    const int NUM_COLUMNS_CONNECTIVITY = 4;
    
    vector<vector <float> > geometry_file_x_y_z_r_g_b;
    vector<vector <float> > geometry_file_connectivity;
    
    ifstream geometry_file(geometry_file_name);
    
    if(!geometry_file.is_open())
    {
        cout << "Error opening input file" << endl;
        exit(1);
    }
    if(geometry_file.is_open())
    {
        float num = 0;
        float numbers_in_line = 0;
        int lines_in_header = 0;
        string current_string;
        vector<string> tokens;
        int tokens_size = 0;
        string i;
        
        //BOTH CONTAIN SPACES AT THE END OF THE STRINGS
        string element_vertex = "element vertex ";
        string element_face = "element face ";

        
        //READ HEADER & GET NUMBER OF LINES IN HEADER
        while(current_string != "end_header")
        {
            getline(geometry_file, current_string);
            cout << current_string << endl;
            
            //FIX: MAKE THESE ASSIGNMENTS VARIABLE BASED ON STRING
            if(current_string.find(element_vertex) != std::string::npos)
            {
                cout << "found element_vertex" << endl;
                std::stringstream temp(current_string);
                
                while(temp >> i)
                {
                    tokens.push_back(i);
                }
                tokens_size = tokens.size();
                
                num_lines_vertices =  std::atoi(tokens[tokens_size - 1].c_str());
                cout << "num_lines_vertices: " << num_lines_vertices << endl;
                num_element_vertex = num_lines_vertices;
            }
            if(current_string.find(element_face) != std::string::npos)
            {
                cout << "found element_face" << endl;
                std::stringstream temp(current_string);
                
                while(temp >> i)
                {
                    tokens.push_back(i);
                }
                tokens_size = tokens.size();
                
                num_lines_triangles = std::atoi(tokens[tokens_size - 1].c_str());
                cout << "num_lines_triangles: " << num_lines_triangles << endl;
                num_element_face = num_lines_triangles;
            }
            lines_in_header++;
        }
        int counter = lines_in_header + 1;
        cout << "counter before starting: " << counter << endl;
        cout << "lines_in_header: " << lines_in_header << endl;
        
        while((geometry_file >> num)) //&& (counter != (lines_in_header + num_lines_vertices)))
        {
            if(counter <= (lines_in_header + num_lines_vertices))
            {
                numbers_in_line_vertices.push_back(num);
                if(numbers_in_line_vertices.size() == NUM_COLUMNS_X_Y_Z_R_G_B)
                {
                    for(int i = 0; i < numbers_in_line_vertices.size(); i++)
                    {
                        cout << "numbers_in_line_vertices[" << i << "]: " << numbers_in_line_vertices[i] << endl;
                    }
                    geometry_file_x_y_z_r_g_b.push_back(numbers_in_line_vertices);
                    numbers_in_line_vertices.clear();
                    
                    counter++;
                    cout << "counter: " << counter << endl;
                }
            }
            else//((counter > (lines_in_header + num_lines_vertices)) && (counter <= lines_in_header + num_lines_vertices + num_lines_triangles))
            {
                numbers_in_line_triangles.push_back(num);
                
                if(numbers_in_line_triangles.size() == NUM_COLUMNS_CONNECTIVITY)
                {
                    for(int i = 0; i < numbers_in_line_triangles.size(); i++)
                    {
                        cout << "numbers_in_line_triangles[" << i << "]: " << numbers_in_line_triangles[i] << endl;
                    }
                    geometry_file_connectivity.push_back(numbers_in_line_triangles);
                    numbers_in_line_triangles.clear();
                    
                    counter++;
                    cout << "counter: " << counter << endl;
                }
            }
        }
    }
        
    /*for(int i = 0; i < geometry_file_x_y_z_r_g_b.size(); i++)
    {
        for(int j = 0; j < geometry_file_x_y_z_r_g_b[i].size(); j++)
        {
            cout << "geometry_file_x_y_z_r_g_b[" << i << "][" << j << "]: " << geometry_file_x_y_z_r_g_b[i][j] << endl;
        }
    }*/
    
    //VECTORS TO FILL WITH INFO FROM PLY FILE
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
    
    for(int j = 0; j < NUM_COLUMNS_X_Y_Z_R_G_B; j++)
    {
        for(int i = 0; i < geometry_file_x_y_z_r_g_b.size(); i++)
        {
            column.push_back(geometry_file_x_y_z_r_g_b[i][column_to_input - 1]);
            //cout << "geometry_file_x_y_z_r_g_b[" << i << "][" << column_to_input - 1 << "]: " << geometry_file_x_y_z_r_g_b[i][column_to_input - 1] << endl;
        }
        
            if((column_to_input - 1) == 0)
            {
                x = column;
                column.clear();
                
                temp_min_x = x[0];
                temp_max_x = x[0];
                for(int i = 0; i < x.size(); i++)
                {
                    if(x[i] > temp_max_x)
                    {
                        temp_max_x = x[i];
                    }
                    else if(x[i] < temp_min_x)
                    {
                        temp_min_x = x[i];
                    }
                }
                cout << "temp_max_x: " << temp_max_x << endl;
                cout << "temp_min_x: " << temp_min_x << endl;
            }
            if((column_to_input - 1) == 1)
            {
                y = column;
                column.clear();
                
                temp_min_y = y[0];
                temp_max_y = y[0];
                for(int i = 0; i < y.size(); i++)
                {
                    if(y[i] > temp_max_y)
                    {
                        temp_max_y = y[i];
                    }
                    else if(y[i] < temp_min_y)
                    {
                        temp_min_y = y[i];
                    }
                }
                cout << "temp_max_y: " << temp_max_y << endl;
                cout << "temp_min_y: " << temp_min_y << endl;
            }
            if((column_to_input - 1) == 2)
            {
                z = column;
                column.clear();
                
                temp_min_z = z[0];
                temp_max_z = z[0];
                for(int i = 0; i < z.size(); i++)
                {
                    if(z[i] > temp_max_z)
                    {
                        temp_max_z = z[i];
                    }
                    else if(z[i] < temp_min_z)
                    {
                        temp_min_z = z[i];
                    }
                }
                cout << "temp_max_z: " << temp_max_z << endl;
                cout << "temp_min_z: " << temp_min_z << endl;
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
    }
    
    column_to_input = 1;
    
    for(int j = 0; j < NUM_COLUMNS_CONNECTIVITY; j++)
    {
        for(int i = 0; i < geometry_file_connectivity.size(); i++)
        {
            column.push_back(geometry_file_connectivity[i][column_to_input - 1]);
            //cout << "geometry_file_connectivity[" << i << "][" << column_to_input - 1 << "]: " << geometry_file_connectivity[i][column_to_input - 1] << endl;
        }
        
        if((column_to_input - 1) == 0)
        {
            num_vertices_to_connect = column;
            column.clear();
        }
        if((column_to_input - 1) == 1)
        {
            vertex1 = column;
            column.clear();
        }
        if((column_to_input - 1) == 2)
        {
            vertex2 = column;
            column.clear();
        }
        if((column_to_input - 1) == 3)
        {
            vertex3 = column;
            column.clear();
        }
        column_to_input++;
    }
    
    geometry_file.close();

    x_main = x;
    y_main = y;
    z_main = z;
    red_main = red;
    green_main = green;
    blue_main = blue;
    
    num_vertices_to_connect_main = num_vertices_to_connect;
    vertex1_main = vertex1;
    vertex2_main = vertex2;
    vertex3_main = vertex3;
    
    min_x = temp_min_x;
    max_x = temp_max_x;
    min_y = temp_min_y;
    max_y = temp_max_y;
    min_z = temp_min_z;
    max_z = temp_max_z;
    
}

void readGeometryFilePLY2(string geometry_file_name, vector<float>& x_main, vector<float>& y_main, vector<float>& z_main, vector<float>& red_main, vector<float>& green_main, vector<float>& blue_main, vector<float>& num_vertices_to_connect_main, vector<float>& vertex1_main, vector<float>& vertex2_main, vector<float>& vertex3_main)
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
    for(int j = 0; j < NUM_COLUMNS_X_Y_Z_R_G_B; j++)
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
    }
    
    for(int k = 0; k < NUM_COLUMNS_CONNECTIVITY; k++)
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
        
    }
    
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


