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

    //SOLAR AND CLIMATE CONSTANTS

    const float kSOLAR_constant = 2600;

    const double ignore_PPFD_threshold = 1; //umol.m - 2.s - 1

    //LIGHT REFLECTIVITY

    const double BRDF_s = 8.67e-01;
    const double BRDF_Rd = 5.61e-02;
    const double BRDF_m = 1.46e-01;
    const double BRDF_F0 = 6.15e-03;
    const double BRDF_ad = 0.001;
    
    //LIGHT INTENSITY
    int INCIDENT_LIGHT_INTENSITY_METHOD = 0;
    double ATMOSPHERE_TRANSMITTANCE = 0.7;
    double DIR_LIGHT_INTENSITY = 1000;
    double DIFF_LIGHT_INTENSITY = 300;
    double TOT_LIGHT_INTENSITY = 1500;
    double DIR_TO_DIFF_RATIO = 3;
    
    //LIGHT DIRECTION
    int INCIDENT_LIGHT_CAL_OR_DIR = 0;
    int NORTH_OR_SOUTH = 0;
    
    //RAY TRACING DIMENSIONS
    double X_MIN = -100;
    double X_MAX = 100;
    double Y_MIN = -100;
    double Y_MAX = 100;
    double Z_MIN = -100;
    double Z_MAX = 100;
    double NEAREST_RAY_DISTANCE = 0.1;
    
    //LEAF REFLECTANCE AND TRANSMITTANCE
    int LEAF_REF_TRAN_CONST_OR_INPUTFILE = 0;
    double LEAF_REF = 0.075;
    double LEAF_TRAN = 0.075;

    //ENERGY BALANCE MODEL
    const double KAPPA = 0.41;
    

    //TEMP SETTING FOR WINDSPEED PARAMETER
    const double WINDSPEED_TEMP = 1;

    //STOMATA CONDUCTANCE MODEL
    const double BOUNDARY_LAYER_CONDUCTANCE = 1000;

    //TIME
    int START_YEAR;
    int END_YEAR;
    float START_HOUR = 9;
    float END_HOUR = 10;
    float INTERVAL_HOUR = 1;
    float START_DAY = 108;
    float END_DAY;
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


    //EARTH AND SOLARITY
    float LATITUDE = 30;
    float LONGITUDE;
    const float SOLAR_CONSTANT = 2600;
    float SOLAR_RADIATION_MJ;
    float T_MAX;
    float T_MIN;
    float RAIN;
    float CLOUD;
    float NUM_WET_DAYS;

    //ENVIRONMENTAL FACTORS
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
    
    //TEMPERATURE RELATED TO PHOTOSYNTHETIC PARAMETERS
    int APPLY_TEMPERATURE_RESPONSE = 0;
    int AIR_TEMPERATURE_INPUT_OR_PREDICT = 0;
    double TAIR = 25;
    double T1 = 17.6;
    double T2 = 13.7;
    double T3 = 3.2;
    double T4 = 5.5;
    double HA_VCMAX = 65.33;
    double C_VCMAX = 26.35;
    double HA_JMAX = 43.54;
    double C_JMAX = 17.57;
    double HA_GSTAR = 37.87;
    double C_GSTAR = 19.02;
    double HA_KC = 79.43;
    double C_KC = 38.05;
    double HA_KO = 36.38;
    double C_KO = 20.3;
    double HA_RD = 46.39;
    double C_RD = 18.72;
    
    //STOMATAL CONDUCTANCE AND INTERCELLULAR CO2
    int STOMATA_AND_CI_METHOD = 1;
    double CA = 280;
    double RH = 50;
    double G0 = 20;
    double G1 = 11.35;
    
    //LIGHT REACTION AND ELECTRON TRANSPORT RATE
    //FARQUHAR MODEL FOR DARK REACTION
    double F = 0.15;
    double JMAX = 140;
    double OA = 210;
    double GSTAR = 41;
    double RD = 1;
    double VCMAX = 80;
    double KN = 2;
    
    vector<double> VCMAX_LIST;
    vector<double> JMAX_LIST;
    

    //LEAF MODEL
    int USE_NITROGEN_CALCULATIONS = 0;
    float ALPHA = 0.7;
    float THETA = 0.7;
    float BETA = 0.7;

    bool CANOPY_CALCULATION = false; //can be changed when calculating more than one plant

    float QUANTUM_YIELD;
    float LIGHT_COMPENSATION_POINT;

    float MESOPHYLL_CO2_CONCENTRATION;
    float GAME_STAR;

    float RATE_TRIOSE_USE;

    const float STOMATA_CONDUCTANCE_COEFFICIENT_G0 = 19.148;
    const float STOMATA_CONDUCTANCE_COEFFICIENT_G1 = 9.8465;
    const float STOMATA_CONDUCTANCE_COEFFICIENT_G0_650 = 20;
    const float STOMATA_CONDUCTANCE_COEFFICIENT_G1_650 = 11.35;
    const float MAX_STOMATA_CONDUCTANCE = 1041;
    const float STOMATA_COEFFICIENT_1 = 25;
    const float STOMATA_COEFFICIENT_2 = -0.004;
    const float STOMATA_COEFFICIENT_3 = 0.0;
    const float MAX_REDUCTION_STOMATA_CONDUCTION = 0.0;

    float C3_DARK_RESPIRATION_COEFFICIENT;
    int RESPIRATION_TEMPERATURE_RESPONSE;

    float LEAF_TRANSPIRATION;
    float LEAF_WATER_POTENTIAL;
    float WATER_STRESS_SENSITIVITY;
    float LEAF_STAMATA_WATER_STRESS_FACTOR;

    float LIGHT_FRACTION_NOT_ABSORBED;
    float SPECTRAL_IMBALANCE;
    float LEAF_REFLECTANCE;

    float ACTIVATION_ENERGY_FOR_KC;
    float ACTIVATION_ENERGY_FOR_KO;
    float ACTIVATION_ENERGY_FOR_VC_MAX;
    float ACTIVATION_ENERGY_FOR_VO_MAX;
    float ACTIVATION_ENERGY_FOR_J_MAX;
    float ACTIVATION_ENERGY_DARK_RESPIRATION;

    float LEAF_NITROGEN_CONCENTRATION = 2;
    float VC_MAX_NITROGEN_CONSTANT_K3;
    float VC_MAX_NITROGEN_CONSTANT_K4;
    float J_MAX_NITROGEN_CONSTANT_K5;
    float J_MAX_NITROGEN_CONSTANT_K6;
    float RD_NITROGEN_CONSTANT_K7;
    float RD_NITROGEN_CONSTANT_K8;

    const float VC_MAX_NITROGEN_HARLEY_K1 = -6.6;
    const float VC_MAX_NITROGEN_HARLEY_K2 = 52.8;
    const float VC_MAX_NITROGEN_HARLEY_K1_650 = -9.6;
    const float VC_MAX_NITROGEN_HARLEY_K2_650 = 60;
    const float J_MAX_NITROGEN_HARLEY_K1 = -4.6;
    const float J_MAX_NITROGEN_HARLEY_K2 = 98.1;
    const float TPU_NITROGEN_HARLEY_K1 = 0.63;
    const float TPU_NITROGEN_HARLEY_K2 = 5.13;

    const float C4CO2_RESPONSE_SLOPE = 0.22;
    const float C4_MAX_RUBISCO_CAPACITY = 31.4;
    const float C4_LIGHT_RESPONSE_SLOPE = 0.04;
    const float C4_LEAF_RESPIRATION_RATE = 0.8;
    const float C4_STOMATA_SLOPE_FACTOR = 4.7824;
    const float C4_STOMATA_INTERCEPT_FACTOR = 32.061;
    const float C4_CURVATURE_PARAM1 = 0.83;
    const float C4_CURVATURE_PARAM2 = 0.93;

    float A_MAX;
    float CONVEXITY;
    float A_MAX_CO2_RESPONSE;
    float QUANTUM_YIELD_CO2_RESPONSE;

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

    //GROWTH MODEL

    //COMPONENT OF ORGANS?
    //REL PARTITION?

    float k_N_UPTAKE_CAPACITY = 0.05;
    float k_LGUCOSE_PER_NITROGEN = 0.1;

    int INITIAL_STEM_LENGTH_SWITCH;
    int INITIAL_S_ROOT_LENGTH_SWITCH;
    int INITIAL_F_ROOT_LENGTH_SWITCH;
    int STEM_GROWTH_MODEL_TYPE;
    int ROOT_GROWTH_MODEL_TYPE; //S root and F root are the same
    int POD_GROWTH_MODEL_TYPE;
    int ROOT_DISTRIBUTION_MODEL_TYPE;

    int LEAF_AREA_TEMPERATURE_LIMIT_SWITCH;
    int LEAF_AREA_WATER_POTENTIAL_SWITCH;

    float MAX_CANOPY_LAI = 9;
    float SPECIFIC_LEAF_AREA = 50; // G/M2
    float SPECIFIC_STEM_LENGTH = 60;
    float SPECIFIC_F_ROOT_LENGTH = 10;
    float SPECIFIC_S_ROOT_LENGTH = 60;
    float SPECIFIC_POD_NUMBER = 20;

    const float GLUCOSE_TO_PROTEIN = 2.556;
    const float GLUCOSE_TO_CARBON = 1.242;
    const float GLUCOSE_TO_LIPID = 3.106;
    const float GLUCOSE_TO_ORGANIZE_ACID = 0.929;
    const float GLUCOSE_TO_MINERALS = 0.05;
    const float GLUCOSE_TO_LIGNIN = 2.174;

    float INITIAL_LEAF_AREA_METHOD;
    int PLANTING_DAY = 100;
    int DEG_DAY_END_STAGES[11] = {185, 230, 260, 302, 591, 1048, 1453, 1614, 2109, 2286, 4000};
    int NUM_DEVELOPMENTAL_STAGES;

    float SUGAR_PHOSPHATE_POOL;
    float INORGANIC_PHOSPHATE_POOL;
    float STARCH_POOL;
    float CYTOSOLIC_SUCROSE_POOL;
    float VACUOLAR_SUCROSE_POOL;
    float APOPLASTIC_SUCROSE_POOL;
    float LOADING_SUCROSE_POOL;
    float UNLOADING_SUCROSE_POOL;
    float SINK_VACUOLAR_SUCROSE_POOL;
    float GROWTH_SUCROSE_POOL;

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

    //SOIL
    //maybe?


    //MODEL
    int MODEL_TYPE;
    int LEAF_GROWTH_MODEL_TYPE = 1;
    int SIMULATION_START_DAY = 100;
    int SIMULATION_END_DAY = 300;
/*
    int convertDateToDOY(int month, int day);
    void calDirDiffPPFD(int TOT_LIGHT_INTENSITY, int DIR_TO_DIFF_RATIO);
    void convertSouthLat(int NORTH_OR_SOUTH, int LATITUDE);
    void calSolarTimeNoon(int SOLAR_TIME_AT_NOON_HOUR, int SOLAR_TIME_AT_NOON_MINUTE);
    void calTotalPPFDandRatio(int DIR_LIGHT_INTENSITY, int DIFF_LIGHT_INTENSITY, int DIR_TO_DIFF_RATIO);
 */

};


int convertDateToDOY(int month, int day)
{
    int DOY = 0;
    int DOY1 = 0;
    int dayInEachMonthList[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for(int i = 0; i < month; i++)
    {
        DOY1 = DOY1 + dayInEachMonthList[i];
    }
    DOY1 = DOY1 + day;
    DOY = DOY1;
    return DOY;
}

void calDirDiffPPFD(int TOT_LIGHT_INTENSITY, int DIR_TO_DIFF_RATIO)
{
    int DIR_LIGHT_INTENSITY;
    int DIFF_LIGHT_INTENSITY;
    DIR_LIGHT_INTENSITY = TOT_LIGHT_INTENSITY * DIR_TO_DIFF_RATIO / (DIR_TO_DIFF_RATIO + 1);
    DIFF_LIGHT_INTENSITY = TOT_LIGHT_INTENSITY - DIR_LIGHT_INTENSITY;
}

void calTotalPPFDandRatio(int DIR_LIGHT_INTENSITY, int DIFF_LIGHT_INTENSITY, int DIR_TO_DIFF_RATIO)
{
    int TOT_LIGHT_INTENSITY;
    TOT_LIGHT_INTENSITY = DIR_LIGHT_INTENSITY + DIFF_LIGHT_INTENSITY;
    if(DIFF_LIGHT_INTENSITY != 0)
    {
        DIR_TO_DIFF_RATIO = DIR_LIGHT_INTENSITY / DIFF_LIGHT_INTENSITY;
    }
    else
    {
        DIR_TO_DIFF_RATIO = 10000;
    }
}

void convertSouthLat(int NORTH_OR_SOUTH, int LATITUDE)
{
    if(NORTH_OR_SOUTH == 1)
    {
        LATITUDE = -LATITUDE;
    }
}

void calSolarTimeNoon(int SOLAR_TIME_AT_NOON_HOUR, int SOLAR_TIME_AT_NOON_MINUTE)
{
    int SOLAR_TIME_NOON;
    SOLAR_TIME_NOON = SOLAR_TIME_AT_NOON_HOUR + SOLAR_TIME_AT_NOON_MINUTE / 60;
}


#endif





