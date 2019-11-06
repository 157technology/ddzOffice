#include "memory.h"



uint64_t MallocSRAM[MEMORY_SIZE*1024/8];

void MallocInit(void)
{
	osRtxMemoryInit(MallocSRAM, MEMORY_SIZE*1024);
}

mem_head_t * MallocInfo(void)
{
	return MemHeadPtr(MallocSRAM);
}