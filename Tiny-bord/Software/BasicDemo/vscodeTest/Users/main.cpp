#include "common.h"

using namespace std;
/**/
SHT21 sht21;
Serial * console;
Serial * wifis;
/**/

class Led : public Object
{
public:
    Led(){};
    ~Led(){};
    void click()
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    }

    void longpress()
    {
        HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    }

    void getdata(char * data)
    {
        //console->print("%s\n", data);
        //printf("Has get: %s\n", data);
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    }
};
/**/
#ifndef GUI_CONST_STORAGE
#define GUI_CONST_STORAGE const
#endif
static GUI_CONST_STORAGE GUI_LOGPALETTE _Palwifi = {
    2, // Number of entries
    0, // No transparency
    NULL};

static GUI_CONST_STORAGE unsigned char _acwifi[] = {
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, ________, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXX_____, ________, _____XXX, XXXXXXXX,
    XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
    XXXXXXX_, ________, ________, ________, _XXXXXXX,
    XXXXX___, ________, ________, ________, ___XXXXX,
    XXX_____, ________, ________, ________, ____XXXX,
    XX______, ________, XXXXXXXX, ________, ______XX,
    X_______, ____XXXX, XXXXXXXX, XXXX____, _______X,
    ________, __XXXXXX, XXXXXXXX, XXXXXX__, ________,
    X_______, XXXXXXXX, XXXXXXXX, XXXXXXXX, _______X,
    XX_____X, XXXXXXXX, XXXXXXXX, XXXXXXXX, X_____XX,
    XXX__XXX, XXXXXXXX, XX____XX, XXXXXXXX, XXX__XXX,
    XXXXXXXX, XXXXXX__, ________, __XXXXXX, XXXXXXXX,
    XXXXXXXX, XXXX____, ________, ____XXXX, XXXXXXXX,
    XXXXXXXX, XX______, ________, ______XX, XXXXXXXX,
    XXXXXXXX, X_______, ________, _______X, XXXXXXXX,
    XXXXXXXX, ________, ________, ________, XXXXXXXX,
    XXXXXXXX, X_______, __XXXX__, _______X, XXXXXXXX,
    XXXXXXXX, XX_____X, XXXXXXXX, X_____XX, XXXXXXXX,
    XXXXXXXX, XXX__XXX, XXXXXXXX, XXX__XXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XX____XX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, X_______, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXX__XXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX};

GUI_CONST_STORAGE GUI_BITMAP bmwifi = {
    40,                             // xSize
    40,                             // ySize
    5,                              // BytesPerLine
    1,                              // BitsPerPixel
    (const unsigned char *)_acwifi, // Pointer to picture data (indices)
    &_Palwifi                       // Pointer to palette
};

static GUI_CONST_STORAGE unsigned char _acterminal[] = {
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXX_XXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XX___XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XX____XX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXX____X, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXX____, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXX___, _XXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXX__, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXX_, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, ____XXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, X____XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XX____XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XX_____X, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XX____XX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, X____XXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, ____XXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXX_, ___XXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXX__, __XXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXX___, _XXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXX____, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXX____X, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XX____XX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XX___XXX, XXXXXXXX, X_______, ________, ______XX,
    XXXXXXXX, XXXXXXXX, ________, ________, ______XX,
    XXXXXXXX, XXXXXXXX, ________, ________, ______XX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX,
    XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX, XXXXXXXX};

GUI_CONST_STORAGE GUI_BITMAP bmterminal = {
    40,                                 // xSize
    40,                                 // ySize
    5,                                  // BytesPerLine
    1,                                  // BitsPerPixel
    (const unsigned char *)_acterminal, // Pointer to picture data (indices)
    &_Palwifi                           // Pointer to palette
};

static GUI_CONST_STORAGE unsigned char _acset[] = {
  XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXX___, XXXXXX__, ________, __XXXXXX, ___XXXXX,
  XXX_____, __XXX___, ________, ___XXX__, _____XXX,
  XXX_____, ________, ________, ________, _____XXX,
  XX______, ________, ________, ________, ______XX,
  XX______, ________, ________, ________, ______XX,
  XX______, ________, ________, ________, _______X,
  XX______, ________, __XXXX__, ________, _______X,
  XX______, ________, XXXXXXXX, ________, ______XX,
  XXX_____, _______X, XXXXXXXX, X_______, _____XXX,
  XXXX____, ______XX, XXXXXXXX, XX______, ____XXXX,
  XXXXX___, ______XX, XXXXXXXX, XX______, __XXXXXX,
  XXXXXX__, _____XXX, XXXXXXXX, XXX_____, __XXXXXX,
  XXXXXXX_, _____XXX, XXXXXXXX, XXX_____, _XXXXXXX,
  XXXXXXX_, _____XXX, XXXXXXXX, XXX_____, _XXXXXXX,
  XXXXXX__, _____XXX, XXXXXXXX, XXX_____, __XXXXXX,
  XXXXX___, ______XX, XXXXXXXX, XX______, ___XXXXX,
  XXXX____, ______XX, XXXXXXXX, XX______, ____XXXX,
  XX______, _______X, XXXXXXXX, X_______, _____XXX,
  XX______, ________, XXXXXXXX, ________, ______XX,
  X_______, ________, __XXXX__, ________, ______XX,
  XX______, ________, ________, ________, ______XX,
  XX______, ________, ________, ________, ______XX,
  XX______, ________, ________, ________, ______XX,
  XXX_____, ________, ________, ________, _____XXX,
  XXXX____, __XXX___, ________, ___XXX__, _____XXX,
  XXXXX___, XXXXXX__, ________, __XXXXX_, ____XXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXX_, ________, _XXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, ________, XXXXXXXX, XXXXXXXX,
  XXXXXXXX, XXXXXXXX, X______X, XXXXXXXX, XXXXXXXX
};

GUI_CONST_STORAGE GUI_BITMAP bmset = {
  40, // xSize
  40, // ySize
  5, // BytesPerLine
  1, // BitsPerPixel
  (const unsigned char *)_acset,  // Pointer to picture data (indices)
  &_Palwifi   // Pointer to palette
};

void test(void *argument)
{
	GUI_Init();
    GUI_DrawBitmap(&bmwifi, 2, 12);
    GUI_DrawBitmap(&bmterminal, 44, 12);
    GUI_DrawBitmap(&bmset, 86, 12);
    GUI_DispStringAt(__TIME__, 80, 0);
    GUI_DispStringHCenterAt("Wifi", 63, 54);
    while (1)
    {
        GUI_DispStringAt("T:", 0, 0);
        GUI_DispFloat(sht21.temper(), 5);
        //GUI_DispCEOL();
        osDelay(2000);
        GUI_DispStringAt("H:", 0, 0);
        GUI_DispFloat(sht21.humidity(), 5);
        //GUI_DispCEOL();
        //GUI_DispString("This is test.\n");
        osDelay(2000);
    }
}

void main_thread(void *argument)
{
    //GUI_DispStringAt("Hello World.......", 0, 18);

    while (1)
    {
        /* code */
        OLED_Replot();
        osDelay(40); // max 25 fps
    }
}

/**/
static const osThreadAttr_t GUIThread_attr = {
    .priority = osPriorityLow};

int main(void)
{
    /* code */

    Key *k = keypointer();
    Led *led = new Led;
    console = new Serial("com1");
    wifis = new Serial("com6");
    wifis->init();
    console->init();
    console->print("This is com1\n");

    connect(wifis, &Serial::readyRead, led, &Led::getdata);
    connect(console, &Serial::readyRead, led, &Led::getdata);
    connect(k, &Key::leftClicked, led, &Led::click);
    connect(k, &Key::leftLongClicked, led, &Led::longpress);
    //OLED_Init();
    //wifis->print("AT\r\n");
	HAL_Delay(1000);
	wifis->print("AT\r\n");

    SystemCoreClockUpdate();
    osKernelInitialize(); // Initialize CMSIS-RTOS

    osThreadNew(main_thread, NULL, &GUIThread_attr); // Create application main thread
    osThreadNew(test, NULL, NULL);
    osKernelStart(); // Start thread execution
    for (;;)
    {
    }

    return 0;
}
