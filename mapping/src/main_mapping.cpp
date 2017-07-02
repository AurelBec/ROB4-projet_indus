#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../hdr/my_function.h"
#include "../hdr/thread.h"

using namespace std;
using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::slam;
using namespace mrpt::maps;
using namespace mrpt::obs;
using namespace mrpt::math;
using namespace mrpt::poses;


int main(int argc, char* argv[])
{
	int port_localisation_out,port_slam_out, port_odometry_in, port_mesure_in;

	int i(1);
	while (i < argc)
	{
		if(string(argv[i]) == "--listen")
		{
			if(string(argv[i + 1]) == "odometry")
			{
				port_odometry_in = open(argv[i+2], O_RDONLY);
				if( port_odometry_in < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}

			if(string(argv[i + 1]) == "mesure")
			{
				port_mesure_in = open(argv[i+2], O_RDONLY);
				if( port_mesure_in < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		if(string(argv[i]) == "--publish")
		{
			if(string(argv[i + 1]) == "localisation")
			{
				port_localisation_out = open(argv[i+2], O_WRONLY);
				if( port_localisation_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}

			if(string(argv[i + 1]) == "slam_info")
			{
				port_slam_out = open(argv[i+2], O_WRONLY);
				if( port_slam_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		i++;
	}

	cerr << "Starting node mapping" << endl;


	mutex mtx_odom, mtx_lidar;
	
	Odom odom = {.port = port_odometry_in, .x = 0, .y = 0, .angle = 0, .check = false, .mtx = &mtx_odom};
	thread thread_odom (listen_odometry, &odom);

	Data_lidar data_lidar = {.port = port_mesure_in, .check = false, .mtx = &mtx_lidar};
	thread thread_lidar(listen_lidar, &data_lidar);


	int stop = 0;
	/*do
	{
		mtx_odom.lock();
		if( odom.check)
		{
			if( !(odom.x == -1 && odom.y == -1 && odom.angle == -1))
			{
				cerr << "slam : x: " << odom.x << " y: " << odom.y << " a: " << odom.angle << endl;
				odom.check = false;
			}
			else
			{
				stop = 1;
			}
		}
		mtx_odom.unlock();

		mtx_lidar.lock();
		if()


	} while( stop != 1);*/


//														Remplir entre ici...
//
//




	//	Setting up for the map creation
	mrpt::maps::COccupancyGridMap2D my_Occupancy_Grid;
	int Measure_Number = 0;


	sleep(2);

	do	
	{
		//	For forever (a bit less..)

		//std::cerr << "New reading !" << std::endl;
		//std::cerr << "Calling updateOccupancy !" << std::endl;
		updateOccupancyGrid(&my_Occupancy_Grid, &odom, &data_lidar, Measure_Number);

		//	Sauvegarde des Occupancy_Grid intermédiaires
		/*char File_Name[258];
		File_Name[0] = '\0';
		strcat(File_Name, "MapConstruction/Occupancy_Grid_");

		char My_Number[128];

		//	Ajout de '0's pour que les fichiers apparaissent dans le bon ordre : '008', '009', '010'... au lieu de '8', '9', '10'...
		char Zero[50];
		sprintf(Zero, "%d", 0);
		if(Measure_Number < 100)	{
			if(Measure_Number < 10)	{
				strcat(File_Name, Zero);
			}
			strcat(File_Name, Zero);
		}

		sprintf(My_Number, "%d", Measure_Number);
		strcat(File_Name, My_Number);

		strcat(File_Name, ".png");
			//	File_Name contient desormais le nom du fichier ou la "OccupancyGrid" va etre sauvegardee

//		printf("%s\n", File_Name);
			//	Affichage du nom du fichier sauvegarde

		my_Occupancy_Grid.saveAsBitmapFile(File_Name);*/

		//std::cerr << "OccupancyGrid updated!" << std::endl;

		Measure_Number ++;
	}while( stop != 1);

//
//															...et là.
//

	cerr << "Ending node mapping" << endl;
	
	thread_odom.join();
	thread_lidar.join();
	
	close(port_localisation_out);
	close(port_slam_out);
	close(port_odometry_in);
	close(port_mesure_in);

	return 0;
}
