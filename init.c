/************************************************************************
 * copyright (c) 2016-2020 private code
 * File: init.c
 * Author:     
 * Description: serial init.
 * History:    2019-11-26 09:51
 ************************************************************************/
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <unistd.h>

/********************************************************************
 * FuncName:serial_init
 * FuncDesc:init ttyUSBx.
 * Input   :example ttyUSB0, ttyUSB1, ttyUSB2  
 * Output  :none
 * Return  : OK:return serial Descriptor ERR: -1
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_init(const char* tty_name)
{
	int fd;
	char s[64] = {0};

	if(tty_name == NULL)
	{
		return -1;
	}

	sprintf(s, "/dev/%s", tty_name);

	fd = open(s, O_RDWR | O_NONBLOCK | O_NOCTTY);
	if(fd < 0)
	{
		printf("open %s failed \n", s);
		return -1;
	}

	return fd;
}

/********************************************************************
 * FuncName:serial_exit
 * FuncDesc:serial safe exit when code end.
 * Input   :a valid serial Descriptor.  
 * Output  :none
 * Return  : OK: 0 
 * Date    :2019-11-26
 * Modify  :create
 ********************************************************************/
ssize_t serial_exit(int fd)
{
	int ret = 0;

	if(fd > 0)
		close(fd);

	return ret;
}
