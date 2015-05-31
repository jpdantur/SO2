#include <naiveConsole.h>
#include <video.h>
#define SYSCALL_WRITE 4

char get_call(void);
char get_rax(void);
char get_rcx(void);


void int80()
{
	char call = get_rax();
	char c;

	switch (call)
	{
		case SYSCALL_WRITE:
			c = get_rcx();
			video_write_byte(c);
			break;

		default:
	 		break;
	}
}

void timerTick()
{
	//video_write_byte('f');
}

