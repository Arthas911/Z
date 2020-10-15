/*
 * command.c
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
#include "config.h"
#include "list.h"

static cmd_tbl_t *find_cmd (const char *name);

static cmd_tbl_t 	CMD_HEAD;

enum command_ret_t {
	CMD_RET_SUCCESS,	/* 0 = Success */
	CMD_RET_FAILURE,	/* 1 = Failure */
	CMD_RET_USAGE = -1,	/* Failure, please report 'usage' error */
};

static int _cmd_head_init(cmd_tbl_t* head)
{
	INIT_LIST_HEAD(&(head->list));
	return 0;
}

static int _cmd_add(cmd_tbl_t* head, cmd_tbl_t* cmd)
{
	list_add(&(cmd->list), &(head->list));
	return 0;
}

static int _cmd_add_prev(cmd_tbl_t* head, cmd_tbl_t* cmd)
{
	list_add_tail(&(cmd->list), &(head->list));
	return 0;
}

static int _register_cmd(cmd_tbl_t* head, cmd_tbl_t* cmd)
{
	int ret = 0;
	cmd_tbl_t* pos;

	if(list_empty(&(head->list)) == 1)
	{
		return _cmd_add(head, cmd);
	}else
	{
		list_for_each_entry(pos, &(head->list), list)
		{
			ret = strcmp(cmd->name, pos->name);
			if(ret == 0)
			{
				/* cmd exist */
				return -1;
			}
			else if(ret > 0)
			{
				if(list_is_last(&(pos->list), &(head->list)) == 1)
				{
					return _cmd_add(pos, cmd);
				}else
				{
					continue;
				}	
			}else
			{
				return _cmd_add_prev(pos, cmd);
			}
		}
	}

	return -1;
}

int register_cmd(cmd_tbl_t* cmd)
{
	return _register_cmd(&CMD_HEAD, cmd);
}

int cmd_usage(const cmd_tbl_t *cmdtp)
{
	cli_printf("%s - %s\n\n", cmdtp->name, cmdtp->help);

	return 1;
}

/*************************************************************************************************/
#ifdef CONFIG_AUTO_COMPLETE

static cmd_tbl_t* cmd_entry_start(void)
{
	return &CMD_HEAD;
}

static int complete_cmdv(int argc, char * const argv[], char last_char, int maxv, char *cmdv[])
{
	cmd_tbl_t* pos = NULL;
	cmd_tbl_t *cmdtp = cmd_entry_start();
	const char *p;
	int len, clen;
	int n_found = 0;
	const char *cmd;
	/* sanity? */
	if (maxv < 2)
		return -2;

	cmdv[0] = NULL;

	if (argc == 0) {
		/* output full list of commands */
		list_for_each_entry(pos, &(cmdtp->list), list)
		{
			if (n_found >= maxv - 2) {
				cmdv[n_found] = "...";
				break;
			}
			cmdv[n_found] = pos->name;
			continue;
		}

		cmdv[n_found] = NULL;
		return n_found;
	}

	pos = NULL;
	/* more than one arg or one but the start of the next */
	if (argc > 1 || (last_char == '\0' || isblank(last_char))) {
		pos = find_cmd(argv[0]);
		if (pos == NULL || pos->complete == NULL) {
			cmdv[0] = NULL;
			return 0;
		}
		return (*pos->complete)(argc, argv, last_char, maxv, cmdv);
	}

	cmd = argv[0];
	/*
	 * Some commands allow length modifiers (like "cp.b");
	 * compare command name only until first dot.
	 */
	p = strchr(cmd, '.');
	if (p == NULL)
		len = strlen(cmd);
	else
		len = p - cmd;

	pos = NULL;
	list_for_each_entry(pos, &(cmdtp->list), list)
	{
		clen = strlen(pos->name);
		if (clen < len)
			continue;

		if (memcmp(cmd, pos->name, len) != 0)
			continue;

		/* too many! */
		if (n_found >= maxv - 2) {
			cmdv[n_found++] = "...";
			break;
		}
		cmdv[n_found++] = pos->name;
	}

	cmdv[n_found] = NULL;
	return n_found;
}

static int make_argv(char *s, int argvsz, char *argv[])
{
	int argc = 0;

	/* split into argv */
	while (argc < argvsz - 1) {

		/* skip any white space */
		while (isblank(*s))
			++s;

		if (*s == '\0')	/* end of s, no more args	*/
			break;

		argv[argc++] = s;	/* begin of argument string	*/

		/* find end of string */
		while (*s && !isblank(*s))
			++s;

		if (*s == '\0')		/* end of s, no more args	*/
			break;

		*s++ = '\0';		/* terminate current arg	 */
	}
	argv[argc] = NULL;

	return argc;
}

static void print_argv(const char *banner, const char *leader, const char *sep, int linemax, char * const argv[])
{
	int ll = leader != NULL ? strlen(leader) : 0;
	int sl = sep != NULL ? strlen(sep) : 0;
	int len, i;

	if (banner) {
		getcmd_putstr("\n");
		getcmd_putstr(banner);
	}

	i = linemax;	/* force leader and newline */
	while (*argv != NULL) {
		len = strlen(*argv) + sl;
		if (i + len >= linemax) {
			getcmd_putstr("\n");
			if (leader)
				getcmd_putstr(leader);
			i = ll - sl;
		} else if (sep)
			getcmd_putstr(sep);
		getcmd_putstr(*argv++);
		i += len;
	}
	printf("\n");
}

static int find_common_prefix(char * const argv[])
{
	int i, len;
	char *anchor, *s, *t;

	if (*argv == NULL)
		return 0;

	/* begin with max */
	anchor = *argv++;
	len = strlen(anchor);
	while ((t = *argv++) != NULL) {
		s = anchor;
		for (i = 0; i < len; i++, t++, s++) {
			if (*t != *s)
				break;
		}
		len = s - anchor;
	}
	return len;
}

static char tmp_buf[CONFIG_SYS_CBSIZE];	/* copy of console I/O buffer	*/

int cmd_auto_complete(const char *const prompt, char *buf, int *np, int *colp)
{
	int n = *np, col = *colp;
	char *argv[CONFIG_SYS_MAXARGS + 1];		/* NULL terminated	*/
	char *cmdv[20];
	char *s, *t;
	const char *sep;
	int i, j, k, len, seplen, argc;
	int cnt;
	char last_char;

	if (strcmp(prompt, CONFIG_SYS_PROMPT) != 0)
		return 0;	/* not in normal console */

	cnt = strlen(buf);
	if (cnt >= 1)
		last_char = buf[cnt - 1];
	else
		last_char = '\0';

	/* copy to secondary buffer which will be affected */
	strcpy(tmp_buf, buf);

	/* separate into argv */
	argc = make_argv(tmp_buf, sizeof(argv)/sizeof(argv[0]), argv);

	/* do the completion and return the possible completions */
	i = complete_cmdv(argc, argv, last_char, sizeof(cmdv)/sizeof(cmdv[0]), cmdv);

	/* no match; bell and out */
	if (i == 0) {
		if (argc > 1)	/* allow tab for non command */
			return 0;
		getcmd_putch('\a');
		return 1;
	}

	s = NULL;
	len = 0;
	sep = NULL;
	seplen = 0;
	if (i == 1) { /* one match; perfect */
		k = strlen(argv[argc - 1]);
		s = cmdv[0] + k;
		len = strlen(s);
		sep = " ";
		seplen = 1;
	} else if (i > 1 && (j = find_common_prefix(cmdv)) != 0) {	/* more */
		k = strlen(argv[argc - 1]);
		j -= k;
		if (j > 0) {
			s = cmdv[0] + k;
			len = j;
		}
	}

	if (s != NULL) {
		k = len + seplen;
		/* make sure it fits */
		if (n + k >= CONFIG_SYS_CBSIZE - 2) {
			getcmd_putch('\a');
			return 1;
		}

		t = buf + cnt;
		for (i = 0; i < len; i++)
			*t++ = *s++;
		if (sep != NULL)
			for (i = 0; i < seplen; i++)
				*t++ = sep[i];
		*t = '\0';
		n += k;
		col += k;
		getcmd_putstr(t - k);
		if (sep == NULL)
			getcmd_putch('\a');
		*np = n;
		*colp = col;
	} else {
		print_argv(NULL, "  ", " ", 78, cmdv);

		getcmd_putstr(prompt);
		getcmd_putstr(buf);
	}
	return 1;
}
#endif /* CONFIG_AUTO_COMPLETE */
/*************************************************************************************************/

static int  _cmd_show(const cmd_tbl_t* head)
{
	cmd_tbl_t* pos;
	list_for_each_entry(pos, &(head->list), list)
	{
		cmd_usage(pos);
	}

	return 0;
}

static int cmd_show(void)
{
	_cmd_show(&CMD_HEAD);
	return 0;
}

static int do_help(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	cmd_tbl_t* pos = NULL;

	if(0 == strcmp(argv[0], "help"))
	{
		if(argc == 1)
		{
			cmd_show();
		}
		else
		{
			pos = find_cmd(argv[1]);
			if(pos != NULL)
				cmd_usage(pos);
		}
	}else
	{
		cli_printf("command err. \r\n");
	}
	return 0;
}

int cmd_init(void)
{
	_cmd_head_init(&CMD_HEAD);
	BOOT_CMD(help, 2, do_help, "This is help command.");
	return 0;
}

static cmd_tbl_t* __find_cmd(const char *name, const cmd_tbl_t* head)
{
	cmd_tbl_t* pos;

	if(name == NULL || head == NULL)
		return NULL;

	list_for_each_entry(pos, &(head->list), list)
	{
		if(0 == strcmp(pos->name, name))
			return pos;
	}

	return NULL;
}

static cmd_tbl_t *find_cmd (const char *name)
{
	return __find_cmd(name, &CMD_HEAD);
}

static int cmd_call(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int result;

	result = (cmdtp->cmd)(cmdtp, flag, argc, argv);

	return result;
}

enum command_ret_t cmd_process(int flag, int argc, char * const argv[],
		unsigned long *ticks)
{
	enum command_ret_t rc = CMD_RET_SUCCESS;
	cmd_tbl_t *cmdtp;

	/* Look up command in command table */
	cmdtp = find_cmd(argv[0]);
	if (cmdtp == NULL) {
		cli_printf("Unknown command '%s' - try 'help'\n", argv[0]);
		return 1;
	}

	/* found - check max args */
	if (argc > cmdtp->maxargs)
		rc = CMD_RET_USAGE;

	if(rc == CMD_RET_SUCCESS)
	{
		rc = cmd_call(cmdtp, flag, argc, argv);
	}else
	{
		rc = cmd_usage(cmdtp);
	}

	return rc;
}

int parse_line (char *line, char *argv[])
{
	int nargs = 0;

	while (nargs < CONFIG_SYS_MAXARGS) {

		/* skip any white space */
		while (isblank(*line))
			++line;

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return nargs;
		}

		argv[nargs++] = line;	/* begin of argument string	*/

		/* find end of string */
		while (*line && !isblank(*line))
			++line;

		if (*line == '\0') {	/* end of line, no more args	*/
			argv[nargs] = NULL;
			return nargs;
		}

		*line++ = '\0';		/* terminate current arg	 */
	}

	return (nargs);
}

static int builtin_run_command(const char *cmd, int flag)
{
	char cmdbuf[CONFIG_SYS_CBSIZE];	/* working copy of cmd		*/
	char *token;			/* start of token in cmdbuf	*/
	char *sep;			/* end of token (separator) in cmdbuf */
	char *str = cmdbuf;
	char *argv[CONFIG_SYS_MAXARGS + 1];	/* NULL terminated	*/
	int argc, inquotes;
	int rc = 0;

	if (!cmd || !*cmd) {
		return -1;	/* empty command */
	}

	if (strlen(cmd) >= CONFIG_SYS_CBSIZE) {
		return -1;
	}

	strcpy (cmdbuf, cmd);

	/* Process separators and check for invalid
	 * repeatable commands
	 */

	while (*str) {

		/*
		 * Find separator, or string end
		 * Allow simple escape of ';' by writing "\;"
		 */
		for (inquotes = 0, sep = str; *sep; sep++) {
			if ((*sep=='\'') &&
					(*(sep-1) != '\\'))
				inquotes=!inquotes;

			if (!inquotes &&
					(*sep == ';') &&	/* separator		*/
					( sep != str) &&	/* past string start	*/
					(*(sep-1) != '\\'))	/* and NOT escaped	*/
				break;
		}

		/*
		 * Limit the token to data between separators
		 */
		token = str;
		if (*sep) {
			str = sep + 1;	/* start of command for next pass */
			*sep = '\0';
		}
		else
			str = sep;	/* no more commands for next pass */

		/* Extract arguments */
		if ((argc = parse_line (token, argv)) == 0) {
			rc = -1;	/* no command at all */
			continue;
		}

		if (cmd_process(flag, argc, argv, NULL))
			rc = -1;

	}

	return rc;
}

int run_cmd(const char *cmd)
{
	if(cmd == NULL)
		return -1;

	return builtin_run_command(cmd, 0);
}	
