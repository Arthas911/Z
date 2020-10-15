#ifndef __MEMORY_H__
#define __MEMORY_H__

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ALIGN(x, a)	(((x) + ((a) - 1)) & ~((a) - 1))

/************************************************************
 *   0xc28000000      reserved      reserved
 *   |                |             |
 *   v                v             v
 *   -----------------------------------------------
 *   |       16MB     | 1KB |   4KB | 1KB |   1MB  |
 *   -----------------------------------------------
 *                          ^             ^
 *                          |             |
 *                          zone_mm       chunk_mm
 *
 ***********************************************************/
#define RESERVED_SIZE	    (0x400)      /* 1KB */
#define MALLOC_ADDR	        (0xC2800000) /* chunk memory start addr*/
#define MALLOC_SIZE	        (0x01000000) /* 16MB */
#define ZONE_MM_ADDR	    (MALLOC_ADDR+MALLOC_SIZE+RESERVED_SIZE)
#define ZONE_MM_SIZE	    (0x1000)     /* 4KB zone managerment */
#define CHUNK_MM_ADDR	    (ZONE_MM_ADDR+ZONE_MM_SIZE+RESERVED_SIZE)
#define CHUNK_MM_SIZE	    (0x100000)   /* 1MB chunk managerment */

#define ZONE_OVER_HEAD		0x12345678
#define CHUNK_OVER_HEAD		0x12345679
#define ZONE_NUMS		    64
#define OVER_HEADER_SIZE	4
#define INDEX_SIZE          4
#define MEM_BYTES_4	        (4)
#define MEM_BYTES_8	        (8)
#define MEM_BYTES_16	    (16)
#define MEM_BYTES_32	    (32)
#define MEM_BYTES_64	    (64)
#define MEM_BYTES_128	    (128)
#define MEM_BYTES_256	    (256)
#define MEM_BYTES_512	    (512)
#define MEM_BYTES_1KB	    (1024)
#define MEM_BYTES_2KB	    (2*1024)
#define MEM_BYTES_4KB	    (4*1024)
#define MEM_BYTES_8KB	    (8*1024)
#define MEM_BYTES_16KB	    (16*1024)
#define MEM_BYTES_32KB	    (32*1024)
#define MEM_BYTES_64KB	    (64*1024)
#define MEM_BYTES_128KB	    (128*1024)
#define MEM_BYTES_256KB	    (256*1024)
#define MEM_BYTES_512KB	    (512*1024)
#define MEM_BYTES_1MB	    (1024*1024)

#endif
