cmake_minimum_required(VERSION 2.6)

project(ProjetIndustriel)

set(EXECUTABLE_OUTPUT_PATH ../${CMAKE_BUILD_TYPE})
add_definitions(-std=c++11)
SET(CMAKE_CXX_FLAGS "-Wall -g")

find_package (Threads)

file(
	GLOB
	SRCS
	src/*
)

#include(cmakemodules/script_create_MRPTConfig.cmake REQUIRED)  # Generate the MRPTConfig.cmake & configure files


FIND_PACKAGE( MRPT REQUIRED base) # WARNING: Add all the MRPT libs used by your program: "gui", "obs", "slam",etc.
#FIND_PACKAGE( MRPT REQUIRED hwdrivers)
FIND_PACKAGE( MRPT REQUIRED obs)
FIND_PACKAGE( MRPT REQUIRED slam)



#Déclaration de l'exécutable
add_executable(
        slam_start
      	${SRCS}
)



target_link_libraries (slam_start ${CMAKE_THREAD_LIBS_INIT} ${MRPT_LIBS})



