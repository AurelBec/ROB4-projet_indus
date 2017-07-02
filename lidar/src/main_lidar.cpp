#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iostream>

//	projet/rplidar/sdk/sdk/include



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hdr/rplidar.h" //RPLIDAR standard sdk, all-in-one header

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifdef _WIN32
#include <Windows.h>
#define delay(x)   ::Sleep(x)
#else
#include <unistd.h>
static inline void delay(_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}
#endif

using namespace rp::standalone::rplidar;

bool checkRPLIDARHealth(RPlidarDriver * drv)
{
    u_result     op_result;
    rplidar_response_device_health_t healthinfo;


    op_result = drv->getHealth(healthinfo);
    if (IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        printf("RPLidar health status : %d\n", healthinfo.status);
        if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want rplidar to be reboot by software
            // drv->reset();
            return false;
        } else {
            return true;
        }

    } else {
        fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
        return false;
    }
}

#include <signal.h>
bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}






using namespace std;

int main(int argc, char* argv[])
{
	int port_mesure_map_out, port_mesure_nav_out;
	const char * opt_com_path = NULL;
	opt_com_path = "/dev/ttyUSB0";
	_u32         opt_com_baudrate = 115200;
	u_result     op_result;

	int i(argc - 1);
	while (i > 0)
	{
		if(string(argv[i]) == "--publish")
		{
			if(string(argv[i + 1]) == "mesure_nav")
			{
				port_mesure_nav_out = open(argv[i+2], O_WRONLY);
				if( port_mesure_nav_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}				
			}

			if(string(argv[i + 1]) == "mesure_map")
			{
				port_mesure_map_out = open(argv[i+2], O_WRONLY);
				if( port_mesure_map_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}				
			}
		}

		if(string(argv[i]) == "--baudrate")
		{
			opt_com_baudrate = strtoul(argv[i+1], NULL, 10);
		}

		if(string(argv[i]) == "--path")
		{
			opt_com_path = argv[i+1];
		}

		i--;
	}

	//usleep(5000000);
	cerr << "Starting node lidar" << endl;

//	Copier/Collé de "/projet/rplidar/sdk/app/ultra_simple"


	    // read serial port from the command line...
	   // if (argc>1) opt_com_path = argv[1]; // or set to a fixed value: e.g. "com3" 

	    // read baud rate from the command line if specified...
	   // if (argc>2) opt_com_baudrate = strtoul(argv[2], NULL, 10);

		//opt_com_path = "/dev/ttyUSB0";
		//opt_com_baudrate = 115200;


	    // create the driver instance
	    RPlidarDriver * drv = RPlidarDriver::CreateDriver(RPlidarDriver::DRIVER_TYPE_SERIALPORT);
	    
	    if (!drv) 
	    {
			fprintf(stderr, "insufficent memory, exit\n");
			exit(-2);
	    }


	    // make connection...
	    if (IS_FAIL(drv->connect(opt_com_path, opt_com_baudrate))) 
	    {
			fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n" , opt_com_path);
			goto on_finished;
	    }

	    rplidar_response_device_info_t devinfo;

		// retrieving the device info
	    ////////////////////////////////////////
	    op_result = drv->getDeviceInfo(devinfo);

	    if (IS_FAIL(op_result)) 
	    {
			fprintf(stderr, "Error, cannot get device info.\n");
			goto on_finished;
	    }

	    // print out the device serial number, firmware and hardware version number..
	    printf("RPLIDAR S/N: ");
	    for (int pos = 0; pos < 16 ;++pos) 
	    {
			printf("%02X", devinfo.serialnum[pos]);
	    }

	    printf("\n"
		    "Firmware Ver: %d.%02d\n"
		    "Hardware Rev: %d\n"
		    , devinfo.firmware_version>>8
		    , devinfo.firmware_version & 0xFF
		    , (int)devinfo.hardware_version);



	    // check health...
	    if (!checkRPLIDARHealth(drv)) 
	    {
			goto on_finished;
	    }

		signal(SIGINT, ctrlc);
	    
		drv->startMotor();
	    // start scan...
	    drv->startScan();

	  //  sleep(2);

	    // fetech result and print it out...
	    while (1) 
	    {
			//char File_Name[128];
			//File_Name[0] = '\0';
			//strcat(File_Name, "Mesure_Num_");

			//char Number[256];
		 	//sprintf(Number, "%d", Iteration_Number);

			//strcat(File_Name, Number);

			//FILE *Mon_Doccument = fopen(File_Name, "w");


			rplidar_response_measurement_node_t nodes[360*2];
			float data[361*2][2];

			size_t   count = _countof(nodes);

			op_result = drv->grabScanData(nodes, count);

			if (IS_OK(op_result)) 
			{
			    drv->ascendScanData(nodes, count);
			    data[0][0] = count;
			    data[0][1] = count;

			   	for (int pos = 0; pos < (int)count ; ++pos) 
			    {
					//fprintf(Mon_Doccument, "%f:%f\n", (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f, nodes[pos].distance_q2/4.0f);

			       /* printf("%s theta: %03.2f Dist: %08.2f Q: %d \n", 
			            (nodes[pos].sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT) ?"S ":"  ", 
			            (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f,
			            nodes[pos].distance_q2/4.0f,
			            nodes[pos].sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);

			    	write(port_mesure_map_out, &(nodes[pos]), 100);*/

			    	data[pos+1][0] = (float)  (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f;
			    	data[pos+1][1] = nodes[pos].distance_q2/4.0f;
			    }
			}

			//read(port_request_mesure, XXXX, XXXX);
			//if(request = 1)
				//write(port_mesure_map_out, &count, 100);
			  	write(port_mesure_map_out, &data, 4096);

			//usleep(100000);


			if (ctrl_c_pressed)
			{
				data[0][0] = -1;
				data[0][1] = -1;
			  	write(port_mesure_map_out, &data, 4096);
				break;
			}

			//fclose(Mon_Doccument);
			//Iteration_Number ++;
	    }

	    drv->stop();
	    drv->stopMotor();
	    // done!
	on_finished:
	    RPlidarDriver::DisposeDriver(drv);





	

	cerr << "Ending node lidar" << endl;
	close(port_mesure_nav_out);
	close(port_mesure_map_out);

	return 0;
}








