cmake_minimum_required(VERSION 2.6)

project(ProjetIndustriel)

set(EXECUTABLE_OUTPUT_PATH ../${CMAKE_BUILD_TYPE})
add_definitions(-std=c++0x)
SET(CMAKE_CXX_FLAGS "-Wall -g")

find_package (Threads)

file(
	GLOB
	SRCS
	src/*
)

#Déclaration de l'exécutable
add_executable(
        nav_start
      	${SRCS}
)

target_link_libraries (nav_start ${CMAKE_THREAD_LIBS_INIT})
