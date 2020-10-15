#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include "interface.h"


void make_cmd(int argc, char** argv, char* cmd)
{
	switch(argc)
	{
		case 2:
		sprintf(cmd, "%s\n", argv[1]);
		break;
		case 3:
		sprintf(cmd, "%s %s\n", argv[1], argv[2]);
		break;
		case 4:
		sprintf(cmd, "%s %s %s\n", argv[1], argv[2], argv[3]);
		break;
		case 5:
		sprintf(cmd, "%s %s %s %s\n", argv[1], argv[2], argv[3], argv[4]);
		break;
		case 6:
		sprintf(cmd, "%s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4], argv[5]);
		break;
		case 7:
		sprintf(cmd, "%s %s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
		break;
		case 8:
		sprintf(cmd, "%s %s %s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6] \
			, argv[7]);
		break;
		case 9:
		sprintf(cmd, "%s %s %s %s %s %s %s, %s\n", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6] \
			, argv[7], argv[8]);
		break;
		case 10:
		sprintf(cmd, "%s %s %s %s %s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6] \
			, argv[7], argv[8], argv[9]);
		break;
		case 11:
		sprintf(cmd, "%s %s %s %s %s %s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6] \
			, argv[7], argv[8], argv[9], argv[10]);
		break;
		case 12:
		sprintf(cmd, "%s %s %s %s %s %s %s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6] \
			, argv[7], argv[8], argv[9], argv[10], argv[11]);
		break;
		case 13:
		sprintf(cmd, "%s %s %s %s %s %s %s %s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4], argv[5], argv[6] \
			, argv[7], argv[8], argv[9], argv[10], argv[11], argv[12]);
		break;
		case 14:
		sprintf(cmd, "%s %s %s %s %s %s %s %s %s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4] \
			, argv[5], argv[6] , argv[7], argv[8], argv[9], argv[10], argv[11], argv[12], argv[13]);
		break;
		case 15:
		sprintf(cmd, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4] \
			, argv[5], argv[6] , argv[7], argv[8], argv[9], argv[10], argv[11], argv[12], argv[13], argv[14]);
		break;
		case 16:
		sprintf(cmd, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n", argv[1], argv[2], argv[3], argv[4] \
			, argv[5], argv[6] , argv[7], argv[8], argv[9], argv[10], argv[11], argv[12], argv[13], argv[14], argv[15]);
		break;
		default:
		printf("argc = %d err \n", argc);
		break;
	}

}

int main(int argc, char** argv)
{
	int fd;
	int l;
	int i;
	int ret;
	char cmd[1024] = {0};
	char rcv_buf[8192] = {0};
	
	if(argc == 1)
	{
		printf("parameter must be > 1 \n");
		return 1;
	}

	make_cmd(argc, argv, cmd);
#if 1	
	fd = serial_init("ttyUSB0");
	if(fd < 0)
	{
		printf("full_init failed \n");
		return 1;
	}

	//ret = serial_config(fd, BAUDRATE_9600, DATABIT_8, STOP_BIT_1, NONE_CHECK);
	ret = serial_config(fd, BAUDRATE_115200, DATABIT_8, STOP_BIT_1, NONE_CHECK);
	if(ret < 0)
	{
		serial_exit(fd);
		return 1;
	}
	//usleep(10000);	
	serial_write(fd, cmd, strlen(cmd));
	//sleep(2);
	l = serial_read(fd, rcv_buf, sizeof(rcv_buf));
	serial_exit(fd);
	rcv_buf[l+1] = '\0';
	printf("recv len = %d \n", l);
	for(i = 0; i < l; i++)
	{
		printf("%c", rcv_buf[i]);
	}
	//printf("rcv_buf: %s \n", rcv_buf);
#endif
	
	return 0;
}
