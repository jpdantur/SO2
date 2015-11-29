#include <stdint.h>
#include <time.h>
#include <lib.h>
#include <shell.h>

extern char bss;
extern char endOfBinary;

static int var1 = 0;
static int var2 = 0;

void * memset(void * destiny, int32_t c, uint64_t length);

int main() {

	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);
	int pid = newproc(&shell, "shell",1);
	//printint(pid);
	//while(1)
	//	print("Hola");
	//shell();

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

