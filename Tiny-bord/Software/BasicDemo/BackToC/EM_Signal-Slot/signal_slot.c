#include "signal_slot.h"
#include "memory.h"
#include "cmsis_os2.h"
static SignalList m_Root;

void connect(pfun signal, pfun slot)
{
    SignalList *pi = &m_Root;
    int contain_signal = 0;

    for (pi = &m_Root;; pi = pi->pnext)
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

/**/
#define LEN 10
static Queue signalQueue = {
    0,
    LEN - 1,
    0,
    0,
    NULL};

void EnQueue(Item item)
{
    if (signalQueue.list == NULL)
    {
        signalQueue.list = (Item *)MALLOC(sizeof(Item) * LEN);
    }

    if (!(signalQueue.cur == signalQueue.head && signalQueue.count == LEN-1))
    {
        signalQueue.list[signalQueue.cur] = item;
        signalQueue.count++;
        if (++signalQueue.cur == LEN)
        {
            signalQueue.cur = 0;
        }
    }
}

Item *DeQueue()
{
    if (signalQueue.count == 0)
    {
        return NULL;
    }
    Item *pitem = &signalQueue.list[signalQueue.head];

    if (++signalQueue.head == LEN)
    {
        signalQueue.head = 0;
    }
    if (++signalQueue.tail == LEN)
    {
        signalQueue.tail = 0;
    }
    signalQueue.count--;

    return pitem;
}

static osSemaphoreId_t sem_signal;

void emit(pfun signal, void *param)
{
    Item item = {
        signal,
        param,
    };
    //入队
    EnQueue(item);
    //释放信号量
    osSemaphoreRelease(sem_signal);
}

static const osThreadAttr_t signalThread_attr = {
    .priority = osPriorityNormal1};
void signal_thread(void *argument)
{
    //GUI_DispStringAt("Hello World.......", 0, 18);
    sem_signal = osSemaphoreNew(10U, 0U, NULL);
    SignalList *pi;
    SlotList *ps;
    while (1)
    {
        osSemaphoreAcquire(sem_signal, osWaitForever);

        __disable_irq();
        Item *pitem = DeQueue();
        __enable_irq();

        for (pi = &m_Root;; pi = pi->pnext)
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

                    if ( ps->pnext == NULL )
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

void creat_signalThread()
{
    osThreadNew(signal_thread, NULL, &signalThread_attr);
}