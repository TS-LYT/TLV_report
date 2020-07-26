#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdlib.h>
int  check_run(char *proname)
{
	FILE		*fp = NULL;
	char            command[150];
	char            buf[300];
	int             count = 3;

	/******************************************************

		ps -ef列出相关进程的信息  grep -w %S精确字符串的匹配  wc -l显示数据有几行，用此来判断程序是否存在 
		注意的是grep是一个单独的进程，popen会执行一个sh -c .... 的进程，所以需设置count为3

	******************************************************/
	snprintf(command, sizeof(command), "ps -ef | grep -w %s | wc -l ", proname);

	fp=popen(command, "r");
	if( !fp )
	{
		printf("pepen errno\n");
		return -1;
	}

	if((fgets(buf, sizeof(buf), fp)) != NULL)
	{
		count = atoi(buf);
		printf("%d\n", count);
	}

	/*snprintf(command, sizeof(command), "ps -ef | grep -w %s ", proname);
	fp=popen(command, "r");
	while( count != 7)
	{
		fgets(buf, sizeof(buf), fp);
		printf("%s",buf);
		count++;
	}*/

	pclose(fp);
	fp = NULL;

	if((count -3) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/*int   main(int argc, char **argv)
{
	int    rv;
	char   *progname=NULL;	
	progname=basename(argv[0]);
	printf("%s\n", progname);
	rv = check_run(progname);
	printf("%d\n", rv);
	while(1)
	{};
}*/
