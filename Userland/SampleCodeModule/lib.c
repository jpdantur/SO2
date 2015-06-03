void print(char * str)
{
	while(*str)
	{
		//*v=i+'0';
		putchar(*str);
		str++;
	}
	putchar(-1);
}

void putchar(char c)
{
	sys_write(c);
}

char getchar()
{
	char c;
	do
	{
		sys_read(&c);
	} while (c == -1);
	putchar(c);
	return c;
}

char time(int type)
{
	char c;
	sys_time(&c,type);
	return c;
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789ABCDEF";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/16;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%16];
        i = i/16;
    }while(i);
    return b;
}