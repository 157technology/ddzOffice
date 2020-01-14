#include "signal_slot.h"
#include "../EM_Device/memory.h"
#include "cmsis_os2.h"

static SignalList m_Root[3];
/*
 *  
*/
void connect(pfun signal, pfun slot, ConnectType type)
{
    SignalList *root = m_Root + type;
    SignalList *pi;
    int contain_signal = 0;

    for (pi = root;; pi = pi->pnext)
    {
        if (pi->psignal->psignal == signal)
        {
            contain_signal = 1;
            // 如果存在该信号, 将槽函数添加进槽函数列表
            SlotList *t_slot = (SlotList *)MALLOC(sizeof(SlotList));
            SlotList *ti = pi->psignal->slots;
            t_slot->pslot = slot;
            t_slot->pnext = NULL;

            while (ti->pnext != NULL)
                ti = ti->pnext;
            ti->pnext = t_slot;
            return;
        }

        if (pi->pnext == NULL)
            break;
    }

    if (contain_signal == 0)
    { //add
        SlotList *t_slot = (SlotList *)MALLOC(sizeof(SlotList));
        Signal *t_signal = (Signal *)MALLOC(sizeof(Signal));
        SignalList *t_list = (SignalList *)MALLOC(sizeof(SignalList));
        t_slot->pslot = slot;
        t_slot->pnext = NULL;

        t_signal->psignal = signal;
        t_signal->slots = t_slot;

        t_list->psignal = t_signal;
        t_list->pnext = NULL;

        pi->pnext = t_list;
    }
}

/*queue*/
#define LEN 10
static Queue signalQueue_H = {
    0,
    LEN - 1,
    0,
    0,
    NULL};
static Queue signalQueue_N = {
    0,
    LEN - 1,
    0,
    0,
    NULL};
static Queue signalQueue_L = {
    0,
    LEN - 1,
    0,
    0,
    NULL};

void EnQueue_H(Item item)
{
    if (signalQueue_H.list == NULL)
    {
        signalQueue_H.list = (Item *)MALLOC(sizeof(Item) * LEN);
    }

    if (!(signalQueue_H.cur == signalQueue_H.head && signalQueue_H.count == LEN - 1))
    {
        signalQueue_H.list[signalQueue_H.cur] = item;
        signalQueue_H.count++;
        if (++signalQueue_H.cur == LEN)
        {
            signalQueue_H.cur = 0;
        }
    }
}
void EnQueue_N(Item item)
{
    if (signalQueue_N.list == NULL)
    {
        signalQueue_N.list = (Item *)MALLOC(sizeof(Item) * LEN);
    }

    if (!(signalQueue_N.cur == signalQueue_N.head && signalQueue_N.count == LEN - 1))
    {
        signalQueue_N.list[signalQueue_N.cur] = item;
        signalQueue_N.count++;
        if (++signalQueue_N.cur == LEN)
        {
            signalQueue_N.cur = 0;
        }
    }
}
void EnQueue_L(Item item)
{
    if (signalQueue_L.list == NULL)
    {
        signalQueue_L.list = (Item *)MALLOC(sizeof(Item) * LEN);
    }

    if (!(signalQueue_L.cur == signalQueue_L.head && signalQueue_L.count == LEN - 1))
    {
        signalQueue_L.list[signalQueue_L.cur] = item;
        signalQueue_L.count++;
        if (++signalQueue_L.cur == LEN)
        {
            signalQueue_L.cur = 0;
        }
    }
}

Item *DeQueue_H()
{
    if (signalQueue_H.count == 0)
    {
        return NULL;
    }
    Item *pitem = &signalQueue_H.list[signalQueue_H.head];

    if (++signalQueue_H.head == LEN)
    {
        signalQueue_H.head = 0;
    }
    if (++signalQueue_H.tail == LEN)
    {
        signalQueue_H.tail = 0;
    }
    signalQueue_H.count--;

    return pitem;
}
Item *DeQueue_N()
{
    if (signalQueue_N.count == 0)
    {
        return NULL;
    }
    Item *pitem = &signalQueue_N.list[signalQueue_N.head];

    if (++signalQueue_N.head == LEN)
    {
        signalQueue_N.head = 0;
    }
    if (++signalQueue_N.tail == LEN)
    {
        signalQueue_N.tail = 0;
    }
    signalQueue_N.count--;

    return pitem;
}
Item *DeQueue_L()
{
    if (signalQueue_L.count == 0)
    {
        return NULL;
    }
    Item *pitem = &signalQueue_L.list[signalQueue_L.head];

    if (++signalQueue_L.head == LEN)
    {
        signalQueue_L.head = 0;
    }
    if (++signalQueue_L.tail == LEN)
    {
        signalQueue_L.tail = 0;
    }
    signalQueue_L.count--;

    return pitem;
}
/**/

static osSemaphoreId_t sem_signal_H;
static osSemaphoreId_t sem_signal_N;
static osSemaphoreId_t sem_signal_L;

void emit(pfun signal, void *param)
{
    Item item = {
        signal,
        param,
    };
    //入队
    EnQueue_H(item);
    EnQueue_N(item);
    EnQueue_L(item);
    //释放信号量
    osSemaphoreRelease(sem_signal_H);
    osSemaphoreRelease(sem_signal_N);
    osSemaphoreRelease(sem_signal_L);
}

void signal_slotThread_H(void *argument)
{
    //GUI_DispStringAt("Hello World.......", 0, 18);
    sem_signal_H = osSemaphoreNew(10U, 0U, NULL);
    SignalList *pi;
    SlotList *ps;
    while (1)
    {
        osSemaphoreAcquire(sem_signal_H, osWaitForever);

        __disable_irq();
        Item *pitem = DeQueue_H();
        __enable_irq();

        for (pi = m_Root;; pi = pi->pnext)
        {
            if (pi->psignal->psignal == pitem->psignal)
            {
                // 如果存在该信号, 依次执行相关槽函数
                ps = pi->psignal->slots;
                while (1)
                {

                    if (ps->pslot == NULL)
                    {
                        break;
                    }
                    // 调用槽函数
                    ps->pslot(pitem->param);

                    if (ps->pnext == NULL)
                    {
                        break;
                    }
                    else
                    {
                        ps = ps->pnext;
                    }
                }
                break;
            }
        }
    }
}

void signal_slotThread_N(void *argument)
{
    //GUI_DispStringAt("Hello World.......", 0, 18);
    sem_signal_N = osSemaphoreNew(10U, 0U, NULL);
    SignalList *pi;
    SlotList *ps;
    while (1)
    {
        osSemaphoreAcquire(sem_signal_N, osWaitForever);

        __disable_irq();
        Item *pitem = DeQueue_N();
        __enable_irq();

        for (pi = m_Root + 1; ; pi = pi->pnext)
        {
            if (pi->psignal->psignal == pitem->psignal)
            {
                // 如果存在该信号, 依次执行相关槽函数
                ps = pi->psignal->slots;
                while (1)
                {

                    if (ps->pslot == NULL)
                    {
                        break;
                    }
                    // 调用槽函数
                    ps->pslot(pitem->param);

                    if (ps->pnext == NULL)
                    {
                        break;
                    }
                    else
                    {
                        ps = ps->pnext;
                    }
                }
                break;
            }
			if ( pi->pnext == NULL )
				break;
        }
    }
}

void signal_slotThread_L(void *argument)
{
    //GUI_DispStringAt("Hello World.......", 0, 18);
    sem_signal_L = osSemaphoreNew(10U, 0U, NULL);
    SignalList *pi;
    SlotList *ps;
    while (1)
    {
        osSemaphoreAcquire(sem_signal_L, osWaitForever);

        __disable_irq();
        Item *pitem = DeQueue_L();
        __enable_irq();

        for (pi = m_Root + 2;; pi = pi->pnext)
        {
            if (pi->psignal->psignal == pitem->psignal)
            {
                // 如果存在该信号, 依次执行相关槽函数
                ps = pi->psignal->slots;
                while (1)
                {

                    if (ps->pslot == NULL)
                    {
                        break;
                    }
                    // 调用槽函数
                    ps->pslot(pitem->param);

                    if (ps->pnext == NULL)
                    {
                        break;
                    }
                    else
                    {
                        ps = ps->pnext;
                    }
                }
                break;
            }
						if ( pi->pnext == NULL )
				break;
        }
    }
}

void creat_signalThread()
{
    osThreadAttr_t signalThread_attr = {
        .priority = osPriorityHigh};
    osThreadNew(signal_slotThread_H, NULL, &signalThread_attr);
    signalThread_attr.priority = osPriorityNormal;
    osThreadNew(signal_slotThread_N, NULL, &signalThread_attr);
    signalThread_attr.priority = osPriorityLow;
    osThreadNew(signal_slotThread_L, NULL, &signalThread_attr);
}