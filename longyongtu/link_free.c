#include "link_free.h"



/*************************************************************
 ***
 ***	free all data of link
 ***
 *************************************************************/
int link_free_all(PNODE head)
{
	if( !head )
	{
		printf("%s->%s->%d:invalid argument\n", __FILE__, __func__, __LINE__);
		return 0;
	}

	PNODE temp;
	while(head->next != NULL)
	{
		temp = head->next;
		free(head);
		head = temp;
	}
	free(head);
	return 0;
}




/*************************************************************
 ***
 ***     free specified(socket) data of link
 ***
 *************************************************************/
int link_free_specified(PNODE head, int sockfd)
{
	if( !head )
	{       
		printf("%s->%s->%d:invalid argument\n", __FILE__, __func__, __LINE__);
		return 0;
	}
	if( !sockfd )
	{
		printf("%s->%s->%d:invalid argument\n", __FILE__, __func__, __LINE__);
		return 0;
	}

	while(head->next->sockfd != sockfd)
	{
		head = head->next;
	} 
	PNODE temp;
	temp =head->next;
	free(head->next);
	head->next = temp->next;
	return 0;
}
