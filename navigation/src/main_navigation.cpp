#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int port_path_out, port_destination_in, port_slam_in, port_mesure_in;

	int i(1);
	while (i < argc)
	{
		if(string(argv[i]) == "--listen")
		{
			if(string(argv[i + 1]) == "destination")
			{
				port_destination_in = open(argv[i+2], O_RDONLY);
				if( port_destination_in < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}

			if(string(argv[i + 1]) == "slam_info")
			{
				port_slam_in = open(argv[i+2], O_RDONLY);
				if( port_slam_in < 0 )
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
			if(string(argv[i + 1]) == "chemin")
			{
				port_path_out = open(argv[i+2], O_WRONLY);
				if( port_path_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		i++;
	}
	
	cerr << "Starting node navigation" << endl;


	cerr << "Ending node navigation" << endl;
	close(port_path_out);
	close(port_destination_in);
	close(port_slam_in);
	close(port_mesure_in);

	return 0;
}
