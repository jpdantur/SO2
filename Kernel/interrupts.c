#include <naiveConsole.h>
#include <video.h>
#include <keyboard.h>
#include <CMOS.h>
#include <alloc.h>
#include <scheduler.h>
#include <shmem.h>
#include <paging.h>

#define SYSCALL_READ 3
#define SYSCALL_WRITE 4
#define SYSCALL_TIME_READ 2
#define SYSCALL_TIME_WRITE 5
#define SYSCALL_SCREEN_SAVER_SET 6
#define SYSCALL_MALLOC 7
#define SYSCALL_FREE 8
#define SYSCALL_NEWPROC 9
#define SYSCALL_FORE 10
#define SYSCALL_KILL 11
#define SYSCALL_LIST 12
#define SYSCALL_GETPID 13
#define SYSCALL_GETPPID 14
#define SYSCALL_SEM_UP 15
#define SYSCALL_SEM_DOWN 16
#define SYSCALL_GET_MEM 17
#define SYSCALL_SLEEP 18

char get_call(void);
char get_rax(void);
char get_rcx(void);
void set_rax(char c);


void int80(int *p1, int rbx, int rdx)
{
	//switch_u2k();

	char call = get_rax();
	//video_print("Rax vale: ");
	//video_write_byte(p1+'0');
	//video_write_byte('\n');
	char *p = (char*)p1;
	char *aux = p;
	char c;
	int i = 0;
	Process *newpr;
	char enter = 0;
	
	switch (call)
	{
		case SYSCALL_SLEEP:
			//video_write_byte(rdx+'0');
			sched_sleep((int)p1,rdx);
			break;
		case SYSCALL_SEM_UP:

			up();
			//video_print("AAAA");
			break;
		case SYSCALL_SEM_DOWN:
			down();
			break;
		case SYSCALL_GET_MEM:
			*(void **)p1=get_mem();
			break;
		case SYSCALL_GETPPID:
			*p1=get_ppid();
			break;
		case SYSCALL_GETPID:
			*p1=get_pid();
			break;
		case SYSCALL_LIST:
			while(get_current()->process->pid!=get_forepid());
			list();
			break;
		case SYSCALL_KILL:
			kill((int)p1);
			break;
		case SYSCALL_FORE:
			set_current_fore((int)p1);
			break;
		case SYSCALL_NEWPROC:
			//__video_debug('h');
			//video_print("wassap");
			//video_write_byte(rdx);
			newpr = new_process((void*)p1, (char*)rbx);
			*((int *)rdx)=enqueue(newpr);
			break;
		case SYSCALL_READ:
			while(get_current()->process->pid!=get_forepid());
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
			while(get_current()->process->pid!=get_forepid());
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
			*p1=malloc(4096);
			//video_write_byte(*p);
			break;
		case SYSCALL_FREE:
			free((void *)p1);
			break;
		default:
	 		break;
	}
	//video_print("AA");

	//switch_k2u();
}

void timerTick()
{
	//video_screen_saver_check_count();
	sched_check_sleep();
}

void keyboard()
{
	video_screen_saver_check_restore();

	keyboard_buffer_write();	
}
