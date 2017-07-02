#include "../hdr/keyInteraction.h"


using namespace std;

int main(int argc, char* argv[])
{
	int port_command_out, port_destination_out, port_request_out;
	char key;

	int i(1);

	while (i < argc)
	{
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

			if(string(argv[i + 1]) == "destination")
			{
				port_destination_out = open(argv[i+2], O_WRONLY);
				if( port_destination_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}

			if(string(argv[i + 1]) == "request")
			{
				port_request_out = open(argv[i+2], O_WRONLY);
				if( port_request_out < 0 )
				{
					fprintf(stderr, "ERR : Impossible d'ouvrir le tube nommé \"%s\"\n", argv[3]);
					exit(EXIT_FAILURE);
				}
			}
		}

		i++;
	}

	cerr << "Starting node telecmd" << endl;

	do
	{
		if(kbhit() == 1)
		{
			key = getch();
	
			if( key == 'a')
				write(port_request_out, &key, 10);
			else
				write(port_command_out, &key, 1);

			//usleep(100);
		}

	} while(key != 'x');


	cerr << "Ending node telecmd" << endl;
	close(port_command_out);
	close(port_request_out);
	close(port_destination_out);

	return 0;
}
