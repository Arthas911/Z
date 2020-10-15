#ifndef __MM_CONFIG_H__
#define __MM_CONFIG_H__

#include "memory.h"

mem_table_t memory_table[] = 
{
	{MEM_BYTES_4, 100},
	{MEM_BYTES_8, 100},
	{MEM_BYTES_16, 60},
	{MEM_BYTES_32, 80},
	{MEM_BYTES_64, 40},
	{MEM_BYTES_128, 60},
	{MEM_BYTES_256, 80},
	{MEM_BYTES_512, 100},
	{MEM_BYTES_1KB, 10},
	{MEM_BYTES_2KB, 100},
	{MEM_BYTES_4KB, 1},
	{MEM_BYTES_8KB, 1},
	{MEM_BYTES_16KB, 1},
	{MEM_BYTES_32KB, 1},
	{MEM_BYTES_64KB, 1},
	{MEM_BYTES_128KB, 1},
	{MEM_BYTES_256KB, 1},
	{MEM_BYTES_512KB, 1},
	{MEM_BYTES_1MB, 1},

};

#endif
