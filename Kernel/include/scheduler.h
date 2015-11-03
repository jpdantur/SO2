#define SLEEPING 0
#define ACTIVE 1

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
	stack_frame * kernel;
	int state;
} Process;

typedef struct
{
	Process * process;
	ProcessSlot * next;
} ProcessSlot;


void next_process();
void enqueue(ProcessSlot *p);
void remove_process(ProcessSlot * process);