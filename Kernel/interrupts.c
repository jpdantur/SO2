#include <naiveConsole.h>

#define CALL_WRITE 4

char get_call(void);
char get_rax(void);
char get_rcx(void);

void int80()
{
	/*char c = get_call();
	ncPrintChar(c);*/

	char call = get_rax();

	switch (call)
	{
		case CALL_WRITE:
			screen_write();
			break;
		default:
	 		break;
	}
}

void screen_write()
{
	char c = get_rcx();
	ncPrintChar(c);
}
