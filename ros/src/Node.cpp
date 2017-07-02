#include "../hdr/Node.hpp"

using namespace std;

void Node::run()
{
	using namespace std;

	vector<char*> commandVect;
	
	//commandVect.push_back((char*) "echo");
	//commandVect.push_back((char*) "strace");
	commandVect.push_back((char*) m_bin.c_str());
	commandVect.push_back((char*) m_args.c_str());

	for(IT_PORT port = m_publish.begin(); port != m_publish.end(); ++port)
	{
		commandVect.push_back((char*) "--publish");
		commandVect.push_back((char*) (*((*port)->getName())).c_str());
		commandVect.push_back((char*) (*((*port)->getFifo())).c_str());
	}

	for(IT_PORT port = m_listen.begin(); port != m_listen.end(); ++port)
	{
		commandVect.push_back((char*) "--listen");
		commandVect.push_back((char*) (*((*port)->getName())).c_str());
		commandVect.push_back((char*) (*((*port)->getFifo())).c_str());
	}

	//commandVect.push_back((char*) "2>");
	//commandVect.push_back((char*) "trace_master2.txt");

	commandVect.push_back((char*) "&");
	commandVect.push_back(NULL);

	char **command = reinterpret_cast<char**> (&commandVect[0]);
	execvp((char*) command[0], command);
}

bool Node::addPort(const string buffer, const int type)
{
	string namePort = "", nameFifo = "", junk = "";
	stringstream ss(buffer);

	ss >> namePort >> junk >> nameFifo;

	switch(type)
	{
		case PUBLISH:
			if( junk != "->" || nameFifo == "")
			{
				cerr << "ERR : Incompatible publish format for node " << m_name << endl;
				cerr << "expected 'publish: XXX -> YYY.fifo'" << endl;
				cerr << "read     'publish: " << buffer << "'" << endl;
				return false;
			}

			m_publish.push_back( new Port(namePort, nameFifo));
			break;

		case LISTEN:
			if( junk != "<-" || nameFifo == "")
			{
				cerr << "ERR : Incompatible listen format for node " << m_name << endl;
				cerr << "expected 'listen: XXX <- YYY.fifo'" << endl;
				cerr << "read     'listen: " << buffer << "'" << endl;
				return false;
			}

			m_listen.push_back( new Port(namePort, nameFifo));
			break;
	}

	return true;
}

bool Node::addArgs(const string buffer) 
{
	m_args = buffer;
	return true;
}

bool Node::addBin(const string buffer) 
{
	/*if(buffer[0] != '.' || buffer[1] != '/')
	{
		cerr << "ERR : Incompatible bin format for node " << m_name << endl;
		cerr << "expected 'bin: ./XXX'" << endl;
		cerr << "read     'bin: " << buffer << "'" << endl;
		return false;
	}*/

	m_bin = buffer;
	return true;
}


void Node::print()
{
	cerr << m_name << " [" << m_bin << " " << m_args << "]" << endl;
	
	if(m_publish.size() != 0)
	{
		cerr << "  publish:" << endl;
		for(IT_PORT port = m_publish.begin(); port != m_publish.end(); ++port)
			(*port)->print(PUBLISH);
	}

	if( m_listen.size() != 0)
	{
		cerr << "  listen:" << endl;
		for(IT_PORT port = m_listen.begin(); port != m_listen.end(); ++port)
			(*port)->print(LISTEN);
	}
	
	cerr << endl;
}
