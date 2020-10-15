#ifndef __MM_H__
#define __MM_H__

#include "type.h"
#include "list.h"
#include "rbtree.h"

enum mm_error_num
{
	ZONE_OK = 0,
	ZONE_ERR,
	ZONE_MM_USE_UP,
	CHUNK_INIT_OK,
	CHUNK_INIT_ERR,
	CHUNK_MM_USE_UP,
	OS_MALLOC_USE_UP,
	MM_INIT_OK,
	MM_INIT_ERR,
};

enum chunk_status
{
	IS_FREE = 0x12,
	IS_ALLOC,
};

enum zone_status
{
	IS_IDLE = 0x34,
	IS_BUSY,
};

typedef struct mem_chunk
{
	u32* overhead;
	u32 status;
	void* addr;
	u32 size;
	u32* mm;
	u32 malloc;
	u32 free;
	struct list_head list;

}chunk_t;

typedef struct mem_zone
{
	u32* overhead;
	u32 toal_zone;
	u32 free_zone;
	u32 chunk_size;
	u32 status;
    struct rb_node rb;
	struct list_head list;
	chunk_t* c_head;	
}zone_t;

typedef struct mem_table
{
	u32 c_size;
	u32 c_num;
}mem_table_t;

#endif
