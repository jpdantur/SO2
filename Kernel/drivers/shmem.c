#include <alloc.h>
#include <scheduler.h>
#include <shmem.h>

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
	Nodo *aux;
	aux = malloc(sizeof(Nodo));
	get_current()->process->state=SLEEPING;
	if (waiting_first==NULL)
	{
		if (sem==0)
		{
			waiting_first = aux;
			waiting_last = aux;
		}
		else
		{
			sem=0;
		}

	}
	else
	{
		waiting_last->next=aux;
		waiting_last=aux;
	}
	switch_context();
}

void up()
{	
	sem=1;
	Nodo * aux = waiting_first;
	if (waiting_first!=NULL)
	{
		set_state(waiting_first->pid,ACTIVE);
		waiting_first=waiting_first->next;
		free(aux);
	}
}