
#include "cmsis_os2.h"
#include "GUI.h"

//
#include "DIALOG.h"
#include "oled.h"
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
			
			OLED_Replot();
			GUI_TIMER_Restart(pTM->hTimer);
			break;
		default:
			break;
	}
}

extern WM_HWIN CreateWindow(void);
WM_HWIN myWin;
__NO_RETURN static void GUIThread (void *argument) {
  (void)argument;

  GUI_Init();           /* Initialize the Graphics Component */
	WM_HTIMER hTimer;
    hTimer = GUI_TIMER_Create(_cbTimer, /* ???? */
						        1,       /* ????,??????? 1ms ?????? */
						        0x01,   /* ???????????????? ID,??????????????? */
						        0);  	
	
	GUI_TIMER_SetPeriod(hTimer, 100);
	
	//GUI_DispString("Hello World");
	//OLED_BufToPanel();
  /* Add GUI setup code here */
	myWin = CreateWindow();
  while (1) {
    
    /* All GUI related activities might only be called from here */

    GUI_Exec();         /* Execute all GUI jobs ... Return 0 if nothing was done. */
    GUI_X_ExecIdle();   /* Nothing left to do for the moment ... Idle processing */
  }
}
