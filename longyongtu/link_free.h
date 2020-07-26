#ifndef _LINK_FREE_H
#define _LINK_FREE_H

#include "link_creat.h"
int link_free_all(PNODE head);
int link_free_specified(PNODE head, int sockfd);
/*extern typedef struct node
{
	int   data;
	struct node *next;
}*PNODE;*/

#endif
