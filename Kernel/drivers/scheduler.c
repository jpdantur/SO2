#include <alloc.h>
#include <scheduler.h>
#include <video.h>
#include <stdint.h>
#include <interrupts.h>
#include <const.h>
#include <paging.h>

process_slot *current=NULL;

int forepid=0;
int nextpid=0;


int get_pid()
{
	return current->process->pid;
}
int get_ppid()
{
	return current->process->ppid;
}
process_slot * get_current()
{
	return current;
}
void list()
{
	process_slot * this = current;
	do
	{
		video_print("PID: ");
		video_write_byte(this->process->pid+'0');
		video_write_byte(' ');
		video_print("Estado: ");
		video_write_byte(this->process->state+'0');
		video_print(" Nombre: ");
		video_print(this->process->name);
		video_write_byte('\n');
		this=this->next;
	}while(this!=current);
	video_print("Foreground pid: ");
	video_write_byte(get_forepid()+'0');
	video_write_byte('\n');
}
int enqueue(Process *p)
{
	process_slot* proc = new_process_slot(p);
	if (current==NULL)
	{
		//__video_debug('m');
		current=proc;
		current->next=proc;
	}
	else
	{
		process_slot * aux = current->next;
		current->next=proc;
		proc->next=aux;
	}
	return p->pid;
}

void remove_process(Process * process) {
	process_slot * prev_slot = current;
	process_slot * slot_to_remove = current->next;

	if (current == NULL) {
		return;
	} else if (prev_slot == slot_to_remove && process == current->process) {
		delete(current);
		current = NULL;
		return;
	}

	while(slot_to_remove->process != process) {
		prev_slot = slot_to_remove;
		slot_to_remove = slot_to_remove->next;
	}

	prev_slot->next = slot_to_remove->next;
	delete(slot_to_remove);
}
void kill(int pid)
{
	if (pid==0)
		return;
	int flag=0;
	process_slot * start = current;
	process_slot * this = current;
	do
	{
		if (this->process->pid==pid)
		{
			set_parents(pid,this->process->ppid);
			remove_process(this->process);
			flag=1;
			//if (this==current)
			//	set_rsp(switch_kernel_to_user());
		}
		this=this->next;
	}while (this != start && flag == 0);
}

void set_parents(int pid, int ppid)
{
	process_slot * start = current;
	process_slot * this = current;
	do
	{
		if (this->process->ppid==pid)
		{
			this->process->ppid=ppid;
		}
		this=this->next;
	}while (this != start);	
}
void next_process()
{
	//__video_debug(current->process->pid+'0');
	do
	{
		current=current->next;
	} while(current->process->state==SLEEPING);

	set_process_last_malloc(current->process->malloc_current);
}

void * switch_kernel_to_user() {
	if (current==NULL)
	{
		//__video_debug('l');
		return 0;
	}
	//__video_debug('o');
	next_process();
	return current->process->regs;
}

void * switch_user_to_kernel(void * esp) {
	//video_print("wassap");
	if (current==NULL)
		return esp;
	//__video_debug('h');
	Process * process = current->process;
	process->regs = esp;
	process->malloc_current = get_process_malloc();

	return process->kernel;
}

void * to_stack_address(void * page) {
	return (uint8_t*)page + 0x1000 - 0x10;
}


Process *new_process(void * entry_point, char *name) {
	Process *p = malloc(sizeof(Process));
	p->entry=entry_point;
	p->regs_page=malloc(sizeof(stack_frame));
	p->kernel_page=malloc(sizeof(stack_frame));
	p->kernel = to_stack_address(p->kernel_page);

	p->cr3 = NewProcessPagination();
	p->regs = AllocNewProcessStack(p->cr3, entry_point);

	p->malloc_current = get_sys_malloc();

	p->state=ACTIVE;
	p->pid=nextpid;
	p->name=name;
	if (nextpid!=0)
	{
		p->ppid=current->process->pid;
	}
	nextpid++;
	return p;
}

process_slot* new_process_slot(Process *p)
{
	process_slot * ret = malloc(sizeof(process_slot));
	ret->process=p;
	return ret;
}

int get_forepid()
{
	return forepid;
}

void set_forepid (int p)
{
	forepid=p;
}

void delete(process_slot *p)
{
	if (get_forepid()==p->process->pid)
		set_forepid(p->process->ppid);
	FreeL4(p->process->cr3);
	free(p->process->regs_page);
	free(p->process->kernel_page);
	free(p->process);
	free(p);
}

void set_current_fore(int pid)
{
	set_forepid(pid);
}

void set_state(int pid, int state)
{
	int flag=0;
	process_slot * start = current;
	process_slot * this = current;
	do
	{
		if (this->process->pid==pid)
		{
			this->process->state=state;
			flag=1;
		}
		this=this->next;
	}while (this != start && flag == 0);
}