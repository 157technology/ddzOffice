#ifndef __MEMORY_H__
#define __MEMORY_H__ 

#include "memory_config.h"
#include "rtx_lib.h"


extern uint64_t MallocSRAM[MEMORY_SIZE*1024/8];

#define EM_MALLOC(size)		osRtxMemoryAlloc(MallocSRAM, size, 0)
#define EM_FREE(addr)		osRtxMemoryFree(MallocSRAM, addr)


void MallocInit(void);
mem_head_t * MallocInfo(void);

#endif