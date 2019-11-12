#include "startup.h"

#include "led.h"

int main(void)
{
	ledDrv.set(&ledDrv, 1, LED_ON);
	while ( 1 )
	{
		//ledDrv.rightstream(&ledDrv, 100);
	}
	return 0;
}