#include <alloc.h>

//Aca va el scheduler
ProcessSlot *current=NULL;

void enqueue(ProcessSlot *p)
{
	if (current==NULL)
	{
		current=p;
		current->next=p;
	}
	else
	{
		ProcessSlot * aux =current->next;
		current->next=p;
		p->next=aux;
	}
}

void remove_process(ProcessSlot * process) {
	ProcessSlot * prevSlot = current;
	ProcessSlot * slotToRemove = current->next;

	if (current == NULL) {
		return;
	} else if (prevSlot == slotToRemove && process == current) {
		current = NULL;
		return;
	}

	while(slotToRemove != process) {
		prevSlot = slotToRemove;
		slotToRemove = slotToRemove->next;
	}

	prevSlot->next = slotToRemove->next;
}

void next_process()
{
	do
	{
		current=current->next;
	} while(current->state==SLEEPING);
}