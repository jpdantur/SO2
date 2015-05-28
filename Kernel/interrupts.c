#include <naiveConsole.h>
#include "drivers/video.h"
#define SYSCALL_WRITE 4

char get_call(void);
char get_rax(void);
char get_rcx(void);


void int80()
{
	char call = get_rax();

	switch (call)
	{
		case SYSCALL_WRITE:
			char c = get_rcx();
			video_write_byte(char c);
			break;

		default:
	 		break;
	}
}

