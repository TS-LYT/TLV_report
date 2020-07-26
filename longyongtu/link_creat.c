#include "link_creat.h"


/*************************************************
***
***	set head of the singal link;
***
*************************************************/
PNODE link_init()
{
	PNODE temp = (PNODE)malloc(sizeof(NODE));
	memset(temp, 0,sizeof(NODE));
	temp->sockfd = 0;
	temp->port = 0;
	temp->next = NULL;
	temp->buf_offset = 0;
	memset(temp->ip, 0,sizeof(temp->ip));
	return temp;
}



/************************************************
***
***    insert new data to singal link; 
***
************************************************/
int link_insert(PNODE head, int sockfd, int port, char *ip)
{
	/*insert in head*/
	PNODE temp = (PNODE)malloc(sizeof(NODE));
	memset(temp, 0,sizeof(NODE));
	temp->next = head->next;

	temp->sockfd = sockfd;
	temp->port = port;
	strncpy(temp->ip, ip, sizeof(temp->ip));

	head->next = temp;
	return 0;
	
#if 0
	/*insert in tail*/
	PNODE temp = (PNODE)malloc(sizeof(NODE));
	memset(temp, 0,sizeof(NODE));
	while(head->next != NULL)
	{
		head = head->next;
	}

	temp->sockfd = sockfd;
	temp->port = port;
	strncpy(temp->ip, ip, sizeof(temp->ip));

	head->next = temp;
	temp->next = NULL;
	return 0;
#endif

}



/************************************************
***
***    	print all data in link
***
************************************************/
int link_print_all(PNODE head)
{
	head = head->next;
	while(head->next != NULL)
	{
		printf("socket:%d  ip:%s  port:%d\n",head->sockfd, head->ip, head->port);
		head = head->next;
	}
	return 0;
}


/************************************************
***
***     print specified data of socket;
***
************************************************/



int link_print_specified(PNODE head, int sockfd)
{
	while(head->sockfd != sockfd)
	{
		head = head->next;
	}
	printf("socket:%d  ip:%s  port:%d\n",head->sockfd, head->ip, head->port);
	return 0;
}




PNODE link_specified(PNODE head, int sockfd)
{
	PNODE temp = head;
	while(temp->sockfd != sockfd)
	{
		temp = temp->next;
	}
	return  temp; 
}
