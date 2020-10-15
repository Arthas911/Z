/*
 * console.c
 * transplant by ztm
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (Version 2) as
 * published by the Free Software Foundation.
 *
 * date: 2020-04-02
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void putch(char ch)
{
	putchar(ch);
}

void putstr(const char* s)
{
	while(*s)
	{
		putch(*s++);
	}
}

char getch(void)
{
	int ret;
	char c;
	
	ret = system("stty -echo");
	ret = system("stty -icanon");
	c = getchar();
	ret = system("stty icanon");
	ret = system("stty echo");
	if(ret < 0)
	{
		printf("system err\n");
	}	
	return c;
}
