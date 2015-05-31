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