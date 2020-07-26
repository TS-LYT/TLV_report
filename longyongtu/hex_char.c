#include "hex_char.h"



void hex_char(unsigned char *src, int src_len, unsigned char *des, int des_len)
{
    int     i=0;
    int     rv=0;    

    for(i=0; i<des_len; i++)      
    {
        rv=snprintf(des+rv, des_len, "%d", (unsigned short)*(src+i));
        rv -= 1;
    }
    return;
}


void hex_char_temp(unsigned char *src, int src_len, unsigned char *des, int des_len)
{
    int     rv=0;

    rv=snprintf(des, des_len, "%d.%d", (unsigned short)*src, (unsigned short)*(src+1));
    if(rv < 0)
    {
        printf("snprintf failure\n");
    }
    return;
}


void hex_char_time(unsigned char *src, int src_len, unsigned char *des, int des_len)
{
    int     rv = 0;
    
    rv=snprintf(des, des_len, "%d-%d-%d:%d:%d:%d", (unsigned short)(*src)+1900, (unsigned short)*(src+1), (unsigned short)*(src+2), (unsigned short)*(src+3), (unsigned short)*(src+4), (unsigned short)*(src+5));
    return;
}

#if 0

int main(int argc, char *argv[])
{
    char    src[20];
    char    des[20];
    memset(des, 0, sizeof(des));
    src[0]=0x78;
    src[1]=0x05;
    src[2]=0x12;
    src[3]=0x12;
    src[4]=0x04;
    src[5]=0x29;
    hex_char_time(src, 6, des, 20);
    printf("%s\n", des);
    return 0;
}
#endif









