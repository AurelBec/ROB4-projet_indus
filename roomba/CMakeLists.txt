cmake_minimum_required(VERSION 2.6)

project(ProjetIndustriel)

set(EXECUTABLE_OUTPUT_PATH ../${CMAKE_BUILD_TYPE})
add_definitions(-std=c++0x -lwiringPi -lpthread)
SET(CMAKE_CXX_FLAGS "-Wall -g")

find_package (Threads)
FIND_LIBRARY(WIRINGPI_LIBRARY wiringPi /home/ubuntu/wiringPi)

file(
	GLOB
	SRCS
	src/*
)

#Déclaration de l'exécutable
add_executable(
        roomba_start
      	${SRCS}
)

target_link_libraries (roomba_start ${CMAKE_THREAD_LIBS_INIT} ${WIRINGPI_LIBRARY} rt crypt)
