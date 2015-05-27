#include <naiveConsole.h>

char get_call(void);
//void write();

void write()
{
	char c = get_call();
	ncPrintChar(c);	
}

