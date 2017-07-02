#ifndef __Thread__Mapping__
#define __Thread__Mapping__

#include <stdio.h>
#include <unordered_map>
#include <array>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include <thread>
#include <mutex>

void *listen_odometry(void *odom);
void *listen_lidar(void* param);

typedef struct Odom 
{
	int port;
	float x, y, angle;
	bool check;
	std::mutex *mtx;
} Odom;

typedef struct Data_lidar 
{
	int port;
	bool check;
	std::mutex *mtx;
	float data[360*2][2];

} Data_lidar;


#endif
