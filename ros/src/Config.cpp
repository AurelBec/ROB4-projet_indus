#include "../hdr/Config.hpp"

using namespace std;

bool Config::init(const string fname)
{
	ifstream fichier(fname.c_str(), ios::in);
	string line;

	if( !fichier)
	{
		cerr << "ERR : Unable to open file '" << fname << "'" << endl;
		return false;
	}

	while (getline(fichier, line))
	{
		if(line.empty()) 
			continue;

		stringstream buffer(line);
		string command, params;

		buffer >> command >> ws && getline(buffer, params, '\0');
		command.erase( command.end()-1 );

		if(command == "node")
		{
			if( m_node.size() != 0 && m_node.back()->isBinEmpty())
			{
				cerr << "ERR : Node " << *(m_node.back()->getName()) << " doesn't have any bin" << endl;
				return false;
			}

			m_node.push_back(new Node(params));
		}
		else if( command == "bin")
		{
			if( !m_node.back()->addBin(params))
				return false;
		}
		else if( command == "publish")
		{
			if( !m_node.back()->addPort(params, PUBLISH))
				return false;
		}
		else if( command == "listen")
		{
			if( !m_node.back()->addPort(params, LISTEN))
				return false;
		}
		else if( command == "args")
		{
			if( !m_node.back()->addArgs(params))
				return false;
		}
		else
		{
			cerr << "ERR : Unknown command '" << command.c_str() << "' in " << fname << endl;
			cerr << "expected 'node', 'bin', 'args', 'publish', 'listen'" << endl;
			//return false;
		}
	}
	
	fichier.close();
	return true;
}

void Config::print()
{
	cerr << " -- CONFIGURATION --" << endl;

	for(IT_NODE node = m_node.begin(); node != m_node.end(); ++node)
		(*node)->print();
}

bool Config::createFifo()
{

	for(IT_FIFO fifo = m_fifo.begin(); fifo != m_fifo.end(); ++fifo)
	{
		if(mkfifo((*fifo)->getName().c_str(), 0777) != 0 )
		{
			cerr << "ERR : Unable to create fifo '" << (*fifo)->getName() << "'" << endl;
			return false;
		}
	}

	return true;
}

bool Config::check()
{
	return checkFifo() && checkExe();
}

bool Config::checkFifo()
{
	bool present;
	vector<Port*> v_port;

	//Creation liste fifo
	for (IT_NODE node = m_node.begin(); node != m_node.end(); ++node)
	{
		v_port = *((*node)->getPublish());
		for(IT_PORT port = v_port.begin(); port != v_port.end(); ++port)
		{
			present = false;
			for(IT_FIFO fifo = m_fifo.begin(); fifo != m_fifo.end(); ++fifo)
				if((*fifo)->getName() == *((*port)->getFifo()))
				{
					present = true;
					(*fifo)->addPublish();
				}

			if(!present)
			{
				m_fifo.push_back(new Fifo(*((*port)->getFifo())));
				m_fifo.back()->addPublish();
			}
		}

		v_port = *((*node)->getListen());
		for(IT_PORT port = v_port.begin(); port != v_port.end(); ++port)
		{
			present = false;
			for(IT_FIFO fifo = m_fifo.begin(); fifo != m_fifo.end(); ++fifo)
				if((*fifo)->getName() == *((*port)->getFifo()))
				{
					present = true;
					(*fifo)->addListen();
				}

			if(!present)
			{
				m_fifo.push_back(new Fifo(*((*port)->getFifo())));
				m_fifo.back()->addListen();
			}
		}
	}

	for(IT_FIFO fifo = m_fifo.begin(); fifo != m_fifo.end(); ++fifo)
		if( (*fifo)->getNbListen() != 1 || (*fifo)->getNbPublish() != 1)
		{
			cerr << "ERR : number of I/O for '" << (*fifo)->getName() << "' uncorrect" << endl;
			
			int nbcand = 0;
			for( IT_FIFO fifocand = fifo + 1; fifocand != m_fifo.end(); ++fifocand)
			{
				if( (*fifocand)->getNbListen() != 1 || (*fifocand)->getNbPublish() != 1)
				{
					nbcand ++;
					if(nbcand == 1)
					{
						cerr << "Try replacing by :" << endl;
						cerr << "  " << (*fifo)->getName() << endl;
					}
					cerr << "  " << (*fifocand)->getName() << endl;
				}
			}

			if(nbcand == 0)
			{
				if((*fifo)->getNbListen() != 1)
					cerr << "expected one listen" << endl;
				else
					cerr << "expected one publish" << endl;
			}

			return false;
		}

	return true;
}

bool Config::checkExe()
{
	cerr << "WAR  : Exe non verifies (add 'echo' before './')" << endl;
	return true;
}

Config::~Config()
{
	for (IT_NODE node = m_node.begin(); node != m_node.end(); ++node)
		delete (*node);
	m_node.clear();

	for (IT_FIFO fifo = m_fifo.begin(); fifo != m_fifo.end(); ++fifo)
		delete (*fifo);
	m_fifo.clear();
}

