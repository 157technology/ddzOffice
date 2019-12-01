#include "QPinDev.hpp"

#include <iostream>

#include "main.h"
#include "gpio.h"
using namespace std;

QPinDev::QPinDev()
{
	
}

QPinDev::~QPinDev()
{
}

int QPinDev::readData(char * data, int maxSize)
{
	cout << "Get data" << endl;
	
	return 1;
}

bool QPinDev::writeData(const char * data, int maxSize)
{
	
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PinState(data[0]));
	//cout << data << endl;
	return true;
}