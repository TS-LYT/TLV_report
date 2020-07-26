#ifndef _SERVER_H
#define _SERVER_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <poll.h>
#include <sys/epoll.h>
#include <getopt.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <libgen.h>
//#include "crc.h"
#include "socket_init.h"
#include "unpack.h"
#include "times.h"
#include "sql.h"
#include "link_creat.h"
#include "link_free.h"
#include "int_convert_string.h"
#include "check_run.h"
#include "crc-itu-t.h"
#include "hex_char.h"


#define DB_NAME        "temp.db"
#define EVENT_NUMBER   100
#define HEADER         0xfd
#define TAG_SN         0X01
#define TAG_TEMP       0X02
#define TAG_TIME       0X03
#define TAG_ACK        0X04
#define TAG_NAk        0X05
#define ACK            0XFE
#define NAK            0X00
#define MIN_LEN        6
#define MAX_LEN        30 

#if 0
typedef union char_short
{
    unsigned char  a;
    unsigned short b;
}C_S, *PC_S;
#endif




#endif


