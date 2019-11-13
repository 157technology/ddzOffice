#include "memory.h"


uint64_t MallocRAM[MEMORY_SIZE*1024/8];

void MallocInit()
{
	osRtxMemoryInit(MallocRAM, MEMORY_SIZE*1024);
}

