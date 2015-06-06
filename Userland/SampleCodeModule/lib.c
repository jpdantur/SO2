#include <lib.h>

void print(char * str)
{
	char * aux = str;
	int len = 0;
	
    while(*aux)
	{
		aux++;
		len++;
	}

	sys_write(str, len);
	putchar(0);
}

void putchar(char c)
{
	sys_write(&c, 1);
}

char getchar()
{
	char c;
	sys_read(&c, 1);
	return c;
}

int scan(char * str, int len)
{
	char scan_bff[256];
	sys_read(scan_bff, len);
	
    int i = 0;
	int j = 0;

	while(scan_bff[i] != '\n')
	{
		if (scan_bff[i] == '\b')
		{
			if(j > 0)
				j--;
			str[j] = 0;
		}
        else
		{
			str[j++] = scan_bff[i];
		}
		
        i++;
	}

    if (j != 0)
    {
		str[j] = '\n';
		str[j + 1] = 0;
	}
	else
	{
		str[j] = 0;
	}
	return j;
}

int strcmp(const char * str1, const char * str2)
{
    int sub;

    while (*str1 && *str2)
    {
        sub = *str1 - *str2;

        if (sub != 0)
            return sub;
        
        str1++;
        str2++;
    }
    
    if (*str1)
        return 1;

    if (*str2)
        return -1;

    return 0;
}

char * itoa(int i, char b[], int len){
    char const digit[] = "0123456789ABCDEF";
    char* p = b;
    int a = 0;
    
    if(i < 0){
        *p++ = '-';
        i *= -1;
    }
    
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        ++a;
        shifter = shifter/16;
    }while(a < len);
    
    *p = '\0';
    
    do{ //Move back, inserting digits as u go
        *--p = digit[i%16];
        i = i/16;
    }while(p != b);
    return b;
}


int pow(int x, int y)
{
    int ret = 1;

    for (int i = 0; i < y; i++)
        ret *= x;

    return ret;
}


int dtoh(int h)
{
    int ret = 0;
    int count = 0;
    do
    {
        ret += (h % 10) * pow(16, count);
        h = h / 10;
        count++;

    } while(h != 0);

    return ret;
}