#include <naiveConsole.h>

char get_call(void);

void int80()
{
	char c = get_call();
	ncPrintChar(c);	
}

