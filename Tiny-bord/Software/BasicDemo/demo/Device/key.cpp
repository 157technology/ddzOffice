#include "key.hpp"
#include "key_config.h"

#include <stdio.h>
#include "tim.h"

inline Key::KeyState m_get_left_key()
{
	HAL_GPIO_WritePin(BTN1_GPIO_Port, BTN1_Pin, GPIO_PIN_SET);
	return (Key::KeyState)GET_KEY_LEFT();
}
inline Key::KeyState m_get_right_key()
{
	HAL_GPIO_WritePin(BTN2_GPIO_Port, BTN2_Pin, GPIO_PIN_SET);
	return (Key::KeyState)GET_KEY_RIGHT();
}

Key::Key()
{
	m_leftPressTimeCount = 0;
	m_rightPressTimeCount = 0;
	m_leftState = Key::UNPRESSED;
	m_rightState = Key::UNPRESSED;
}

void Key::isr_handle(void)
{
	/*left*/
	Key::KeyState sl = m_get_left_key();
	if ( sl == Key::UNPRESSED )
	{
		//printf("Key Left Unpress.\n");
		//HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
	}
	else
	{
		printf("####Key Left press.\n");
		//HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
	}
	
	/*right*/
	Key::KeyState sr = m_get_right_key();
	if ( sr == Key::UNPRESSED )
	{
		//printf("Key Right Unpress.\n");
		//HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
	}
	else
	{
		printf("####Key Right press.\n");
		//HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
	}
	
	if ( sl == Key::UNPRESSED and sr == Key::UNPRESSED )
	{
__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
	}
	else
	{
		if ( sl == Key::PRESSED )
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1000);
		else
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 500);		
	}
		
	
	if ( m_leftState == Key::UNPRESSED ) {}
}

extern Key btn;
extern "C" {
	extern TIM_HandleTypeDef	htim11;
	void TIM1_TRG_COM_TIM11_IRQHandler(void)
	{
		/* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 0 */
		//SEGGER_SYSVIEW_RecordEnterISR();
		//EvrMyC1_Active(555);
		/* USER CODE END TIM1_TRG_COM_TIM11_IRQn 0 */
		HAL_TIM_IRQHandler(&htim11);
		/* USER CODE BEGIN TIM1_TRG_COM_TIM11_IRQn 1 */
		btn.isr_handle();
		//SEGGER_SYSVIEW_RecordExitISR();
		//EvrMyC1_Idle(555);
		/* USER CODE END TIM1_TRG_COM_TIM11_IRQn 1 */
	}
}