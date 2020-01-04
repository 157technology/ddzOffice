#ifndef __KEY_H__
#define __KEY_H__

#include "signalslot.h"

class Key : public Object
{
private:
    /* data */
public:
    Key();
    ~Key()  {}

    enum KeyState
    {
        PRESSED = 0,
        UNPRESSED,
        RELEASED,
        LONGPRESSED,
        CLICKED
    };

private:
    int m_LeftPressCount;
    enum KeyState m_Left_State;

    int m_RightPressCount;
    enum KeyState m_Right_State;

public:
    /* 仅用于底层 */
    void m_isr_handle(void);

    /*-------------------------------*/
   	/* signal for Key left */
	void leftClicked(void) ;
	void emit_leftClicked(void) ;


	void leftLongClicked(void) ;
	void emit_leftLongClicked(void) ;

	/* signal for Key right */
	void rightClicked(void) {};
	void emit_rihgtClicked(void) {};

	void rightPressed(void) {};
	void emit_rightPressed(void) {};

	void rightReleased(void) {};
	void emit_rightReleased(void) {}; 

};

extern void init_Key(void);
extern Key * keypointer(void);

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
#include "main.h"
#include "gpio.h"
#include "tim.h"

#define SET_KEY_LEFT()      HAL_GPIO_WritePin(BTN1_GPIO_Port, BTN1_Pin, GPIO_PIN_SET)
#define SET_KEY_RIGHT()     HAL_GPIO_WritePin(BTN2_GPIO_Port, BTN2_Pin, GPIO_PIN_SET)
#define GET_KEY_LEFT()		HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin)
#define GET_KEY_RIGHT()		HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin)
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


#endif // !__KEY_H__
