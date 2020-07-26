#ifndef _SQL_H
#define _SQL_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

#define  PRINT_ERRNO        printf("%s->%s->%d:illegal variable errno\n", __FILE__, __func__, __LINE__)

/*用于保存进数据库的结构体*/
typedef struct info
{
	char    ip[20];
	char    name[15];
	char    temp[15];
	char    client_time[20];
	char    server_time[20];
}INFO, *PINFO;

static int callback_table(void *pv, int argc, char **argv, char **col);
static int callback_insert(void *pv, int argc, char **argv, char **col);
static int callback_delete(void *pv, int argc, char **argv, char **col);
int creat_table(sqlite3 *db);
int insert_data(sqlite3 *db,INFO *data);
#endif
