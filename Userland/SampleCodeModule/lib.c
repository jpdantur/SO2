#include <lib.h>
int newproc(void *entry)
{
    int pid;
    sys_newproc(entry, &pid);
    return pid;
}
void kill(int pid)
{
    sys_kill(pid);
}
void list()
{
    sys_list();
}
void fore()
{
    sys_fore();
}
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
		/*if (scan_bff[i] == '\b')
		{
			if(j > 0)
				j--;
			str[j] = 0;
		}
        else
		{*/
			str[j++] = scan_bff[i];
		//}
		
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

int htod(int h)
{
    int ret = 0;
    int count = 0;

    while (h != 0)
    {
        ret += (h % 16) * pow(10, count);
        h = h / 16;
        count++;
    }

    return ret;
}

void printint(int num)
{
    char numvec[MAX_NUMBER_PRINT_LEN] = {'0'};
    int i = 0;
    int len = 0;
    char aux;

    if (num == 0){
        putchar('0');
        return;
    }

    while (num != 0)
    {
        numvec[len++] = num % 10 + '0';
        num /= 10;
    }

    numvec[len--] = 0;

    while(i < len)
    {
        aux = numvec[i];
        numvec[i] = numvec[len];
        numvec[len] = aux;

        i++;
        len--;
    }

    print(numvec);

}

void printhex(int num)
{
    print("0x");
    printint(htod(num));
}

void * malloc()
{
    void *ret;
    sys_malloc(&ret);
    return ret;
}

void free(void *p)
{
    sys_free(p);
    return;
}