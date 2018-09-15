#ifndef __CONSTANTS__
#define __CONSTANTS__

#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include "parameters.hpp"

using namespace std;

//MATHEMATICAL CONSTANTS
class Constants
{
public:
    Constants();
    ~Constants();
    
    const double PI = 3.1415926535897932384;
    const double PI_div_2 = 1.5707963267948922192;
    const double PI_mul_4_div_9 = 1.39626340159546;
    const double mul_8_div_9 = 0.8888888888888888889;
    const double TWO_PI = 6.2831853071795864769;
    const double PI_ON_180 = 0.0174532925199432957;
    const double inv_PI = 0.3183098861837906715;
    const double inv_TWO_PI = 0.1591549430918953358;
    
    const double kEpsilon = 0.0001;
    const double kHugeValue = 1.0E10;
    
    const float invRAND_MAX = 1.0 / (float)RAND_MAX;

    const float kSOLAR_constant = 2600;
    
    //DEFAULTS FOR LIGHT REFLECTIVITY
    const double BRDF_s = 8.67e-01;
    const double BRDF_Rd = 5.61e-02;
    const double BRDF_m = 1.46e-01;
    const double BRDF_F0 = 6.15e-03;
    const double BRDF_ad = 0.001;
    
    //DEFAULTS FOR LIGHT INTENSITY
    int INCIDENT_LIGHT_INTENSITY_METHOD = 0;
    double ATMOSPHERE_TRANSMITTANCE = 0.7;
    double DIR_LIGHT_INTENSITY = 1000;
    double DIFF_LIGHT_INTENSITY = 300;
    double TOT_LIGHT_INTENSITY = 1500;
    double DIR_TO_DIFF_RATIO = 3;
    
    //DEFAULTS FOR LIGHT DIRECTION
    int INCIDENT_LIGHT_CAL_OR_DIR = 0;
    int NORTH_OR_SOUTH = 0;
    
    //DEFAULTS FOR RAY TRACING DIMENSIONS
    double X_MIN = -100;
    double X_MAX = 100;
    double Y_MIN = -100;
    double Y_MAX = 100;
    double Z_MIN = -100;
    double Z_MAX = 100;
    double NEAREST_RAY_DISTANCE = 0.1;
    
    //DEFAULTS FOR LEAF REFLECTANCE AND TRANSMITTANCE
    int LEAF_REF_TRAN_CONST_OR_INPUTFILE = 0;
    double LEAF_REF = 0.075;
    double LEAF_TRAN = 0.075;
    
    //DEFAULTS FOR TIME
    int START_YEAR;
    int END_YEAR;
    float START_HOUR = 9;
    float END_HOUR = 10;
    float INTERVAL_HOUR = 1;
    float START_DAY = 108;
    float END_DAY = 110;
    float INTERVAL_DAY = 1;
    int START_MINUTE = 0;
    int END_MINUTE = 0;
    int INTERVAL_MINUTE = 0;
    
    int HOUR = 10;
    int MINUTE = 30;
    
    int TIME_SOLAR_NOON = 12;
    int SOLAR_TIME_AT_NOON_HOUR = 12;
    int SOLAR_TIME_AT_NOON_MINUTE = 0;
    int SOLAR_ELE_ANGLE = 60;
    int SOLAR_DIR_ANGLE = 30;
    
    //DEFAULTS FOR EARTH AND SOLARITY
    float LATITUDE = 30;
    float LONGITUDE;
    const float SOLAR_CONSTANT = 2600;
    float SOLAR_RADIATION_MJ;
    float T_MAX;
    float T_MIN;
    float RAIN;
    float CLOUD;
    float NUM_WET_DAYS;
    
    double TAIR = 25;
    float ALPHA = 0.7;
    float THETA = 0.7;
    float BETA = 0.7;
    
    double CA = 280;
    double RH = 50;
    double G0 = 20;
    double G1 = 11.35;
    
    //VARIABLES FOR ENVIRONMENTAL FACTORS
    float ATMOSPHERIC_TRANSMITTANCE = 0.85;
    const float ATMOSPHERIC_PRESSURE = 101;
    const float ATMOSPHERIC_PRESSURE_SEA_LEVEL = 101.324;
    float ATMOSPHERIC_CO2_CONC;
    float ATMOSPHERIC_O2_CONC;
    float PRECIPITATION_PER_DAY;
    float PRECIPITATION_TOTAL;
    float RELATIVE_HUMIDITY;
    float VAPOR_PRESURE_DEFECIT;
    
    float THERMAL_TIME_SINCE_GERMINATION;
    float THERMAL_DAY_SINCE_GERMINATION;
    
    float TIME_CORRECTION_FACTOR;
    float DAY_TOTAL_PHOTON_FLUX;
    float DAY_TOTAL_THERMAL_HOURS;
    float RUN_TOTAL_PHOTON_FLUX;
    float RUN_TOTAL_THERMAL_HOURS;
    
    const float KARMANS_CONSTANT = 0.41;
    float WIND_SPEED = 2;
    float WIND_SPEED_HEIGHT = 2;
    float ZETA_ROUGHNESS_COEFFICIENT = 0.026;
    float ZETA_M_ROUGHNESS_COEFFICIENT = 0.13;
    float CANOPY_HEIGHT = 0.5;
    
    float PERCENT_SKY_COVERED_WITH_CLOUDS = 0.3;
    const float SPECIFIC_HEAT_CAPACITY_AIR = 1010;
    
    float SINK_MOMENTUM = 0.77;
    float LEAF_WIDTH = 0.04;
    
    float WIND_SPEED_ROUGHNESS_LENGTH = 0.13;
    float WIND_SPEED_HEIGHT_COEFFICIENT = 0.77;
    
    const float STEFAN_BOLTZMAN_CONST = 5.67e-8;
    
    float CANOPY_EXTINCTION_COEFFICIENT = 0.8;
    
    //DEFAULTS FOR FARQUHAR MODEL
    double F = 0.15;
    double JMAX = 140;
    double OA = 210;
    double GSTAR = 41;
    double RD = 1;
    double VCMAX = 80;
    double KN = 2;
    
    vector<double> VCMAX_LIST;
    vector<double> JMAX_LIST;
    
    //CANOPY MODEL (SUNLIGHT VS. SHADED)
    float CANOPY_WIDTH;
    float ROW_DISTANCE;
    float IN_ROW_PLANT_PLACING;
    int ROW_AZIMUTH;
    float SUNLIGHT_VC_MAX;
    float SUHADED_VC_MAX;
    float SUNLIGHT_J_MAX;
    float SHADED_J_MAX;
    
    int USE_SUN_SHADE_VC_MAX = 0;
    int USE_SUN_SHADE_J_MAX = 0;
    double PLANT_DENSITY = 20; //number of stemps.m - 2 (calculate canopy height)
    
    float INITIAL_LEAF_AREA_METHOD;
    int PLANTING_DAY = 100;
    int DEG_DAY_END_STAGES[11] = {185, 230, 260, 302, 591, 1048, 1453, 1614, 2109, 2286, 4000};
    int NUM_DEVELOPMENTAL_STAGES;
    
    float VALS;
    float VAC;
    float VUR;
    float KAC;
    float KCA;
    float KLA;
    float KAL;
    float KXT;
    float KXC;
    float KCV;
    float KVC;
    float KRW;
    float KWR;
    float KUR;
    float KRO;
    float YG;
    float TRANSPORT_PARAMETER;
    float MICHAELIS_MENTENK_1;
    float MICHAELIS_MENTENK_2;
    float MICHAELIS_MENTENK_3;
    float MICHAELIS_MENTENK_4;
    float JOX;
    float JXO;
    float JXT;
    float JTX;
    float JXC;
    float JAL;
    float JLA;
    float JCV;
    float JVC;
    float JAC;
    float JLU;
    float JUR;
    float JRH;
    float JRO;
    float JWR;
    float JRW;
    float CARBON_GAIN;
    float NU_P_TAKE;
    float N_REAL_LOCATION;
    float TOTAL_N_DEMAND;
    float THRESHHOLD_VEG;
    
    
    bool REAL_PARTITION;
    float N_UPTAKE_CAPACITY;
    float GLUCOSE_PER_NITROGEN;
    bool CHANGE_IMAGE;
    
    //MODEL
    int MODEL_TYPE;
    int LEAF_GROWTH_MODEL_TYPE = 1;
    int SIMULATION_START_DAY = 100;
    int SIMULATION_END_DAY = 300;
    
    int convertDateToDOY(int month, int day);
    void calDirDiffPPFD(int TOT_LIGHT_INTENSITY, int DIR_TO_DIFF_RATIO);
    void convertSouthLat(int NORTH_OR_SOUTH, int LATITUDE);
    void calSolarTimeNoon(int SOLAR_TIME_AT_NOON_HOUR, int SOLAR_TIME_AT_NOON_MINUTE);
    void calTotalPPFDandRatio(int DIR_LIGHT_INTENSITY, int DIFF_LIGHT_INTENSITY, int DIR_TO_DIFF_RATIO);
    
};

#endif
