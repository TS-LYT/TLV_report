#include "times.h"


void print_time()
{
	time_t                timep;
	struct tm             *p = NULL;

	timep=time(&timep);//time_t就是long，函数返回从1970年1月1日（MFC是1899年12月31日）0时0分0秒，到现在的的秒数。
	if (timep < 0)
	{
		printf("%s->%s->%d:time function errno\n", __FILE__, __func__, __LINE__); 
		return;
	}

	//p=gmtime(&timep);//将日历时间转化为世界标准时间（即格林尼治时间）

	p=localtime(&timep);
	if(p == NULL)
	{
		printf("%s->%s->%d:localtime function errno\n", __FILE__, __func__, __LINE__);
		return ;
	}
	/*********************************
	 ****	printf("%d\n",p->tm_sec);      获取当前秒   
	 ****	printf("%d\n",p->tm_min);      获取当前分
	 ****	printf("%d\n",p->tm_hour);   获取当前时
	 ****	printf("%d\n",p->tm_mday);     获取当前月份日数,范围是1-31
	 ****	printf("%d\n",1+p->tm_mon);    获取当前月份,范围是0-11,所以要加1
	 ****	printf("%d\n",1900+p->tm_year);获取当前年份,从1900开始，所以要加1900
	 ****	printf("%d\n",p->tm_yday);     从今年1月1日算起至今的天数，范围为0-365
	 *********************************/
	printf("%d.",1900+p->tm_year);
	printf("%d.",1+p->tm_mon);
	printf("%d__",p->tm_mday);
	printf("%d:",p->tm_hour);
	printf("%d:",p->tm_min);
	printf("%d\n",p->tm_sec);
}



void get_time(char *gettime, int size)
{
	if( !gettime )
	{
		printf("%s->%s->%d:time function errno\n", __FILE__, __func__, __LINE__);
		return ;
	}

	time_t                timep = -1;
	struct tm             *p = NULL;
	char                  buf[20];
	memset(buf, 0, sizeof(buf));
	

	timep=time(&timep);//time_t就是long，函数返回从1970年1月1日（MFC是1899年12月31日）0时0分0秒，到现在的的秒数。
	if (timep < 0)
	{
		printf("%s->%s->%d:time function errno\n", __FILE__, __func__, __LINE__);
		return ;
	}

	//p=gmtime(&timep);//将日历时间转化为世界标准时间（即格林尼治时间）

	p=localtime(&timep);
	if(p == NULL)
	{
		printf("%s->%s->%d:localtime function errno\n", __FILE__, __func__, __LINE__);
		return ;
	}
	
	int_to_string(1900+p->tm_year, gettime);
	strncat(gettime, "-", 1);
	int_to_string(1+p->tm_mon, buf);
	strncat(gettime, buf, strlen(buf));
	strncat(gettime, "-", 1);
	int_to_string(p->tm_mday, buf);
	strncat(gettime, buf, sizeof(buf));
	strncat(gettime, ":", 2);
	int_to_string(p->tm_hour, buf);
	strncat(gettime, buf, sizeof(buf));
	strncat(gettime, ":", 1);
	int_to_string(p->tm_min, buf);
	strncat(gettime, buf, sizeof(buf));
	strncat(gettime, ":", sizeof(buf));
	int_to_string(p->tm_sec, buf);
	strncat(gettime, buf, sizeof(buf));
}





/*int main ()
{
	char buf[20];
	memset(buf , 0, sizeof(buf));
	get_time(buf, sizeof(buf));
	printf("%s\n",buf);
	return 0;
}*/
