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


void creat_signalThread();
void connect(pfun signal, pfun slot);
void emit(pfun signal, void *param);
#endif