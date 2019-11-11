
#include "cmsis_os2.h"
#include "GUI.h"
#include "WM.h"

#include "OLED_I2C.h"

/*----------------------------------------------------------------------------
 *      GUIThread: GUI Thread for Single-Task Execution Model
 *---------------------------------------------------------------------------*/
#define GUI_THREAD_STK_SZ    (2048U)

static void         GUIThread (void *argument);         /* thread function */
static osThreadId_t GUIThread_tid;                      /* thread id */
static uint64_t     GUIThread_stk[GUI_THREAD_STK_SZ/8]; /* thread stack */

static const osThreadAttr_t GUIThread_attr = {
  .stack_mem  = &GUIThread_stk[0],
  .stack_size = sizeof(GUIThread_stk),
  .priority   = osPriorityIdle 
};

int Init_GUIThread (void) {

  GUIThread_tid = osThreadNew(GUIThread, NULL, &GUIThread_attr);
  if (GUIThread_tid == NULL) {
    return(-1);
  }

  return(0);
}

void _cbTimer(GUI_TIMER_MESSAGE * pTM)
{
	switch (pTM->Context)
	{
		case 0x01:
			
			OLED_BufToPanel();
			GUI_TIMER_Restart(pTM->hTimer);
			break;
		default:
			break;
	}
}
extern WM_HWIN CreateDialog(void);
extern GUI_CONST_STORAGE GUI_BITMAP bm0;
__NO_RETURN static void GUIThread (void *argument) {
  (void)argument;

  GUI_Init();           /* Initialize the Graphics Component */

	WM_HTIMER hTimer;
    hTimer = GUI_TIMER_Create(_cbTimer, /* ???? */
						        1,       /* ????,??????? 1ms ?????? */
						        0x01,   /* ???????????????? ID,??????????????? */
						        0);  	
	
	GUI_TIMER_SetPeriod(hTimer, 100);
	
	GUI_SetColor(GUI_WHITE);
	GUI_SetPenSize(2);
	GUI_SetFont(GUI_FONT_32B_1);
	//GUI_SetFont();
	//GUI_DrawLine(0, 0, 127, 63);
	//GUI_DrawLine(0, 63, 127, 0);
//	GUI_DispString("EMei_Li\nemWin Start...\n");
//	GUI_SetTextMode(GUI_TM_REV); 
//	GUI_DispString("This is\nTiny Bord");
//	GUI_SetTextMode(GUI_TM_NORMAL);
//	GUI_DispString(" ");
	
	//GUI_DispString("remove all.\n");
	//CreateDialog();
	//GUI_DrawBitmap(&bm0, 0, 0);
	
  /* Add GUI setup code here */
	//int i = 1;
  while (1) {
    
    /* All GUI related activities might only be called from here */

    GUI_Exec();         /* Execute all GUI jobs ... Return 0 if nothing was done. */
    GUI_X_ExecIdle();   /* Nothing left to do for the moment ... Idle processing */
	  
//	GUI_DrawCircle(64, 32, i*5);
//	i ++;  
//	  
//	  if ( i >= 7 )
//	  {
//		  i = 1;
//		  
//	  }
//	  osDelay(1000);
  }
}


