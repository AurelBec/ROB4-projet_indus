#include "../hdr/thread.h"
#include "../../lidar/hdr/rplidar.h"

using namespace std;

void* listen_odometry( void* param)
{
	Odom* odom = (Odom*) param;
	float tab[3] = {0,0,0};

	odom->mtx->lock();
	int port = odom->port;
	odom->mtx->unlock();

	do
	{
		if( read(port, tab, 100) < 0)
			cerr << "echec" << endl;
		else
		{
			odom->mtx->lock();
			odom->x = tab[0];
			odom->y = tab[1];
			odom->angle = tab[2];
			odom->check = true;
			odom->mtx->unlock();
		}

	} while( !( tab[0] == -1 && tab[1] != -1 && tab[2] != -1));
}

void* listen_lidar(void* param)
{
	Data_lidar* data_lidar = (Data_lidar*) param;

	data_lidar->mtx->lock();
	int port = data_lidar->port;
	data_lidar->mtx->unlock();

	float data[360*2][2];
	int nbpacket = 1;
	//rplidar_response_measurement_node_t nodes;

	do
	{
		if(read(port, data, 4096) < 0)
			cerr << "echec" << endl;
		else
		{
			data_lidar->mtx->lock();
			data_lidar->data[0][0] = (int) data[0][0];
			data_lidar->data[0][1] = nbpacket ++;

			for (int pos = 1; pos < data_lidar->data[0][0]; ++pos) 
		    {
				//fprintf(Mon_Doccument, "%f:%f\n", (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f, nodes[pos].distance_q2/4.0f);

		        //cerr << "theta: " << (nodes.angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f << " dist: " << nodes.distance_q2/4.0f << endl;
		        //cerr << "theta: " << data[pos][0] << " dist: " << data[pos][1] << endl;
		        data_lidar->data[pos][0] = data[pos][0];			
		        data_lidar->data[pos][1] = data[pos][1];	
			}
			data_lidar->mtx->unlock();
			usleep(1000);
	    }

	} while( data_lidar->data[0][0] != -1);
} 