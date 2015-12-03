#define SLEEPING 0
#define ACTIVE 1
#include <stdint.h>
#define NULL 0

//Process_slot sleepy;
//typedef unsigned long uint64_t;
//typedef unsigned char uint8_t;
typedef struct
{
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;	
} stack_frame;

typedef struct 
{
	void * entry;
	stack_frame * regs;
	stack_frame * regs_page;
	int state;
	int pid;
	int ppid;
	char *name;
} Process;

typedef struct slot
{
	Process * process;
	struct slot * next;
} process_slot;

void next_process();
int enqueue(Process *p);
void remove_process(Process * process);
void * switch_kernel_to_user();
void * switch_user_to_kernel(void * esp);
void * to_stack_address(void * page);
Process *new_process(void * entry_point, char *name);
process_slot * new_process_slot(Process *p);
void * fill_stack_frame(void * entry_point, void * user_stack);
int get_forepid();
void set_forepid (int p);
void delete(process_slot *p);
process_slot * get_current();
process_slot * get_next();
void set_current_fore(int pid);
void kill(int pid);
void set_parents(int pid, int ppid);
void set_state(int pid, int state);
int get_pid();
int get_ppid();
void sched_sleep(int param_time, int pid);
void sched_check_sleep();