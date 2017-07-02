#include "../hdr/Config.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[])
{
	system("rm -rf *.fifo");

	if( argc != 2)
	{
		cerr << "WAR : Not enough inputs" << endl;
		cerr << "expected '" << argv[0] << " your_config_file.txt'" << endl;
		cerr << "received '" << argv[0] << "'" << endl;
		cerr << "using 'config.txt' as default" << endl << endl;
		argv[1] = (char*) "config.txt";
	}

	//Creation de la config
	Config *config = new Config();

	//Chargement du fichier config
	if( !config->init(argv[1]))
		return EXIT_FAILURE;

	//Verification de la config
	if( !config->check())
		return EXIT_FAILURE;

	//Creation des fifo
	if( !config->createFifo())
		return EXIT_FAILURE;

	//config->print();
	
	//Execution des nodes
	cerr << endl << " -- STARTING Roomba Operating System --" << endl;

	vector<Node*> v_node = *(config->getNodes());
	for(IT_NODE node = v_node.begin(); node != v_node.end(); ++node)	
	{
		int pid = fork();

		if( pid < 0)
		{
			fprintf(stderr, "ERR : Unable to create new process\n");
			exit(EXIT_FAILURE);
		}
		else if( pid == 0)
		{
			(*node)->run();
			return 0;
		}
	}

	//Fin du programme
	for(IT_NODE node = v_node.begin(); node != v_node.end(); ++node)	
		wait(NULL);

	cerr << " -- ENDING Roomba Operating System --" << endl;

	delete config;
	
	system("rm -rf *.fifo");

	return 0;
}

