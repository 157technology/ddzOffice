#include "common.hpp"
#include "RTE_Components.h"             // Component selection
#include "EventRecorder.h"              // Keil.ARM Compiler::Compiler:Event Recorder
#define MyEvtC_DEBUG
#include "MyEventComponent.h"



char *__uart1 = "UART1";
char *__tim11 = "TIM11";

#include "signalslot.hpp"
#include "key.hpp"

void signalDis(void * m)
{}
void slotDis(void * m)
{
	em_printf("this is slot:: %s\n", (char*)m);
}
void getData(void * m)
{
	em_printf("Serial Slot:: %s\n", (char*)m);
}
char *para = "test";


extern Serial * pserial;
/*test*/

#include <string>
#include <iostream>
using namespace std;
class TestKey: public Object
{
public:
	TestKey() {};

	void show()
	{
		printf("This is Test Class. Show.\n");
	}
	void tt()
	{

	}

	void hasPress(string key) {}
	void emithasPress(string key)
	{ 
		emitWithParam(hasPress,(key),(key));
	}

};




void ttest() {}
template <typename T, typename... U>
void ttest(T v, U... args)
{
	//cout << v << endl;
	ttest(args...);
}
Key btn;
/*test*/
int main()
{

	registerUartDev();


	EvrMyC1_Create (555, (unsigned int)__tim11);
	EvrMyC1_Create (222, (unsigned int)__uart1);
	
	MallocInit();
	// emSerial = registerSerial(&huart1, 512, 512, NULL);
	// emSerial->Initialize(emSerial);
	// em_printf("-v-\n");
	Serial mySerial("com1");
	mySerial.init();
	mySerial.open();
	
	mySerial.print("This is a Serial Dev.\n");

	ttest(1,2,3,4,5,6);
	TestKey key;
	beep b;
	connect(&key, &TestKey::hasPress, &b, &beep::dealPress);
	connect(&mySerial, &Serial::readyRead, &b, &beep::dealSerial);
	key.emithasPress("Enter");
//	Test tt;
//	Test * pp = &tt;
//	tt.show();
//	omfptr pf = &Object::show;
//	(tt.*pf)();
	//connect(signalDis, slotDis);
	//connect(hasData, getData);
	//emit(signalDis, para);	

	osSetup();

	return 0;
}