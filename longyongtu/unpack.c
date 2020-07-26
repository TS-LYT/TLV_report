#include "unpack.h"

void dump_buf(char *buf, int bytes)
{       
    int   i = 0;

    for(i=0; i<bytes; i++)
    {       
        printf("  0x%02x", (unsigned char)buf[i]);
        if(0 == (i+1)%16)
        {
            printf("\n");
        }
    }
    printf("\n");
}





int read_data(int readfd, PNODE head)
{
    int             rv = -1;
    unsigned short  crc_c = 0;
    unsigned short  crc_s = 0;
    int             i = 0;
    PNODE temp = link_specified(head, readfd);
    if( 0 == temp->buf_offset )
    {
        printf("starting receive data from client ip[%s] port[%d]\n", temp->ip, temp->port);
    }

    rv = read(readfd, (temp->buf+temp->buf_offset), sizeof(temp->buf));
    if( rv < 0)
    {       
        printf("read data from client ip[%s] port[%d] failure:%s\n", temp->ip, temp->port, strerror(errno));
        return -1;
    }
    else if( rv == 0)
    {       
        printf("client ip[%s] port[%d] disconnet with server:%s\n", temp->ip, temp->port, strerror(errno));
        return -1;
    }
    printf("recive %d BYTES data from cilent successful:\n", rv);

    temp->buf_offset += rv;
    dump_buf(temp->buf, temp->buf_offset);

    if( temp->buf_offset < MIN_LEN )
    {
        /*debug : print all data , because data don't have minimum length, so wo have to continue receive data from client*/
        printf("Now the data frome client is: \n");
        dump_buf(temp->buf, temp->buf_offset);
        printf("data don't have minimum length, Continue to wait for data transfer:\n");
        return 0;
    }
    else if( temp->buf_offset <= MAX_LEN+10 )
    {
        for(i=0; i<=(temp->buf_offset); i++)	
        {
            //printf("%c\n",(unsigned char)temp->buf[i]);
            //printf("enter cycel\n");
            if(HEADER == (unsigned char)temp->buf[i])
            {
                printf("find HEADER:%d\n",i);
                if( (temp->buf_offset - i) <= MIN_LEN )
                {
                    printf("Don't have enough data after HEADER,continue to wait for data transfer\n");
                    return 0;
                }
                else if( (temp->buf_offset - i) >= ((unsigned int )temp->buf[i+2]))
                {
                    crc_s=crc_itu_t(MAGIC_CRC, temp->buf+i, ((unsigned int)temp->buf[i+2])-2);
                    //printf("crc data:%s and number :%d",temp->buf+i,((unsigned int)temp->buf[i+2])-2);
                    dump_buf(temp->buf+i, ((unsigned int)temp->buf[i+2])-2);
                    printf("  %d\n", crc_s);

                    crc_c=bytes_to_ushort((unsigned char *)temp->buf+(i-2+((unsigned int )temp->buf[i+2])), 2);
                    printf("  %d\n", crc_c);
                    if ( crc_c == crc_s )
                    {
                        printf("CRC correct\n");
                        if( temp->buf[i+1] == TAG_SN)	
                        {
                            printf("find Byte data of SN:\n");
                            dump_buf(temp->buf+i, ((unsigned int)temp->buf[i+2]));
                            memmove(temp->buf, temp->buf+i, temp->buf_offset-i);
                            temp->buf_offset -= i;
                            //dump_buf(temp->buf, temp->buf_offset);
                            continue;
                        }
                        if( temp->buf[i+1] == TAG_TEMP)
                        {       
                            printf("find Byte data of temp:\n");
                            dump_buf(temp->buf+i, ((unsigned int)temp->buf[i+2]));
                            memmove(temp->buf+12, temp->buf+i, temp->buf_offset-i);
                            temp->buf_offset -= (i-12);
                            //dump_buf(temp->buf, temp->buf_offset);
                            continue;
                        }
                        if( temp->buf[i+1] == TAG_TIME)
                        {       
                            printf("find Byte data of time:\n");
                            dump_buf(temp->buf+i, ((unsigned int)temp->buf[i+2]));
                            memmove(temp->buf+7+12, temp->buf+i, temp->buf_offset-i);
                            temp->buf_offset = MAX_LEN;
                            printf("data from client is complete:\n");
                            //dump_buf(temp->buf, MAX_LEN);
                            return temp->buf_offset;
                        }

                    }
                    else 
                    {
                        printf("check CRC errno \n");
                        printf("send NAK to client \n");
                        memset(temp->buf, 0, sizeof(temp->buf));
                        temp->buf_offset = 0;
                        return 1;

                    }
                }
            }
        }
        printf("data from client not incomplete, so continue wait data from client\n");
        return 0;
    }
    else
    {
        printf("length of the byte data from client errno\n ");
        printf("send NAK to client \n");
        memset(temp->buf, 0, sizeof(temp->buf));
        temp->buf_offset =0;
        return 1;
    }

}


int insert2db(PNODE head, int readfd, sqlite3 *db)
{
    int         rv = 0;
    INFO        data1;
    PINFO       data = NULL;
    PNODE       temp;
    char        buf1[20];

    data = &data1;
    temp = link_specified(head, readfd);

    memset(data, 0, sizeof(data1));
    memset(buf1, 0, sizeof(buf1));
    get_time(buf1, sizeof(buf1));

    memcpy(data->server_time , buf1, sizeof(buf1));

    memcpy(data->ip, temp->ip, sizeof(temp->ip));

    memcpy(data->name, temp->buf+3, 7);

    hex_char_temp((unsigned char*)temp->buf+12+3, 2, (unsigned char *)data->temp, sizeof(data->temp));

    hex_char_time((unsigned char*)temp->buf+12+7+3, 6, (unsigned char *)data->client_time, sizeof(data->client_time));

    rv=insert_data(db, data);
    if(rv < 0)
    {
        printf("inset data to database failure\n");
        memset(temp->buf, 0, sizeof(temp->buf));
        temp->buf_offset = 0;
        return -1;
    }
    
    memset(temp->buf, 0, sizeof(temp->buf));
    temp->buf_offset = 0;

    return 1;
}


