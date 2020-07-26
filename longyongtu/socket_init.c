#include "socket_init.h"

int socket_init(int port)
{
	if( port <= 0)
	{
		printf("invaild argument\n");
		return -1;
	}

	int                    rv = -1;
	int                    listenfd = -1;
	int                    on = 1;
	struct sockaddr_in     serveraddr;
	
	memset(&serveraddr, 0, sizeof(serveraddr));

	listenfd=socket(AF_INET, SOCK_STREAM, 0);
	if( listenfd < 0)
	{
		printf("creat listenfd failure:%s\n", strerror(errno));
		return -2;
	}
	printf("creat listenfd successful\n");

	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	rv=bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if( rv < 0)
	{
		printf("bind port failure:%s\n", strerror(errno));
		close(listenfd);
                return -3;
	}
	printf("bind port successful\n");

	rv=listen(listenfd, 50);
	if(rv < 0)
	{
		printf("listen port failure:%s\n", strerror(errno));
                close(listenfd);
		return -3;
	}
	printf("listen socket:%d successful\n", listenfd);
	 
	return listenfd;
}

































