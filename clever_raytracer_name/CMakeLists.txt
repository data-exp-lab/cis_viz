cmake_minimum_required(VERSION 2.6)
project(cis_ray)

IF(APPLE)
	ADD_DEFINITIONS(-pipe -std=c++11 -stdlib=libc++)
ELSE(APPLE)
	ADD_DEFINITIONS(-pipe -std=gnu++1y -Wall)
ENDIF(APPLE)



ADD_EXECUTABLE(cis_ray main.cpp io.cpp commandArgs.cpp ray.cpp climate.cpp color.cpp compound.cpp equations.cpp leaf.cpp light.cpp scene.cpp stomata.cpp triangle.cpp )






