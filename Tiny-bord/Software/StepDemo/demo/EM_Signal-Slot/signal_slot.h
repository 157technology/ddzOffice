/*
 * @Author: your name
 * @Date: 2020-01-14 10:42:46
 * @LastEditTime : 2020-01-14 16:29:59
 * @LastEditors  : Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \demo\EM_Signal-Slot\signal_slot.h
 */
#ifndef __SIGNAL_SLOT_H__
#define __SIGNAL_SLOT_H__

typedef void (* pfun) (void *);

typedef struct __slot
{
    pfun pslot;
    struct __slot *pnext;
} SlotList;

typedef struct __signal
{
    pfun psignal;
    SlotList *slots;
} Signal;

typedef struct __signals
{
    Signal *psignal;
    struct __signals *pnext;
} SignalList;




/*###########################################*/
typedef struct __item
{
    pfun psignal;
    void * param;
} Item;

typedef struct __queue
{
    int head;
    int tail;
    int cur;
    int count;
    Item * list; 
} Queue;


typedef enum __connecttype {
    DirectConnect = 0,
    QueuedConnect,
    BlockingQueuedConnect
}ConnectType;

void creat_signalThread();
void connect(pfun signal, pfun slot, ConnectType type);
void emit(pfun signal, void *param);
#endif