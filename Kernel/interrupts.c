#include <naiveConsole.h>
#include <video.h>
#include <keyboard.h>

#define SYSCALL_READ 3
#define SYSCALL_WRITE 4
#define SYSCALL_TIME_READ 2
#define SYSCALL_TIME_WRITE 5

char get_call(void);
char get_rax(void);
char get_rcx(void);
void set_rax(char c);

void set_time(char time, char offset);

char get_hours();
char i='A';


void int80(char *p, int type, int value)
{
	char call = get_rax();
	char c;

	switch (call)
	{
		case SYSCALL_READ:
			*p = keyboard_buffer_read();
			break;

		case SYSCALL_WRITE:
			c = get_rcx();
			video_write_byte(c);
			break;

		case SYSCALL_TIME_READ:
			*p = get_time(type);
			break;

		case SYSCALL_TIME_WRITE:
			set_time(value, type);
			break;

		default:
	 		break;
	}
}

void timerTick()
{
	// __do_nothing
}

