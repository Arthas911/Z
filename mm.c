#include <stdio.h>
#include "type.h"
#include "mm.h"
#include "config.h"
#include "memory.h"
#include "console.h"
#include "rbtree.h"

static struct rb_root root = RB_ROOT;

static u32 zone_mm_addr = ZONE_MM_ADDR;
static u32 zone_mm_size = ZONE_MM_SIZE;
static u32 chunk_mm_addr = CHUNK_MM_ADDR;
static u32 chunk_mm_size = CHUNK_MM_SIZE;
static u32 malloc_addr = MALLOC_ADDR;
static u32 malloc_size = MALLOC_SIZE;

static void* os_malloc(u32 size)
{
	void* addr = (void*)malloc_addr;
	size = ALIGN(size, 4);	
	if(size > malloc_size)
		return NULL;

	malloc_addr += size;
	malloc_size -= size;

	return addr;
}

static void* zone_mm_malloc(u32 size)
{
	void* addr = (void*)zone_mm_addr;
	size = ALIGN(size, 4);	

	if(size > zone_mm_size)
		return NULL;

	zone_mm_addr += size;
	zone_mm_size -= size;

	return addr;
}

static void* chunk_mm_malloc(u32 size)
{
	void* addr = (void*)chunk_mm_addr;
	size = ALIGN(size, 4);	

	if(size > chunk_mm_size)
		return NULL;

	chunk_mm_addr += size;
	chunk_mm_size -= size;

	return addr;
}

static void zone_rb_add(zone_t* node, struct rb_root *root)
{
    struct rb_node **new = &root->rb_node, *parent = NULL;
    u32 chunk_size = node->chunk_size;

    while (*new) {
        parent = *new;
        if (chunk_size < rb_entry(parent, zone_t, rb)->chunk_size)
            new = &parent->rb_left;
        else
            new = &parent->rb_right;
    }

    rb_link_node(&node->rb, parent, new);
    rb_insert_color(&node->rb, root);
}

static void chunk_head_init(chunk_t* c)
{
	INIT_LIST_HEAD(&(c->list));
}

static void chunk_list_add(chunk_t* c, chunk_t* h)
{
    list_add_tail(&(c->list), &(h->list));
}

static void chunk_list_del(chunk_t* c)
{
	list_del(&(c->list));
}

static zone_t* zone_get(u32 size, struct rb_root *root)
{
    zone_t* zone = NULL;
    struct rb_node **new = &root->rb_node;
    struct rb_node* parent;

    while(*new)
    {
            parent = *new;
            zone = rb_entry(parent, zone_t, rb);

            if(size == zone->chunk_size)
            {
                break;
            }else if(size > zone->chunk_size)
            {
                    new = &parent->rb_right;
            }else if(size < zone->chunk_size)
            {
                    new = &parent->rb_left;
            }else
            {
                    break;
            }
    }

	return zone;
}

static chunk_t* chunk_get(chunk_t* head)
{
	chunk_t* pos;
	chunk_t* _pos = NULL;

	list_for_each_entry(pos, &(head->list), list)
	{
		if(pos->status == IS_FREE)
		{
			_pos = pos;
			chunk_list_del(pos);
			break;
		}else
		{
			continue;
		}
	}

	return _pos;
}

void* my_malloc(u32 size)
{
	zone_t* zone;
	chunk_t* chunk;

	size = ALIGN(size, 4);

	zone = zone_get(size, &root);
	if(zone == NULL)
	{
		return NULL;
	}
	if(*(zone->overhead) != ZONE_OVER_HEAD)
	{
	    /* zone out bounds */
	}
	if(zone->free_zone < size)
	{
	    /* zone space use up */
	}

	chunk = chunk_get(zone->c_head);

	if(chunk == NULL)
    {
        /* chunk no space */
        return NULL;
    }

	if(*(chunk->overhead) != CHUNK_OVER_HEAD)
	{
	    /* chunk out bounds */
	}

	chunk->status = IS_ALLOC;
	chunk->malloc++;
	zone->free_zone -= zone->chunk_size;
	os_printf("malloc zone  addr = 0x%x \n", zone);
	os_printf("malloc chunk addr = 0x%x \n", chunk);
	os_printf("malloc start addr = 0x%x \n", chunk->addr);
	return chunk->addr;

}

void my_free(void* addr)
{
    chunk_t* chunk;
    zone_t* zone;
    u32* memory;

    memory = (u32*)addr - INDEX_SIZE;
    chunk = (chunk_t*)(*memory);

    if(*(chunk->overhead) != CHUNK_OVER_HEAD)
    {
        /* out memoey space */
    }
    if(chunk->status != IS_ALLOC)
    {
        /* free more 2 times */
    }

    zone = zone_get(chunk->size, &root);
    if(zone == NULL)
    {
        /* The chunk no in all zone */
    }

    chunk->status = IS_FREE;
    chunk->free++;
    zone->free_zone += zone->chunk_size;
    os_printf("free zone  addr = 0x%x \n", zone);
    os_printf("free chunk addr = 0x%x \n", chunk);
    os_printf("free chunk addr = 0x%x \n", chunk->addr);
    chunk_list_add(chunk, zone->c_head);

}

static u32 zone_init(u32 chunk_size, u32 chunk_num)
{
	zone_t* z_new;
	chunk_t* c_head;
	chunk_t* c_new;
	void* memory;
	u32 chunk;

	z_new = (zone_t*)zone_mm_malloc(sizeof(zone_t));
	if(z_new == NULL)
		return ZONE_MM_USE_UP;

	c_head = (chunk_t*)chunk_mm_malloc(sizeof(chunk_t));
	if(c_head == NULL)
		return CHUNK_MM_USE_UP;

	z_new->overhead = (u32*)os_malloc(OVER_HEADER_SIZE);
	*(z_new->overhead) = ZONE_OVER_HEAD;
	z_new->toal_zone = chunk_size*chunk_num;
	z_new->free_zone = chunk_size*chunk_num;
	z_new->chunk_size = chunk_size;
	z_new->c_head = c_head;
	z_new->status = IS_IDLE;

	chunk_head_init(c_head);
	zone_rb_add(z_new, &root);

	for(chunk = 0; chunk < chunk_num; chunk++)
	{
		c_new = (chunk_t*)chunk_mm_malloc(sizeof(chunk_t));
		if(c_new == NULL)
			return CHUNK_MM_USE_UP;

		memory = os_malloc(chunk_size + OVER_HEADER_SIZE + INDEX_SIZE);
		if(memory == NULL)
			return OS_MALLOC_USE_UP;

		c_new->overhead = memory;
		*(c_new->overhead) = CHUNK_OVER_HEAD;
		c_new->status = IS_FREE;
		c_new->mm = (u32*)memory + OVER_HEADER_SIZE;
		*(u32*)(c_new->mm) = (u32)(c_new);
		c_new->addr = (u32*)memory + OVER_HEADER_SIZE + INDEX_SIZE;
		c_new->size = chunk_size;
		c_new->malloc = 0;
		c_new->free = 0;
		chunk_list_add(c_new, c_head);
	}

	return ZONE_OK;
}

u32 mm_init(void)
{
	u32 z;
	u32 n;

	n = ARRAY_SIZE(memory_table);
	for(z = 0; z < n; z++)
	{
		if(ZONE_OK != zone_init(memory_table[z].c_size, memory_table[z].c_num))
		{
			printf("zone init failed \n");
			return MM_INIT_ERR;
		}
	}
	return MM_INIT_OK;
}

