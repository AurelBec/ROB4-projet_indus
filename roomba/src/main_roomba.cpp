//
//  main.cpp
//  Roomba
//
//  Created by janakiraman gopinath on 2/27/15.
//  Copyright (c) 2015 org.koneksahealth.com. All rights reserved.
//


#include "../hdr/Roomba.h"
#include "../hdr/common.h"
#include "../hdr/thread.h"


using namespace std;

int main(int argc, const char * argv[]) 
{	

	int port_odometry_out, port_posture_in, port_cmd_usr_in, port_cmd_dock_in;
	int i(argc - 1);
	while (i > 0)
	{
		if(string(argv[i]) == "--listen")
		{
			if(string(argv[i + 1]) == "commande_usr")
			{
				port_cmd_usr_in = open(argv[i+2], O_RDONLY);
				if( port_cmd_usr_in < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}

			if(string(argv[i + 1]) == "commande_dock")
			{
				port_cmd_dock_in = open(argv[i+2], O_RDONLY);
				if( port_cmd_dock_in < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}

			if(string(argv[i + 1]) == "postures")
			{
				port_posture_in = open(argv[i+2], O_RDONLY);
				if( port_posture_in < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		if(string(argv[i]) == "--publish")
		{
			if(string(argv[i + 1]) == "odometry")
			{
				port_odometry_out = open(argv[i+2], O_WRONLY);
				if( port_odometry_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		i--;
	}

	cerr << "Starting node roomba" << endl;

	Roomba r;
	if (r.getStatus())
		cerr << "Robot initialized successfully" << endl;
	else
		cerr << "Failed to initialize robot" << endl;

	int speed = 0;
	float tab_odom[3];

	bool display_odom = false;

	char key = -1;

	//r->setDebug(true);
	//r->printCommands();
	//int events = BUMP_EVENT | VIRTUAL_WALL_EVENT;// | SONG_PLAYING_EVENT | VIRTUAL_WALL_EVENT;
	//r->setEvents(events, eventf);
	//r->bumpEvent(eventf);
	//r->songPlayingEvent(eventf);
	//r->virtualWallEvent(eventf);
	//r->spin(1); // -1 for clockwise spin
	//r->drive(500, 0); // Velocity and angle
	//r->driveDirect(100, -100);
	//r->bumpEvent(eventf);
	array<int, 32> songSequence = { 76,8, 76,16, 76,16, 72,8, 76,16, 79,32, 67,32 };
	r.createSong(0, 7, songSequence);
	r.writeLEDs(string("olaf").c_str());

	
	mutex mtx;
	Cmd_usr cmd_usr = {.port = port_cmd_usr_in, .key = key, .check = false, .mtx = &mtx};
	thread thread_cmd_usr (listen_cmd_usr, &cmd_usr);

	do
	{
		if( cmd_usr.check)
		{
			cerr << "roomba : received : " << cmd_usr.key << endl;

			switch(cmd_usr.key)
			{
				case 'o' : 
					display_odom = !display_odom;
					break;
				case 'x' :
					r.stop();
					break;

				case 'p':
					speed += 50;
					break;
				case 'm':
					speed -= 50;
					break;

				case 'z':
					r.driveDirect(speed, speed);
					break;
				case 'q':
					r.driveDirect(speed, - speed);
					break;
				case 's':
					r.driveDirect(- speed, - speed);
					break;
				case 'd':
					r.driveDirect(- speed, speed);
					break;

				case 'e':
					r.playSong(0);
					break;

				default: break;
			}
			cmd_usr.mtx->lock();
			cmd_usr.check = false;
			cmd_usr.mtx->unlock();
		
		}

		if( !r.getStatus())
		{
			tab_odom[0] = -1;
			tab_odom[1] = -1;
			tab_odom[2] = -1;
			write(port_odometry_out, &tab_odom, 100);
		}
		else
		{
			tab_odom[0] = r.x;
			tab_odom[1] = r.y;
			tab_odom[2] = r.angle;
			write(port_odometry_out, &tab_odom, 100);
		}

		 if( display_odom == true)
		 	cerr << "roomba : X: " << r.x << " Y: " << r.y << " cap: " << r.angle << endl;

		r.updatePos();

	} while( r.getStatus());

	r.stop(); // Finally stop the robot

	thread_cmd_usr.join();


	cerr << "Ending node Roomba" << endl;

	close(port_odometry_out);
	close(port_posture_in);
	close(port_cmd_usr_in);
	close(port_cmd_dock_in);

	return 0;
}
