#include <alloc.h>
#include <scheduler.h>
#include <video.h>

//Aca va el scheduler
process_slot *current=NULL;
int forepid=0;
int nextpid=0;

process_slot * get_current()
{
	return current;
}
void list()
{
	process_slot * this = current;
	do
	{
		video_write_byte(this->process->pid+'0');
		video_write_byte(' ');
		video_write_byte(this->process->state+'0');
		video_write_byte('\n');
		this=this->next;
	}while(this!=current);
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
	int flag=0;
	process_slot * start = current;
	process_slot * this = current;
	do
	{
		if (this->process->pid==pid)
		{
			remove_process(this->process);
			flag=1;
		}
		this=this->next;
	}while (this != current && flag == 0);
}

void next_process()
{
	//__video_debug('h');
	do
	{
		current=current->next;
	} while(current->process->state==SLEEPING);
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
	if (current==NULL)
		return esp;
	//__video_debug('h');
	Process * process = current->process;
	process->regs = esp;
	return process->kernel;
}

void * to_stack_address(void * page) {
	return (uint8_t*)page + 4096 - 0x10;
}

Process *new_process(void * entry_point) {
	Process *p = allocate();
	p->entry=entry_point;
	p->regs_page=allocate();
	p->kernel_page=allocate();
	p->regs=to_stack_address(p->regs_page);
	p->kernel = to_stack_address(p->kernel_page);
	p->regs = fill_stack_frame(entry_point, p->regs);
	p->state=ACTIVE;
	p->pid=nextpid;
	nextpid++;
	return p;
}
process_slot * new_process_slot(Process *p)
{
	process_slot * ret = allocate();
	ret->process=p;
	return ret;
}

void * fill_stack_frame(void * entry_point, void * user_stack) {
	stack_frame * frame = (stack_frame*)user_stack - 1;
	frame->gs =		0x001;
	frame->fs =		0x002;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	0x00B;
	frame->rdi =	0x00C;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	(uint64_t)entry_point;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return frame;
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
	free(p->process->regs_page);
	free(p->process->kernel_page);
	free(p->process);
	free(p);
}

void set_current_fore()
{
	set_forepid(current->process->pid);
}