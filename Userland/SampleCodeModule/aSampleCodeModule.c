#include <stdint.h>

char * v = (char*)0xB8000 + 79 * 2;

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

void print(char * str);
void sys_read(char * p);
void set_time(char type, char value);

void * memset(void * destiny, int32_t c, uint64_t length);

int main() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);
	char aux[2];
	aux[0]=aux[1]='0';

	set_time(0, 0x40);
	set_time(1, 0x59);
	set_time(2, 0x23);

	while (1)
	{
		print("TODO: add prompt$ ");
		char c;
		do
		{
			c=getchar();

		} while(c!='\n');
		
		print(itoa(time(2),aux,2));
		putchar(':');
		print(itoa(time(1),aux,2));
		putchar(':');
		print(itoa(time(0),aux,2));
		putchar('\n');
	}
	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}

void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
