#include <naiveConsole.h>
#include <video.h>
#include <keyboard.h>
#include <CMOS.h>
#include <alloc.h>

#define SYSCALL_READ 3
#define SYSCALL_WRITE 4
#define SYSCALL_TIME_READ 2
#define SYSCALL_TIME_WRITE 5
#define SYSCALL_SCREEN_SAVER_SET 6
#define SYSCALL_MALLOC 7
#define SYSCALL_FREE 8

char get_call(void);
char get_rax(void);
char get_rcx(void);
void set_rax(char c);


void int80(int *p1, int rbx, int rdx)
{
	char call = get_rax();
	char *p = (char*)p1;
	char *aux = p;
	char c;
	int i = 0;
	
	char enter = 0;
	
	switch (call)
	{
		case SYSCALL_READ:
			while (i < rdx - 1 && !enter)
			{	
				*p = keyboard_buffer_read();
				if (*p == '\n')
				{
					enter = 1;
					p++;
				}
				else if (*p == '\b')
				{
					*p = 0;
					if (p > aux)
					{
						p--;
						*p = 0;
						i--;
					}
				}
				else{
					i++;
					p++;
				}

			}

			if (!enter)
			{
				*p = '\n';
				video_new_line();
			}
			break;

		case SYSCALL_WRITE:
			for (i = 0; i < rdx; i++)
			{
				video_write_byte(*p);
				p++;
			}
			break;

		case SYSCALL_TIME_READ:
			*p = get_time(rbx);
			break;

		case SYSCALL_TIME_WRITE:
			set_time(rdx, rbx);
			break;

		case SYSCALL_SCREEN_SAVER_SET:
			video_set_screen_saver_timer(rdx);
			break;

		case SYSCALL_MALLOC:
			*p1=allocate();
			//video_write_byte(*p);
			break;
		case SYSCALL_FREE:
			free((void *)p1);
			break;
		default:
	 		break;
	}
}

void timerTick()
{
	video_screen_saver_check_count();
}

void keyboard()
{
	video_screen_saver_check_restore();

	keyboard_buffer_write();	
}
