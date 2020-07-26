#ifndef _LINK_CREAT_H
#define _LINK_CREAT_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* 这个结构体是链表的存储单元，结构体存储了每个socket的一些相关信息 */
typedef struct node
{
	int            sockfd;
	int            port;
	int            buf_offset;
	struct         node*next;
	char           ip[20];
	unsigned char  buf[50];
}*PNODE,NODE;

PNODE link_init();
int link_insert(PNODE head, int sockfd, int port, char *ip);
int link_print_all(PNODE head);
int link_print_specified(PNODE head, int sockfd);
PNODE link_specified(PNODE head, int sockfd);

#endif
