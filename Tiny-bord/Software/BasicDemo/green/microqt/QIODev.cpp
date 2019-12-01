#include "QIODev.hpp"



QIODevice::QIODevice()
{
	
}

QIODevice::~QIODevice()
{
	
}

bool QIODevice::open(QIODevice::OpenMode mode)
{
	
	return true;
}

void QIODevice::close()
{
	
}

bool QIODevice::getChar(char * c)
{
	this->readData(c, 1);
	return true;
}
bool QIODevice::putChar(char c)
{
	this->writeData(&c, 1);
	return true;
} 