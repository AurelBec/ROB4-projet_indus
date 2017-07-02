#include "../hdr/Port.hpp"
	
void Port::print(const int type)
{
	using namespace std;

	cerr << "    " << m_namePort;

	switch(type)
	{
		case LISTEN:
			cerr << " from ";
			break;

		case PUBLISH:
			cerr <<  " to ";
			break;
	}

	cerr << m_nameFifo << endl;
}