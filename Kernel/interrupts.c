#include <naiveConsole.h>
#include <video.h>
#include <keyboard.h>
#include <CMOS.h>

#define SYSCALL_READ 3
#define SYSCALL_WRITE 4
#define SYSCALL_TIME_READ 2
#define SYSCALL_TIME_WRITE 5
#define SYSCALL_SCREEN_SAVER_SET 6

char get_call(void);
char get_rax(void);
char get_rcx(void);
void set_rax(char c);


void int80(char *p, int type, int size)
{
	char call = get_rax();
	
	char c;
	int i;
	
	char enter = 0;
	switch (call)
	{
		case SYSCALL_READ:
			for (i = 0; i < size - 1 && !enter; i++)
			{	
				*p = keyboard_buffer_read();
				if (*p == '\n')
					enter = 1;
				p++;
			}

			if (!enter)
			{
				*p = '\n';
				video_new_line();
			}
			break;

		case SYSCALL_WRITE:
			for (i = 0; i < size; i++)
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

		case SYSCALL_SCREEN_SAVER_SET:
			video_set_screen_saver_timer(size);
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
