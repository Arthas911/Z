/*
 * main.c
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
#include <stdlib.h>
#include <string.h>
#include "config.h"

static int do_ls(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	printf("argc = %d \n", argc);
	return 0;
}	
int main(int argc, char** argv)
{
	char cmd[CONFIG_SYS_CBSIZE];
	char* buff = NULL;
	cmd_init();
	BOOT_CMD(mkdir, 0, NULL, "This is dummy mkdir command.");
	BOOT_CMD(mknode, 0, NULL, "This is dummy mknode command.");
	BOOT_CMD(ls, 3, do_ls, "This is dummy ls command.");
	BOOT_CMD(cd, 0, NULL, "This is dummy cd command.");
	BOOT_CMD(ps, 0, NULL, "This is dummy ps command.");
	BOOT_CMD(ifconfig, 0, NULL, "This is dummy ifconfig command.");
	BOOT_CMD(run, 0, NULL, "This is dummy run command.");
	
	while(1)
	{
		buff = readline(CONFIG_SYS_PROMPT);
		if(buff != NULL)
		{
			strcpy(cmd, buff);
			run_cmd(cmd);
		}else
		{
			printf("ERROR \n");
		}
	}
}
