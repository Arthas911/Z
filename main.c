#include <stdio.h>
#include <string.h>
#include "kfifo.h"

struct test_packet
{
	int a;
	int b;
	int c;
	int d;
};

int main(int argc, char** argv)
{
#if 1
	int i;
	DECLARE_KFIFO_PTR(MY_FIFO, struct test_packet);
	kfifo_alloc(&MY_FIFO, 1024);
	struct test_packet packet_out[4];
	
	struct test_packet packet_in[] = 
	{
		{
			.a = 10,
			.b = 20,
			.c = 30,
			.d = 40,
		},
		{
			.a = 11,
			.b = 21,
			.c = 31,
			.d = 41,
		
		},
		{
			.a = 12,
			.b = 22,
			.c = 32,
			.d = 42,
		},
		{
			.a = 13,
			.b = 23,
			.c = 33,
			.d = 43,
		},
		
	};

	kfifo_put(&MY_FIFO, packet_in[0]);
	kfifo_put(&MY_FIFO, packet_in[1]);
	kfifo_put(&MY_FIFO, packet_in[2]);
	kfifo_put(&MY_FIFO, packet_in[3]);

	kfifo_get(&MY_FIFO, &packet_out[0]);
	kfifo_get(&MY_FIFO, &packet_out[1]);
	kfifo_get(&MY_FIFO, &packet_out[2]);
	kfifo_get(&MY_FIFO, &packet_out[3]);

	for(i = 0; i < 4; i++)
	{
		printf("a = %d \n", packet_out[i].a);
		printf("b = %d \n", packet_out[i].b);
		printf("c = %d \n", packet_out[i].c);
		printf("d = %d \n", packet_out[i].d);
	}
	
	kfifo_free(&MY_FIFO);	
	return 0;

#else	
#if 0	
	DECLARE_KFIFO(MY_FIFO, char, 1024);
	INIT_KFIFO(MY_FIFO);	
#else
	DECLARE_KFIFO_PTR(MY_FIFO, char);
	kfifo_alloc(&MY_FIFO, 1024);
#endif	
	int i;
	int len;
	char buff_out[64];
	char buff_in[64];
	for(i = 0; i < sizeof(buff_in); i++)
	{
		buff_in[i] = i;
	}

	memset(buff_out , 0, sizeof(buff_out));
	kfifo_in(&MY_FIFO, buff_in, sizeof(buff_in));

	len = kfifo_out(&MY_FIFO, buff_out, 34);

	for(i = 0; i < len; i++)
	{
		if((i != 0)&&(((i%16) == 0)))
		{
			printf("\n");
		}
		printf("%02d ", buff_out[i]);
	}

	printf("\n");
	kfifo_free(&MY_FIFO);	
	return 0;
#endif
}
