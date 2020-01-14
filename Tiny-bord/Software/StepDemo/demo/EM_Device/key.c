#include "key.h"

#include <stdio.h>
/**/
static Key * m_key = NULL;
static inline Key::KeyState m_get_left_key()
{
    SET_KEY_LEFT();
    return (Key::KeyState)GET_KEY_LEFT();
}
static inline Key::KeyState m_get_right_key()
{
    SET_KEY_RIGHT();
    return (Key::KeyState)GET_KEY_RIGHT();
}
/**/

Key::Key()
{
    m_LeftPressCount = 0;
    m_Left_State = Key::UNPRESSED;

    m_RightPressCount = 0;
    m_Right_State = Key::UNPRESSED;

    m_key = this;
}

void Key::m_isr_handle()
{
    Key::KeyState sleft = m_get_left_key();
    Key::KeyState sright = m_get_right_key();

    if ( sleft == Key::UNPRESSED )
    {
        if ( m_LeftPressCount >= 5 and m_Left_State != Key::LONGPRESSED ) /*防抖*/
        {
            // 触发 点击信号
            //printf("left click\n");

            emit_leftClicked();
        }
        m_LeftPressCount = 0;
        m_Left_State = Key::UNPRESSED;
    }

    if ( sright == Key::UNPRESSED )
    {
        m_RightPressCount = 0;
    }



    if (sleft == Key::UNPRESSED and sright == Key::UNPRESSED)
    {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
    }
    else
    {
        if (sleft == Key::PRESSED)
        {
            if ( m_LeftPressCount ++ >= 1000 )
            {
                // 触发长按信号
                 __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 2500);
                 //printf("long press\n");
                 m_Left_State = Key::LONGPRESSED;
                 m_LeftPressCount = 0;
                 emit_leftLongClicked();
            }
            else
            {
                __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
            } 
        }
           
        else
        {

            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
        }
            
    }
}

void Key::leftClicked() {}
void Key::emit_leftClicked()
{
    emit(leftClicked);
}

void Key::leftLongClicked() {}
void Key::emit_leftLongClicked()
{
    emit(leftLongClicked);
}
/*>>>>>>>>>>>>>>>>>>>>>>*/
void init_Key(void)
{
    m_key = new Key;
}
Key * keypointer(void)
{
    return m_key;
}
/*>>>>>>>>>>>>>>>>>>>>>>*/


extern "C" {
	extern TIM_HandleTypeDef	htim11;
	void TIM1_TRG_COM_TIM11_IRQHandler(void)
	{

		//SEGGER_SYSVIEW_RecordEnterISR();

		HAL_TIM_IRQHandler(&htim11);

		if ( m_key != NULL )    m_key->m_isr_handle();
		//SEGGER_SYSVIEW_RecordExitISR();

	}
}
