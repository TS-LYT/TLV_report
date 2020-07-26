#include "int_convert_string.h"


int int_to_string(int x, char *des)
{
	int         max = 0;
	unsigned int         t=x;
	int         i = 0;

	if(x < 0 || x > UINT_MAX)
	{
		printf("%s->%s->%d:Invalid variable\n", __FILE__, __func__, __LINE__);
		return 0;
	}
	if(x == 0)
	{
		max = 1;
	}
	else
	{
		while (t > 0)
		{
			max++;
			t/=10;
		}
	}

	char        buf[max];
	if(x < 10)
	{
		buf[0] = x+0x30;
		*des++ = buf[0];
	}
	else if(x >= 10)
	{
		while (x > 0)
		{
			t = x%10;
			x/= 10;
			buf[i++] = t + 0x30;
		}
		i--;

		for(;i >= 0;i--)
		{
			*des++ = buf[i];

		}

	}
	*des = '\0';
}
