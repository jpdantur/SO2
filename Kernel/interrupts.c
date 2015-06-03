#include <naiveConsole.h>
#include <video.h>
#include <keyboard.h>

#define SYSCALL_READ 3
#define SYSCALL_WRITE 4
#define SYSCALL_TIME 2

char get_call(void);
char get_rax(void);
char get_rcx(void);
void set_rax(char c);
char get_seconds();
char get_minutes();
char get_hours();
char i='A';


void int80(char *p, int type)
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
		case SYSCALL_TIME:
			switch(type)
			{
				case 0:
					*p=get_seconds();
					break;
				case 1:
					*p=get_minutes();
					break;
				case 2:
					*p=get_hours();
					break;

			}
		default:
	 		break;
	}
}

void timerTick()
{
	//video_write_byte(i);
	//i++;
}

