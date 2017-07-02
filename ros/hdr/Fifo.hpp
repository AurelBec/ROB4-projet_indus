#ifndef FIFO_HPP
#define FIFO_HPP

#include <iostream>
#include <vector>


class Fifo
{
private:
	std::string m_name;
	int nbListen, nbPublish;

public:
	Fifo(const std::string name) : m_name(name), nbListen(0), nbPublish(0) {};
	~Fifo() {};

	std::string getName() const {return m_name;}
	void addListen() {nbListen ++;}
	void addPublish() {nbPublish ++;}

	int getNbListen() const {return nbListen;}
	int getNbPublish() const {return nbPublish;}

	void print() const {std::cout << m_name << " nbL:" << nbListen << " nbP:" << nbPublish << std::endl;}
	
};

typedef typename std::vector<Fifo*> V_FIFO;
typedef typename V_FIFO::const_iterator IT_FIFO;

#endif
