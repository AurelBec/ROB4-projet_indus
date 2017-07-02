#ifndef PORT_HPP
#define PORT_HPP

#include <iostream>
#include <vector>


enum type
{
	LISTEN, PUBLISH
};

class Port
{
private:
	std::string m_namePort;
	std::string m_nameFifo;

public:
	Port(const std::string namePort, const std::string nameFifo) : m_namePort(namePort), m_nameFifo(nameFifo) {};
	~Port(){};

	const std::string *getName() const {return &m_namePort;}
	const std::string *getFifo() const {return &m_nameFifo;}

	void print(const int type);
};


typedef typename std::vector<Port*> V_PORT;
typedef typename V_PORT::const_iterator IT_PORT;

#endif
