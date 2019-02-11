cmake_minimum_required(VERSION 2.6)
project(cis_ray)

IF(APPLE)
	ADD_DEFINITIONS(-pipe -Ofast -O3 -std=c++11 -stdlib=libc++)
ELSE(APPLE)
	ADD_DEFINITIONS(-pipe -std=gnu++1y -Wall)
ENDIF(APPLE)

#ADD_DEFINITIONS(-DDEBUG_LOG)
#ADD_DEFINITIONS(-DDEBUG_LOG_FILE_INFO)

ADD_DEFINITIONS(-DTIMER_SUMMARY)

IF( DEFINED ENV{USE_TESTING} )
	ADD_DEFINITIONS( -DRUN_TESTS)
ENDIF()

ADD_EXECUTABLE(cis_ray main.cpp timing.cpp debug.cpp climate.cpp commandArgs.cpp constants.cpp equations.cpp normal.cpp parameters.cpp triangle.cpp vect.cpp)

