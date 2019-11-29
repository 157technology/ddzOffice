#ifndef __MEMORY_H__
#define __MEMORY_H__ 

#include "memory_config.h"

#include <string.h>
#include <stdint.h>
#include "stm32f4xx.h"


//  Memory Pool Header structure
typedef struct {
  uint32_t size;                // Memory Pool size
  uint32_t used;                // Used Memory
} mem_head_t;

//  Memory Block Header structure
typedef struct mem_block_s {
  struct mem_block_s *next;     // Next Memory Block in list
  uint32_t            info;     // Block Info or max used Memory (in last block)
} mem_block_t;

//  Memory Block Info: Length = <31:2>:'00', Type = <1:0>
#define MB_INFO_LEN_MASK        0xFFFFFFFCU     // Length mask
#define MB_INFO_TYPE_MASK       0x00000003U     // Type mask

//  Memory Head Pointer
__STATIC_INLINE mem_head_t *MemHeadPtr (void *mem) {
  //lint -e{9079} -e{9087} "conversion from pointer to void to pointer to other type" [MISRA Note 6]
  return ((mem_head_t *)mem);
}

//  Memory Block Pointer
__STATIC_INLINE mem_block_t *MemBlockPtr (void *mem, uint32_t offset) {
  uint32_t     addr;
  mem_block_t *ptr;

  //lint --e{923} --e{9078} "cast between pointer and unsigned int" [MISRA Note 8]
  addr = (uint32_t)mem + offset;
  ptr  = (mem_block_t *)addr;

  return ptr;
}

//  ==== Library functions ====

// Memory Heap Library functions
extern uint32_t osRtxMemoryInit (void *mem, uint32_t size);
extern void    *osRtxMemoryAlloc(void *mem, uint32_t size, uint32_t type);
extern uint32_t osRtxMemoryFree (void *mem, void *block);





////////////////////////////////////////////////////////////////

extern uint64_t MallocRAM[MEMORY_SIZE*1024/8];

#define MALLOC(size)	osRtxMemoryAlloc(MallocRAM, size, 0)
#define FREE(addr)		osRtxMemoryFree(MallocRAM, addr)
#define RAMINFO()		  MemHeadPtr(MallocRAM)


void MallocInit();



#endif