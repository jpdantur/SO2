#include <alloc.h>
#include <scheduler.h>
#include <shmem.h>
#include <video.h>
#include <naiveConsole.h>
#include <kernel.h>
#include <const.h>

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
	int interruptStatus = SetInterruptions(FALSE);
	disable_i();
	if (sem>0)
	{
		__print_debug("Baja sem ");
		sem--;
		SetInterruptions(interruptStatus);
		return;
	}
	Nodo *aux;
	aux = malloc(sizeof(Nodo));
	process_slot *cur =get_current();
	aux->pid=get_pid();
	aux->next=NULL;
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
	SetInterruptions(interruptStatus);
	switch_context();
}

void up()
{	
	int interruptStatus = SetInterruptions(FALSE);
	if (waiting_first==NULL)
	{
		sem++;
	}
	else
	{
		video_print("Llegue aca\n");
		Nodo * aux = waiting_first;
		set_state(waiting_first->pid,ACTIVE);
		waiting_first=waiting_first->next;
		free(aux);
	}
	SetInterruptions(interruptStatus);
}