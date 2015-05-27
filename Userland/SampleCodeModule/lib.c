void print(char * str)
{
	int i=0;
	while(*str)
	{
		//*v=i+'0';
		i++;
		putchar(*str);
		str++;
	}
}

void putchar(char c)
{
	sys_write(c);
}
