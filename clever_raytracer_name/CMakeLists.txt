cmake_minimum_required(VERSION 2.6)
project(cis_ray)

IF(APPLE)
	ADD_DEFINITIONS(-pipe -std=c++11 -stdlib=libc++)
ELSE(APPLE)
	ADD_DEFINITIONS(-pipe -std=gnu++1y -Wall)
ENDIF(APPLE)



ADD_EXECUTABLE(cis_ray main.cpp ray.cpp leaf.cpp io.cpp  grid.cpp commandArgs.cpp  climate.cpp color.cpp compound.cpp equations.cpp light.cpp scene.cpp stomata.cpp thermal.cpp triangle.cpp box.cpp cimodel.cpp energyBalance.cpp parameters.cpp normal.cpp point.cpp farquharModel.cpp constants.cpp vect.cpp source.cpp ) 





