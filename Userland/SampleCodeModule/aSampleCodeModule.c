#include <stdint.h>
#include <time.h>
#include <lib.h>

char * v = (char*)0xB8000 + 79 * 2;

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

void print(char * str);
void sys_read(char * p);
void set_time(int hour, int min, int sec);

void * memset(void * destiny, int32_t c, uint64_t length);

int main() {

	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	char aux[2];
	aux[0] = aux[1] = '0';

	set_time(4,20,0);

	char bff[256];
	int a;

	while (1)
	{
		print("Mister Poronga>");
		a = scan(bff,256);
		print(bff);
		//putchar('X');
	}

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}

void * memset(void * destination, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

