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
int sleep=0;
char sleeping=0;


void int80(char *p, int type, int size)
{
	char call = get_rax();
	char c;
	//char * a;
	//a=p;
	int i;
	char enter=0;
	switch (call)
	{
		case SYSCALL_READ:
			//p = get_rcx();
			//*((char*)0xB8006)='?';
			for (i=0;i<size-1 && !enter;i++)
			{	
				//__video_debug('X');
				*p = keyboard_buffer_read();
				if (*p=='\n')
					enter=1;
				//__video_debug(*p);
				p++;
				//i++;
			}
			if (!enter)
			{
				*p='\n';
				video_new_line();
			}
			break;

		case SYSCALL_WRITE:
			for (i=0;i<size;i++)
			{
				video_write_byte(*p);
				p++;
			}
			break;

		case SYSCALL_TIME_READ:
			*p = get_time(type);
			break;

		case SYSCALL_TIME_WRITE:
			set_time(size, type);
			break;

		default:
	 		break;
	}
	//__video_debug('X');
}

void timerTick()
{
	sleep++;
	if (sleep==1000)
	{
		screen_saver();
		sleeping=1;
	}
	if (sleeping)
		video_screen_saver_draw();
}

void keyboard()
{
	if (sleeping)
	{
		sleeping=0;
		restore();
	}
	sleep=0;
	keyboard_buffer_write();	
}
