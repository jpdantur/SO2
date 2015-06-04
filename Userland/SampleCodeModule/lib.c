char * vid = (char*)0xB8000 + 79 * 2;
void print(char * str)
{
	char * aux = str;
	int len = 0;
	while(*aux)
	{
		aux++;
		len++;
	}
	sys_write(str,len);
	putchar(0);
}

void putchar(char c)
{
	sys_write(&c,1);
}

char getchar()
{
	char c;
	sys_read(&c,1);
	return c;
}

int scan(char * str)
{

}

//TODO: Move from here
char time(int type)
{
	char c;
	sys_time(&c,type);
	return c;
}

void set_time(int hour, int min, int sec)
{
    set_time_att(0, dtoh(sec));
    set_time_att(1, dtoh(min));
    set_time_att(2, dtoh(hour));
}

void set_time_att(char type, char att)
{
    sys_time_write(type, att);
}

char* itoa(int i, char b[], int len){
    char const digit[] = "0123456789ABCDEF";
    char* p = b;
    int a=0;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        ++a;
        shifter = shifter/16;
    }while(a<len);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%16];
        i = i/16;
    }while(p!=b);
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