#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int port_command_out, port_request_in;

	int i(1);
	while (i < argc)
	{
		if(string(argv[i]) == "--listen")
		{
			if(string(argv[i + 1]) == "request")
			{
				port_request_in = open(argv[i+2], O_RDONLY);
				if( port_request_in < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		if(string(argv[i]) == "--publish")
		{
			if(string(argv[i + 1]) == "commande")
			{
				port_command_out = open(argv[i+2], O_WRONLY);
				if( port_command_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		i++;
	}

	cerr << "Starting node docking" << endl;


	cerr << "Ending node docking" << endl;
	close(port_request_in);
	close(port_command_out);

	return 0;
}
