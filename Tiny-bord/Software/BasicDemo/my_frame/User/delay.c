#include "delay.h"
#include "delay_config.h"



void Delay_us(uint32_t us_cnt)
{
	__HAL_TIM_SetCounter(&Delay_Timer, 0);
	__HAL_TIM_ENABLE(&Delay_Timer);
	
	while ( __HAL_TIM_GetCounter(&Delay_Timer) < (Delay_Timer_Frequency*us_cnt) );
	
	__HAL_TIM_DISABLE(&Delay_Timer);
}