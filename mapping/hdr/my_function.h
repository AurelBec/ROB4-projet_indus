#include <mrpt/poses/CPose2D.h>
#pragma once
#ifndef __Func__Mapping__
#define __Func__Mapping__

#include <mrpt/maps/COccupancyGridMap2D.h>
#include <mrpt/maps/CSimplePointsMap.h>
#include <mrpt/slam/CICP.h>

#include "thread.h"

#define NOMBRE_D_OBSERVATION 32
#define OBSERVATION_SIZE_MAX 1024	//	Pour RpLidar


int read_Data(float **P_Data_Tab, char **P_Validity_Tab, Data_lidar* data_lidar);
void createMyVector(float *tab, std::vector<float> *vec, int Nombre_De_Distances);
void createMyVectorChar(char *tab, std::vector<char> *vec, int Nombre_De_Distances);
void setPose(mrpt::poses::CPose2D *P_My_Pose, Odom* odom);

void updateOccupancyGrid(mrpt::maps::COccupancyGridMap2D *P_my_Occupancy_Grid, Odom* odom, Data_lidar* data_lidar, int Measure_Number);
void saveOccupancyGrid(mrpt::maps::COccupancyGridMap2D *P_my_Occupancy_Grid, int Measure_Number);

void ajustPoseWithICP(mrpt::maps::COccupancyGridMap2D *P_my_Occupancy_Grid, mrpt::maps::CSimplePointsMap *P_my_Map, mrpt::poses::CPose2D *My_Pose);
void TestICP(mrpt::maps::COccupancyGridMap2D *P_my_Occupancy_Grid, mrpt::maps::CSimplePointsMap *P_my_Map);

void addOdometry(mrpt::poses::CPose2D *My_Pose);


#endif
