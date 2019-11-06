
#include "cmsis_os2.h"
#include "GUI.h"
#include "FRAMEWIN.h"

static int XPos;
static const char aText[] = "Moving text...";

static void _cbCallbackT0(WM_MESSAGE * pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      /* Handle the paint message */
      GUI_SetBkColor(GUI_RED);
      GUI_SetColor(GUI_BLACK);
      GUI_SetFont(&GUI_FontComic24B_ASCII);
      GUI_Clear();
      GUI_DispStringAt(aText, XPos, 0);
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}

static void _cbCallbackT1(WM_MESSAGE * pMsg) {
  WM_HWIN hWin = (FRAMEWIN_Handle)(pMsg->hWin);
  switch (pMsg->MsgId) {
    case WM_PAINT:
      /* Handle the paint message */
      GUI_SetBkColor(GUI_BLUE);
      GUI_SetColor(GUI_WHITE);
      GUI_SetFont(&GUI_FontComic24B_ASCII);
      GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
      GUI_Clear();
      GUI_DispStringHCenterAt("Moving window...", 
                              WM_GetWindowSizeX(hWin) / 2, 
                              WM_GetWindowSizeY(hWin) / 2);
      break;
    default:
      WM_DefaultProc(pMsg);
  }
}

static void _cbBackgroundWin(WM_MESSAGE* pMsg) {
  switch (pMsg->MsgId) {
    case WM_PAINT:
      /* Handle only the paint message */
      GUI_SetBkColor(0x00CC00);
      GUI_Clear();
      GUI_SetFont(&GUI_Font24_ASCII);
      GUI_DispStringHCenterAt("emWin - multitasking demo\n", 160, 5);
      GUI_SetFont(&GUI_Font13_1);
      GUI_DispStringAt("Scrolling text and moving windows without flickering", 5, 35);
    default:
      WM_DefaultProc(pMsg);
  }
}

__NO_RETURN static void _Task_0(void *argument) {
  FRAMEWIN_Handle hFrameWin;
  WM_HWIN         hChildWin;

  (void)argument;

  /* Create frame window */
  hFrameWin = FRAMEWIN_Create("Task 0",  NULL, WM_CF_SHOW | WM_CF_STAYONTOP,  0, 70, 200, 40);
  /* Create child window */
  hChildWin = WM_CreateWindowAsChild(0, 0, 0, 0, WM_GetClientWindow(hFrameWin), 
                                     WM_CF_SHOW | WM_CF_MEMDEV,  _cbCallbackT0, 0);
  FRAMEWIN_SetActive(hFrameWin, 0);
  /* Make sure the right window is active... */
  WM_SelectWindow(hChildWin);
  /* ...and the right font is selected */
  GUI_SetFont(&GUI_FontComic24B_ASCII);
  while (1) {
    GUI_RECT Rect;
    int XLen = GUI_GetStringDistX(aText);   /* Get the length of the string */
    WM_GetClientRect(&Rect);                /* Get the size of the window */
    /* Show moving text */
    for (XPos = 0; XPos < (Rect.x1 - Rect.x0) - XLen; XPos++) {
      WM_InvalidateWindow(hChildWin);
      osDelay(50U);
    }
    for (; XPos >= 0; XPos--) {
      WM_InvalidateWindow(hChildWin);
      osDelay(100U);
    }
  }
}

__NO_RETURN static void _Task_1(void *argument) {
  FRAMEWIN_Handle hFrameWin;

  (void) argument;

  /* Create frame window */
  hFrameWin = FRAMEWIN_Create("Task 1", NULL, WM_CF_SHOW | WM_CF_STAYONTOP, 
                              20, 170, 200, 40);
  /* Create child window */
  WM_CreateWindowAsChild(0, 0, 0, 0, WM_GetClientWindow(hFrameWin), WM_CF_SHOW | WM_CF_MEMDEV,
                         _cbCallbackT1, 0);
  FRAMEWIN_SetActive(hFrameWin, 0);
  while (1) {
    int i;
    int nx = 80;
    int ny = 90;
    /* Move window continously */
    for (i = 0; i < ny; i++) {
      WM_MoveWindow(hFrameWin, 0, -2);
      osDelay(50U);
    }
    for (i = 0; i < nx; i++) {
      WM_MoveWindow(hFrameWin, 2, 0);
      osDelay(50U);
    }
    for (i = 0; i < ny; i++) {
      WM_MoveWindow(hFrameWin, 0, 2);
      osDelay(50U);
    }
    for (i = 0; i < nx; i++) {
      WM_MoveWindow(hFrameWin, -2, 0);
      osDelay(50U);
    }
  }
}


static void     GUIThread(void *argument);
static uint64_t GUIThread_stk[2000/8];
static const osThreadAttr_t GUIThread_attr = {
  .stack_mem  = &GUIThread_stk[0],
  .stack_size = sizeof(GUIThread_stk),
  .priority   = osPriorityIdle
};
static uint64_t _Task_0_stk[600/8];
static const osThreadAttr_t _Task_0_attr = {
  .stack_mem  = &_Task_0_stk[0],
  .stack_size = sizeof(_Task_0_stk),
  .priority   = osPriorityAboveNormal
};
static uint64_t _Task_1_stk[600/8];
static const osThreadAttr_t _Task_1_attr = {
  .stack_mem  = &_Task_1_stk[0],
  .stack_size = sizeof(_Task_1_stk),
  .priority   = osPriorityNormal
};


/*----------------------------------------------------------------------------
 *     Init_GUIThread
 *
 * This function initializes the Graphics component and starts 2 demo threads.
 *---------------------------------------------------------------------------*/

void Init_GUIThread(void) {
  /* Init GUI */
  GUI_Init();
  WM_SetCreateFlags(WM_CF_MEMDEV);              /* Use memory devices on all windows to avoid flicker */
  WM_SetCallback(WM_HBKWIN, _cbBackgroundWin);  /* Set callback for background window */
  /* Create tasks */
  osThreadNew(GUIThread, NULL, &GUIThread_attr);
  osThreadNew(_Task_0, NULL, &_Task_0_attr);
  osThreadNew(_Task_1, NULL, &_Task_1_attr);
}


/*----------------------------------------------------------------------------
 *     GUIThread
 *
 * This task does the background processing.
 * The GUIThread job is to update invalid windows, but other things such as
 * evaluating mouse or touch input may also be done.
 *---------------------------------------------------------------------------*/
__NO_RETURN static void GUIThread(void *argument) {
  (void)argument;

  while (1) {
    GUI_Exec();         /* Do the background work ... Update windows etc.) */
    GUI_X_ExecIdle();   /* Nothing left to do for the moment ... Idle processing */
  }
}
