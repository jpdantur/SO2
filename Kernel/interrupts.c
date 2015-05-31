#include <naiveConsole.h>
#include <video.h>
#include <keyboard.h>

#define SYSCALL_READ 3
#define SYSCALL_WRITE 4

char get_call(void);
char get_rax(void);
char get_rcx(void);
void set_rax(char c);
char i='A';


void int80(char *p)
{
	char call = get_rax();
	char c;
	//char * p;

	switch (call)
	{
		case SYSCALL_READ:
			//p = get_rcx();
			//*((char*)0xB8006)='?';
			*p = keyboard_buffer_read();
			break;
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
	//video_write_byte(i);
	//i++;
}

