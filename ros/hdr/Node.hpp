#ifndef NODE_HPP
#define NODE_HPP

//void node_exec (Node* node, const char* path);

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <algorithm>


#include "Port.hpp"

class Node
{
private:
	std::string m_name;
	std::string m_bin;
	std::string m_args;

	std::vector<Port*> m_listen;
	std::vector<Port*> m_publish;

public:
	Node(const std::string name) : m_name(name), m_bin(""), m_args("") {};
	~Node()
	{
		for (IT_PORT port = m_listen.begin(); port != m_listen.end(); ++port)
			delete (*port);
		m_listen.clear();

		for (IT_PORT port = m_publish.begin(); port != m_publish.end(); ++port)
			delete (*port);
		m_publish.clear();
	}

	void	run();
	bool 	addBin	(const std::string buffer);
	bool 	addPort	(const std::string buffer, const int type);
	bool 	addArgs	(const std::string buffer);

	const	std::vector<Port*>	*getListen()	const 	{return &m_listen;}
	const	std::vector<Port*>	*getPublish()	const 	{return &m_publish;}
	const	std::string	 		*getName() 		const 	{return &m_name;}
	const	std::string 		*getBin() 		const 	{return &m_bin;}

	void 	print();
	bool	isBinEmpty() const { return (m_bin == "");}
};


typedef typename std::vector<Node*> V_NODE;
typedef typename V_NODE::const_iterator IT_NODE;

#endif
