#include <alloc.h>
#include <scheduler.h>
#include <shmem.h>
#include <video.h>
#include <naiveConsole.h>
#include <kernel.h>
#include <const.h>

static void * shmem;
static int sem;
static Nodo * waiting_first;
static Nodo * waiting_last;


void shm_init()
{
	waiting_first=NULL;
	waiting_last=NULL;
	sem = 1;
	shmem = malloc(sizeof(int));
}

void* get_mem()
{
	return shmem;
}

void down()
{


	if (sem == 1)
	{
		__print_debug("Baja sem ");
		sem = 0;

		return;
	}else{
		ncPrint("2 PROCESO");
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

	switch_context();
}

void up()
{	

	if (waiting_first==NULL)
	{
		sem = 1;
	}
	else
	{
		video_print("\\n");
		Nodo * aux = waiting_first;
		set_state(waiting_first->pid,ACTIVE);
		waiting_first=waiting_first->next;
	}

}
