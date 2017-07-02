#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "Fifo.hpp"
#include "Node.hpp"

class Config
{
private: 
	std::vector<Fifo*> m_fifo;
	std::vector<Node*> m_node;

	bool checkFifo();
	bool checkExe();

public:
	Config(){};
	~Config();

	const std::vector<Node*> *getNodes() const {return &m_node;}

	bool init(const std::string fname);
	bool check();
	void print();
	bool createFifo();
};

#endif
