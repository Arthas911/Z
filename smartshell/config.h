/*
 * config.h
 * transplant by ztm
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (Version 2) as
 * published by the Free Software Foundation.
 *
 * date: 2020-04-02
 *
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <stdio.h>
#include "list.h"
#include "console.h"


#define CONFIG_SYS_PROMPT		"user@shell$ "    
#define CONFIG_SYS_CBSIZE		512	        
#define CONFIG_SYS_MAXARGS		16

#define CONFIG_AUTO_COMPLETE

#define getcmd_putch(ch)        putch(ch)
#define getcmd_getch()          getch()
#define getcmd_putstr(s)        putstr(s)
#define getcmd_cbeep()          getcmd_putch('\a')
#define cli_printf              printf

/*
 *  * Rather than doubling the size of the _ctype lookup table to hold a 'blank'
 *   * flag, just check for space or tab.
 *    */
#define isblank(c)      (c == ' ' || c == '\t')

/*
 * Monitor Command Table
 */

struct cmd_tbl_s {
        char            *name;          /* Command Name                 */
        int             maxargs;        /* maximum number of arguments  */
        int             repeatable;     /* autorepeat allowed?          */
                                        /* Implementation function      */
        int             (*cmd)(struct cmd_tbl_s *, int, int, char * const []);
        char            *usage;         /* Usage message        (short) */
        char            *help;          /* Help  message        (long)  */
        /* do auto completion on the arguments */
        int             (*complete)(int argc, char * const argv[], char last_char, int maxv, char *cmdv[]);
        struct list_head list;
};

typedef struct cmd_tbl_s        cmd_tbl_t;

char* readline (const char *const prompt);

int run_cmd(const char *cmd);

int register_cmd(cmd_tbl_t* cmd);

int cmd_init(void);

int cmd_auto_complete(const char *const prompt, char *buf, int *np, int *colp);

#ifdef CONFIG_AUTO_COMPLETE
# define _CMD_COMPLETE(x) x,
#else
# define _CMD_COMPLETE(x)
#endif

#ifdef CONFIG_SYS_UASGE
# define _CMD_USAGE(x) x,
#else
# define _CMD_USAGE(x)
#endif

#define REGISTER_CMD(cmd)       register_cmd(cmd)

#define REG_NAME_TYP(_NAME) CMD_LIST_##_NAME

#define REG_NAME_STR(_NAME) #_NAME

#define REG_CMD_COMPLETE(_name, _maxargs, _cmd, _help, _usage, _comp)   \
static cmd_tbl_t REG_NAME_TYP(_name) =                                  \
{                                                                       \
    .name       = REG_NAME_STR(_name),                                  \
    .maxargs    = _maxargs,                                             \
    .cmd        = _cmd,                                                 \
    .help       = _help,                                                \
    .usage      = _usage,                                               \
    .complete   = _comp,                                                \
}

#define BOOT_CMD(_name, _maxargs, _cmd, _help)                          \
do{                                                                     \
        REG_CMD_COMPLETE(_name, _maxargs, _cmd, _help, NULL, NULL);     \
        REGISTER_CMD(&(REG_NAME_TYP(_name)));                           \
}while(0)


#endif
