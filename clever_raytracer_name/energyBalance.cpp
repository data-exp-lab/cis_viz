#include "energyBalance.hpp"
#include "constants.hpp"
#include <iostream>

using namespace std;

EnergyBalance::EnergyBalance()
{
    leafWidth = 0.015;
    windSpeedHeightCoeff = 0.64;
    windSpeedRoughnessLength = 0.013;
}

Parameters EnergyBalance::calculateLeafT(Constants cs, Parameters ip, double canopyHeight)
{
    double I = ip.PPFD * 0.235;
    double JA = I * 1.57;
    double TAIR = cs.TAIR;
    
    //ENERGY BALANCE MODEL FOR LEAF
    
    //SATURATE WATER VAPOR PRESSURE
    double EST = exp(9.3876 - 3826.36 / (TAIR + 273.15 - 45.47)) * 1000;
    
    //RH ON LEAF SURFACE
    double leafRelativeHumidity;
    if(cs.RH > 0 && cs.RH < 1)
    {
        leafRelativeHumidity = cs.RH * 100;
    }
    else
    {
        leafRelativeHumidity = cs.RH;
    }
    
    //VAPOR PRESSURE DEFICITY
    double VPD = EST * (1 - leafRelativeHumidity / 100);
    
    //UNIT: KJ/KG
    double L = -0.0000614342 * pow(TAIR,3) + 0.00158927 * pow(TAIR,2) - 2.36418 * TAIR + 2500.79;
    //UNIT: J./KG
    double latentHeatVaporization = L * 10e3;
    
    //WATER VAPOR / DRY AIR
    double MWRATIO = 0.622;
    double RH = 0;
    double RV = 0;
    RV = RH;
    //STOMATAL RESISTANCE
    //CONVERT GS TO MMOL.M-2.S-1 to M.S-1
    double RS = 1 / (ip.GS * 1e-3 * 24.39 * 1e-3);
    
    //AIR DENSITY
    double ROUGH = 101.3e3 / (287.058 * (273.16 + TAIR));
    double air_pressure = 101;
    
    double CP = cs.SPECIFIC_HEAT_CAPACITY_AIR;
    
    //UNIT: KPA/K
    double psychromatric = CP * air_pressure / (latentHeatVaporization * MWRATIO);
    double psychromatric_star = psychromatric * (RV + RS) / RH;
    
    double S = exp(9.3876 - 3826.36 / (TAIR + 273.15 - 45.47)) * 1000 * 3826.36 / pow(TAIR + 273.15 - 45.47, 2);
    
    //TEMPERATURE DIFFERENCE (TLEAF - TAIR)
    double deltaTal = 0.01;
    double changeInLeafTemp = 5;
    double deltaTal_old = 0;
    //RADIATIVE EXCHANGE FROM LEAF -> AIR
    double RN = 0;
    //NET RADIATIVE ABSORBED ENERGY FROM OUTSIDE -> LEAF
    double PHIN = 0;
    
    int counter = 0;
    while(changeInLeafTemp > 0.2 && counter < 500)
    {
        deltaTal_old = deltaTal;
        
        //RADIATIVE EXCHANGE
        RN = (4 * cs.STEFAN_BOLTZMAN_CONST * pow(273 + TAIR, 3) * deltaTal);
        
        //CALCULATE NET ABSORBED RADIATIVE ENERGY
        //TOTAL ABSORBED SHOULD BE LESS THAN OR EQUAL TO TOTAL SOLAR RADIATION
        PHIN = JA + RN;
        if(PHIN > JA)
        {
            PHIN = JA;
        }
        
        deltaTal = (psychromatric * RH / (ROUGH * CP)) * RN / (S + psychromatric_star) - VPD / (S + psychromatric_star);
        changeInLeafTemp = abs(deltaTal_old - deltaTal);
        
        counter++;
    }
    
    double TLEAF = TAIR + deltaTal;
    ip.Tleaf = TLEAF;
    
    return ip;
}

Parameters EnergyBalance::calculateLeafT_withWindSpeed(Constants cs, Parameters ip, double canopyHeight)
{
    double I = ip.PPFD * 0.235;
    double JA = I * 2;
    
    double TAIR = cs.TAIR;
    
    double layerWindSpeed = 0.5;
    
    //CALCULATION OF LEAF BOUNDARY LAYER
    double leafBoundaryLayer = (2.126e-5 + 1.48e-7 * TAIR) / (0.004 * sqrt(leafWidth / layerWindSpeed));
    //CALCULATE THE DIFFUSION COEFFICIENT OF WATER VAPOR
    //CALCULATE THE BOUNDARY LAYER THICKNESS FROM THE SIZE OF THE LEAF IN THE DIRECTION OF THE WIND AND THE WIND SPEED
    
    //APPLY MONTEITH EQUATION TO CALCULATE THE WIND SPEED
    double D = 0.63 * canopyHeight;
    double Z0 = 0.1 * canopyHeight;
    
    double windSpeedHeight = 2.5;
    
    double u_star = cs.WINDSPEED_TEMP;
    double u = (u_star / cs.KAPPA) * (log((windSpeedHeight - D) / Z0));
    
    //ENERGY BALANCE MODEL
    
    double EST = exp(9.3876 - 3826.36 / (TAIR + 273.15 - 45.47)) * 1e6;
    
    //RH ON LEAF SURFACE
    double leafRelativeHumidity;
    if(cs.RH > 0 && cs.RH < 1)
    {
        leafRelativeHumidity = cs.RH * 100;
    }
    else
    {
        leafRelativeHumidity = cs.RH;
    }
    
    //VAPOR PRESSURE DEFICITY
    double VPD = EST * (1 - leafRelativeHumidity / 100);
    
    double L = 0.0000614342 * pow(TAIR,3) + 0.00158927 * pow(TAIR,2) - 2.36418 * TAIR + 2500.79;
    double latentHeatVaporization = L * 10e3;
    
    //WATER VAPOR / DRY AIR
    double MWRATIO = 0.622;
    
    //HEAT RESISTANCE OF BOUNDARY LAYER OF LEAF
    double RH = 50;
    
    //VAPORIZATION RESISTANCE OF BOUNDARY LAYER OF LEAF
    double RV;
    RV = RH;
    
    //STOMATAL RESISTANCE
    double RS = 1 / (ip.GS * 1e3 * 24.39 * 1e-3);
    
    //AIR DENSITY
    double ROUGH = 101.3e3 / (287.058 * (273.16 + TAIR));
    double air_pressure = 101 * 1000;
    
    double CP = cs.SPECIFIC_HEAT_CAPACITY_AIR;
    
    double psychromatric = CP * air_pressure / (latentHeatVaporization * MWRATIO);
    double psychromatric_star = psychromatric * (RV + RS) / RH;
    
    double S = exp(9.3876 - 3826.36 / (TAIR + 273.15 - 45.47)) * 1e6 * 3826.36 / pow(TAIR + 273.15 - 45.47, 2);
    
    //TEMPERATURE DIFFERENCE (TAIR - TLEAF)
    double deltaTal = 0.01;
    double changeInLeafTemp = 5;
    double deltaTal_old = 0;
    
    //RADIATIVE EXCHANGE BETWEEN LEAF AND AIR FROM LEAF -> AIR
    double RN = 0;
    //NET RADIATIVE ABSORBED ENERGY FROM OUTSIDE -> LEAF
    double PHIN = 0;
    
    int counter = 0;
    while(changeInLeafTemp > 0.05 && counter < 500)
    {
        deltaTal_old = deltaTal;
        
        //RADIATIVE EXCHANGE
        RN = (4 * cs.STEFAN_BOLTZMAN_CONST * pow(273 + TAIR,3) * deltaTal);
        
        //CALCULATE NET ABSORBED RADIATIVE ENERGY
        PHIN = JA - RN;
        
        double a_temp = (psychromatric_star * RH / (ROUGH * CP)) * PHIN / (S + psychromatric_star);
        double b_temp = VPD / (S + psychromatric_star);
        deltaTal = a_temp - b_temp;
        
        changeInLeafTemp = abs(deltaTal_old - deltaTal);
        
        counter++;
    }
    
    double E = (S * PHIN + ROUGH * CP * VPD / RH) / (S + psychromatric_star) / latentHeatVaporization;
    double TLEAF = TAIR + deltaTal;
    ip.Tleaf = TLEAF;
    
    return ip;
  
}

