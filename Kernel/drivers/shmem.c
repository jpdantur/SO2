#include <alloc.h>
#include <scheduler.h>
#include <shmem.h>
#include <video.h>
#include <naiveConsole.h>
void * shmem;
int sem;
Nodo * waiting_first;
Nodo * waiting_last;


void shm_init()
{
	waiting_first=NULL;
	waiting_last=NULL;
	sem=1;
	shmem = malloc(sizeof(int));
}

void* get_mem()
{
	return shmem;
}

void down()
{
	//return;
	disable_i();
	if (sem>0)
	{
		__print_debug("Baja sem ");
		sem--;
		enable_i();
		return;
	}
	//video_print("Hola");
	//while(1);
	Nodo *aux;
	aux = malloc(sizeof(Nodo));
	process_slot *cur =get_current();
	aux->pid=get_pid();
	aux->next=NULL;
	//cur->process->state=SLEEPING;
	if (waiting_first==NULL)
	{
			waiting_first = aux;
			waiting_last = aux;
	}
	else
	{
		waiting_last->next=aux;
		waiting_last=aux;
	}
	cur->process->state=SLEEPING;
	//list();
	//while(1);
	//switch_context();
	//video_print("Hola");
	//while(1);
	//switch_context();
	enable_i();
	switch_context();
	//list();
	//int i =0;
	//for (i=0;i<300000000;i++);
	//switch_context();
	//video_print("Hola");
	//while(1);
}

void up()
{	
	//return;
	disable_i();
	//video_print("Hola");
	if (waiting_first==NULL)
	{
		//video_print("aa");
		//while(1);
		sem++;
	}
	else
	{
		video_print("Llegue aca\n");
		Nodo * aux = waiting_first;
		//ncPrintDec(waiting_first->pid);
		//while(1);
		set_state(waiting_first->pid,ACTIVE);
		//video_print("Hola");
		//while(1);
		waiting_first=waiting_first->next;
		free(aux);
		//video_print("Hola");
		//while(1);
		//__print_debug("m");
		//list();
		//while(1);
	}
	//list();
	//while(1);
	//while(1);
	enable_i();
	//video_print("Hola");
	//while(1);
}