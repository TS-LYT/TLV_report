#include "server.h"


int     run = 0;

/* 打印帮助信息 */
void usage(const char *progname)
{
    printf("%s usage:\n", progname);
    printf("this programe can get temperature from client\n");
    printf("-d[daemon] runnig in background\n");
    printf("-h[help] print usage\n");
    printf("-p[port] specify port to listen client");
}
#if 1
int pack_tlv(unsigned char *buf, int buf_offset, unsigned char *content, int size, unsigned char tag)
{
    if( !buf || !tag || !content || !size )
    {
        printf("Invalid input argument \n");
        return -1;
    }

    unsigned short   crc = 0;


    buf[buf_offset] = HEADER;
    buf[buf_offset+1] = tag;
    buf[buf_offset+2] = (unsigned char)(size+MIN_LEN-1);
    memcpy(buf+buf_offset+3, content, size);
    crc=crc_itu_t(MAGIC_CRC, buf+buf_offset, size+3);
    ushort_to_bytes(buf+buf_offset+(size+MIN_LEN-1)-2, crc);
    buf_offset += size+MIN_LEN-1;
    
    return buf_offset;
}
#endif


void signal_action()
{
    run = 1;
}

#if 0
void dump_buf(char *buf, int bytes)
{
  int   i = 0;

  for(i=0; i<bytes; i++)
  {
  printf("0x%02x", (unsigned char)buf[i]);
  if(0 == (i+1)%16)
  {
  printf("\n");
  }
  }
  printf("\n");
}
#endif




int main(int argc , char **argv)
{
    char               nak = NAK;
    char               ack = ACK;
    char               *progname = NULL;
    char               ack_buf[6];
    char               nak_buf[6];
    int                opt;
    int                port = 0;
    int                dae = 0;
    int                rv = 0;
    int                newclient = 0;
    int                i = 0;
    int                j = 0;
    int                listenfd = 0;
    int                epollfd = -1;
    int                epoll_list = -1;
    sqlite3            *db = NULL;
    char               *ermssg = NULL;
    char               buf[1024];
    char               server_time[20];
    PNODE     	       head = NULL;
    struct epoll_event event;
    struct epoll_event event_array[EVENT_NUMBER];
    struct sockaddr_in clientaddr;
    socklen_t          sock_len = sizeof(clientaddr);


    struct option long_options[] ={
        {"port", required_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {"daemon", no_argument, NULL, 'd'}
    };

    progname = basename(argv[0]);

    while ((opt=getopt_long(argc, argv, "p:hd", long_options, NULL)) != -1)
    {
        switch (opt)
        {
            case 'p':
                port = atoi(optarg);
                break;

            case 'h':
                usage(progname);
                return 0;
            case 'd':
                dae = 1;
                break ;
        }
    }

    signal(SIGINT, signal_action);


    if( port <= 0)
    {
        printf("please input correct port\n");
        usage(progname);
        return 0;
    }

    /* 检查程序是否在运行 */
    rv=check_run(progname);
    if(rv != 0)
    {
        printf("progname is already running\n");
        return -1;
    }

    /* 选择是否在后台运行 */
    if(dae == 1)
    {
        daemon(1, 0);
        printf("programe runing in background\n");
    }
    else if(dae == 0)
    {
        printf("programe enter debug mode \n");

    }

    /* 检查数据库是否存在，如果不存在需要创建表 */
    rv = access(DB_NAME, F_OK);
    if( rv < 0)
    {
        printf("database not exist\n");
        rv = sqlite3_open(DB_NAME, &db);
        if(rv)
        {
            printf("can't open database: %s\n", sqlite3_errmsg(db));
            return -1;
        }
        rv=creat_table(db);
        if(rv < 0)
        {
            printf("create table failure\n");
            unlink(DB_NAME);
            return -1;
        }
        printf("create table successful\n");
    }
    else
    {
        printf("database already exist\n");
        rv=sqlite3_open(DB_NAME, &db);
        if(rv)
        {
            printf("can't open database: %s\n", sqlite3_errmsg(db));
            return -1;
        }

    }
    printf("open database successful\n");

    /* 进行socket一些相关准备，返回一个listenfd用于监听窗口 */
    listenfd = socket_init(port);
    if ( !listenfd )
    {
        printf("socket init failure\n");
        return -1;
    }
    printf("socket init successful\n");

    /* 创建epoll兴趣列表 */
    epollfd=epoll_create(EVENT_NUMBER);
    if( epollfd < 0)
    {
        printf("epoll_create failure: %s\n", strerror(errno));
        close(listenfd);
        return -1;
    }
    printf("epoll_creat successful\n");

    event.events = EPOLLIN;
    event.data.fd = listenfd;

    /* 将监听的listenfd加入兴趣列表 */
    rv=epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);
    if(rv < 0)
    {
        printf("epoll_ctl  listenfd failure: %s\n", strerror(errno));
        close(listenfd);
        return -1;
    }
    printf("epoll_ctl listenfd successful\n");

    /* 准备好存储每个客户端的信息的链表的头 */
    head=link_init();
    
    pack_tlv(ack_buf, 0, &ack, 1, TAG_ACK);
    pack_tlv(nak_buf, 0, &nak, 1, TAG_NAk);

    while ( !run )
    {
        /* epoll_wait等待兴趣列表的socket触发 */
        epoll_list=epoll_wait(epollfd, event_array, EVENT_NUMBER, -1);
        if (epoll_list == 0)
        {
            printf("epoll_wait timeout: %s\n", strerror(errno));
            continue;
        }
        else if(epoll_list < 0)
        {
            printf("epoll_wait failure: %s\n", strerror(errno));
            close(listenfd);
            return -2;
        }

        for(i=0; i<epoll_list; i++)
        {
            /* 兴趣列表中某个socket出现错误或挂断进行清理工作 */
            if(event_array[i].events&EPOLLERR||event_array[i].events&EPOLLHUP)
            {
                printf("epoll_wait get error on socketfd[%d]: %s\n", event_array[i].data.fd, strerror(errno)); 
                link_free_specified(head, event_array[i].data.fd);
                epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                close(event_array[i].data.fd);
                continue;
            }

            /* listenfd触发了，将使用accpet接受客户端的连接 */
            if(event_array[i].data.fd == listenfd)
            {
                newclient=accept(listenfd, (struct sockaddr*)&clientaddr, &sock_len); 
                if ( newclient < 0)
                {
                    printf("connect with client ip:[%s] port:[%d] failure:%s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), strerror(errno));
                    continue;
                }
                //prinf("accept newclient successful\n");
                event.events = EPOLLIN;
                event.data.fd = newclient;

                /* 还需要将其加入epoll的兴趣列表和链表中 */
                if((epoll_ctl(epollfd, EPOLL_CTL_ADD, newclient, &event)) < 0)
                {
                    printf("epoll_ctl cilent failure: %s\n", strerror(errno));
                    close(newclient);
                    continue;
                }
                link_insert(head, newclient, (int)ntohs(clientaddr.sin_port), inet_ntoa(clientaddr.sin_addr));		
                printf("new client add epoll successful\n");
            }
            else
            {
                /* 接受客户端的信息，成功发送ack，失败发送nak， */
                rv=read_data(event_array[i].data.fd, head);
                if( rv < 0)
                {
                    /* read失败进行相关的清理工作 */
                    printf("read from client failure or disconnect\n");
                    link_free_specified(head, event_array[i].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                    close(event_array[i].data.fd);
                    continue;
                }
                else if(0 == rv)
                {
                    /* 得到的报文不完整，继续等待数据的到来 */
                    printf("data from client incomplete, so conntinue wait data from client\n");
                    continue;
                }
                else if(1 == rv)
                {
                    /* 得到的报文错误，发送nak给客户端，希望客户端重新发送数据 */
                    printf("data from client errno\n");
                    rv=write(event_array[i].data.fd, nak_buf, sizeof(nak_buf));
                    if( rv < 0)
                    {
                        printf("write data to client failure\n");
                        link_free_specified(head, event_array[i].data.fd);
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                        close(event_array[i].data.fd);
                        continue;
                    }
                    printf("send NAk to client successful\n");
                    continue;
                }
                else if(MAX_LEN == rv)
                {
                    /* 接收到的报文正确，发送ack给客户端,进行报文的解析等相关工作 */
                    printf("read data from client and CRC correct \n");
                    rv=insert2db(head, event_array[i].data.fd, db);
                    if(rv < 0)
                    {
                        /* 插入数据失败，发送nak给客户端，重新接收数据 */
                        printf("inset data to database failure\n");
                        printf("so we need send nak to client\n");
                        rv=write(event_array[i].data.fd, nak_buf, sizeof(nak_buf));
                        if( rv < 0)
                        {
                            printf("write data to client failure\n");
                            link_free_specified(head, event_array[i].data.fd);
                            epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                            close(event_array[i].data.fd);
                            continue;
                        }
                        printf("send NAk to client successful\n");
                        continue;
                    }

                    printf("send ACK to client\n");
                    rv=write(event_array[i].data.fd, ack_buf, sizeof(ack_buf));
                    dump_buf(ack_buf, sizeof(ack_buf));
                    if( rv < 0)
                    {
                        //printf("writ data to  client ip[%s] port[%d] failure:%s\n", temp->port, temp->ip, strerror(errno));
                        link_free_specified(head, event_array[i].data.fd);
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                        close(event_array[i].data.fd);
                        continue;
                    }
                    printf("write ACK to successful\n");
                    continue;
                }
            }
        }
    }
    sqlite3_close(db);
}
