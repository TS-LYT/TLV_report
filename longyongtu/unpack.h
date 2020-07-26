#ifndef _UNPACK_H
#define _UNPACK_H

/*typedef struct tlv_s
{
	unsigned char      header;
	unsigned char      tag;
	unsigned char      len;
}tlv_head;*/


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "link_free.h"
#include "link_creat.h"
#include "crc-itu-t.h"
#include "sql.h"
#include "hex_char.h"
#include "int_convert_string.h"
#include "times.h"


#define HEADER         0xfd
#define TAG_SN         0X01
#define TAG_TEMP       0X02
#define TAG_TIME       0X03
#define MIN_LEN        6
#define MAX_LEN        30 


void dump_buf(char *buf, int bytes);
int read_data(int readfd, PNODE head);
int insert2db(PNODE head, int readfd, sqlite3 *db);



#endif
