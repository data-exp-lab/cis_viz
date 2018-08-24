cmake_minimum_required(VERSION 2.6)
project(cis_ray)

IF(APPLE)
	ADD_DEFINITIONS(-pipe -std=c++11 -stdlib=libc++)
ELSE(APPLE)
	ADD_DEFINITIONS(-pipe -std=gnu++1y -Wall)
ENDIF(APPLE)

ADD_DEFINITIONS(-DDEBUG_LOG)
#ADD_DEFINITIONS(-DDEBUG_LOG_FILE_INFO)

ADD_DEFINITIONS(-DTIMER_SUMMARY)

ADD_EXECUTABLE(cis_ray main.cpp timing.cpp debug.cpp)
