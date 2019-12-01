#ifndef __QPINDEV_H__
#define __QPINDEV_H__ 


#include "QIODev.hpp"

class QPinDev : public QIODevice
{
public:
	QPinDev();
	~QPinDev();

	int  readData(char * data, int maxSize);
	bool writeData(const char * data, int maxSize);
	
};

#endif