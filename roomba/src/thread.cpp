#include "../hdr/thread.h"
#include "../hdr/Roomba.h"

void* listen_cmd_usr( void* cmd_temp)
{
	Cmd_usr* command = (Cmd_usr*) cmd_temp;
	char key;
	command->mtx->lock();
	int port = command->port;
	command->mtx->unlock();

	do
	{
		if( read(port, &key, 10) < 0)
			cerr << "echec" << endl;
		else
		{
			command->mtx->lock();
			command->key = key;
			command->check = true;
			command->mtx->unlock();
		}

	} while(command->key != 'x');
}


void eventf(char *event, int value) {


	if(strcmp(event, "BUMP") == 0 ) 
	{
		//printf("Event : %s\n", event);
		if (BUMP_RIGHT(value)) printf("Bump Right\n");
		if (BUMP_LEFT(value)) printf("Bump Left\n");
		if (WHEEL_DROP_LEFT(value)) printf("Wheel Drop Left\n");
		if (WHEEL_DROP_RIGHT(value)) printf("Wheel Drop Right\n");
		if (WHEEL_DROP_CASTER(value)) printf("Wheel Drop Caster\n");
	}
	if (strcmp(event, "SONG_PLAYING") == 0) 
	{
		//printf("Event : %s\n", event);
		if (SONG_PLAYING(value)) printf("Song playing\n");
	}
	if (strcmp(event, "VIRTUAL_WALL") == 0) 
	{
		//printf("Event : %s\n", event);
		if (VIRTUAL_WALL_DETECTED(value)) printf("Virtual wall detected\n");
	}
	if (strncmp(event, "CLIFF", 5) == 0) 
	{
		//printf("Event : %s\n", event);    
		if(CLIFF_LEFT_DETECTED(value)) printf("Cliff left detected\n");
		if(CLIFF_FRONT_LEFT_DETECTED(value)) printf("Cliff front left detected\n");
		if(CLIFF_RIGHT_DETECTED(value)) printf("Cliff right detected\n");
		if(CLIFF_FRONT_RIGHT_DETECTED(value)) printf("Cliff front right detected\n");
	}

}
