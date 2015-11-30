
#include <paging.h>
#include <scheduler.h>
#include <alloc.h>
#include <const.h>
#include <kernel.h>
#include <keyboard.h>
#include <video.h>
static uint64 identityCR3;
static L1_TABLE* l1_tables[6];

//Auxiliary
static uint64 backupCR3;
static int interruptionsStateBackup;

//INITIALIZATION FUNCTIONS

void PagingInitialize(){

	//We create 5 initial tables so that we can preserve identity in the first 10 MiB
	//If we modify this we should also modify NewL4Table ofc
	l1_tables[0] = CreateIdentityL1Table(1,0,0);
	l1_tables[1] = CreateIdentityL1Table(1,0,1);
	l1_tables[2] = CreateIdentityL1Table(1,1,2); //Userland
	l1_tables[3] = CreateIdentityL1Table(1,1,3); //Kernel malloc space
	l1_tables[4] = CreateIdentityL1Table(1,1,4);
	l1_tables[5] = CreateIdentityL1Table(1,0,5); //Kernel pages space

	identityCR3 = (uint64) InitializeIdentityDirectories();
	WRITE_CR3(identityCR3);

}

L1_TABLE* CreateIdentityL1Table(int readWrite, int userSupervisor, int offsetIndex) {
  L1_TABLE* l1t = (L1_TABLE*)PageAlloc();
  int i;
  for (i = 0; i < 512; i++) {
    l1t->table[i].p = 1;
    l1t->table[i].rw = readWrite;
    l1t->table[i].us = userSupervisor;
    // each l2 table uses 2MiB, so index * 2MiB gives us the starting position
    // We divide by PAGE_SIZE so it shifts the 12 0's and maps correctly virtual to physical adress
    l1t->table[i].addr = ((offsetIndex * 2 * 0x100000) + (i*PAGE_SIZE))/PAGE_SIZE;
  }
  return l1t;
}

L4_TABLE * InitializeIdentityDirectories(){
	L4_TABLE* l4t = (L4_TABLE*) PageAlloc();
	L3_TABLE* l3t = (L3_TABLE*) PageAlloc();

	//L4.Addr -> L3
	l4t->table[0].p = 1;
  	l4t->table[0].rw = 1;
  	l4t->table[0].us = 0;
  	l4t->table[0].addr = (uint64)((uint64)l3t/PAGE_SIZE);

	L2_TABLE* l2t = (L2_TABLE*) PageAlloc();

	//L3.Addr -> L2
	l3t->table[0].p = 1;
  	l3t->table[0].rw = 1;
  	l3t->table[0].us = 0;
  	l3t->table[0].addr = (uint64)((uint64)l2t/PAGE_SIZE);

  	//L2.Addr -> L1
  	//L1.Addr = physical addr ( Identity )
  	int i;
  	int j;
  	for (i = 0; i < 256; i++) {
	    L1_TABLE* l1t = (L1_TABLE*)PageAlloc();
	    l2t->table[i].p = 1;
	    l2t->table[i].us = 0;
	    l2t->table[i].rw = 0;
	    l2t->table[i].addr = ((uint64)l1t/PAGE_SIZE);

	    for (j = 0; j < 512; j++) {
	      l1t->table[j].p = 1;
	      l1t->table[j].us = 0;
	      l1t->table[j].rw = 0;
	      l1t->table[j].addr = (uint64)(((i * 2 * 0x100000)+(j * PAGE_SIZE ))/PAGE_SIZE);
	    }
  	}

  	return l4t;
}

// NEW PROCESS FUNCTIONS

L4_TABLE* NewProcessPagination(){
	BackUpState();
	L4_TABLE* cr3 = NewL4Table();
	RestoreState();
	return cr3;
}

L4_TABLE* NewL4Table(){
	L4_TABLE* l4t = (L4_TABLE*) PageAlloc();
	L3_TABLE* l3t = (L3_TABLE*) PageAlloc();

	//L4.Addr -> L3
	l4t->table[0].p = 1;
  	l4t->table[0].rw = 1;
  	l4t->table[0].us = 0;
  	l4t->table[0].addr = (uint64)((uint64)l3t/PAGE_SIZE);

	L2_TABLE* l2t = (L2_TABLE*) PageAlloc();

	//L3.Addr -> L2
	l3t->table[0].p = 1;
	l3t->table[0].rw = 1;
	l3t->table[0].us = 0;
	l3t->table[0].addr = (uint64)((uint64)l2t/PAGE_SIZE);

  //L2 mapping
	int i;
	for (i = 0; i < 6; i++) {
    l2t->table[i].p = 1;
    l2t->table[i].rw = 1;
    l2t->table[i].addr = (uint64)l1_tables[i]/PAGE_SIZE;
    if(i == 2){
    	l2t->table[i].us = 1;
    }else{
    	l2t->table[i].us = 0;
    }
	}
}


uint64 AllocNewProcessStack(L4_TABLE* l4t, uint64 entryPoint) {
  BackUpState();

  uint64 firstPage;

  Offsets* off 	= (Offsets*) malloc(sizeof(Offsets));
  int i, j;
  for (i = 11; i < 15 ; i++) {
    for (j = 0; j < 512; j++) {
      off->L4 = 0;
      off->L3 = 0;
      off->L2 = i;
      off->L1 = j;
      //first page will be the last one added
      firstPage = AddPage(l4t, off, 1, 1);
    }
  }

  uint64_t addr = firstPage + PAGE_SIZE - sizeof(stack_frame);
  stack_frame* frame = (stack_frame*)addr;

  frame->gs     = 0x001;
  frame->fs     = 0x002;
  frame->r15    = 0x003;
  frame->r14    = 0x004;
  frame->r13    = 0x005;
  frame->r12    = 0x006;
  frame->r11    = 0x007;
  frame->r10    = 0x008;
  frame->r9     = 0x009;
  frame->r8     = 0x00A;
  frame->rsi    = 0x00B;
  frame->rdi    = 0x00C;
  frame->rbp    = 0x00D;
  frame->rdx    = 0x00E;
  frame->rcx    = 0x00F;
  frame->rbx    = 0x010;
  frame->rax    = 0x011;
  frame->rip    = (uint64)entryPoint;
  frame->cs     = 0x008;
  frame->eflags = 0x202;
  frame->rsp    = (30 * 0x100000);
  frame->ss     = 0x000;
  frame->base   = 0x000;

  RestoreState();
  return (30 * 0x100000) - sizeof(frame);
}

// FAULT HANDLER
void PageFaultHandler(uint64 error, uint64 cr2) {
interruptionsStateBackup = SetInterruptions(FALSE);
backupCR3 = READ_CR3();

  Offsets* off 	= (Offsets*) malloc(sizeof(Offsets));
  off->phyOff 	= cr2      & 0x0000000000000FFF;
  off->L1    	= ((cr2    & 0x00000000001FF000) >> 12) & 0x00000000000001FF;
  off->L2    	= ((cr2    & 0x000000003FE00000) >> 21) & 0x00000000000001FF;
  off->L3    	= ((cr2    & 0x0000007FC0000000) >> 30) & 0x00000000000001FF;
  off->L4    	= ((cr2	   & 0x0000FF8000000000) >> 39) & 0x00000000000001FF;

  if (cr2 < (22 * 0x100000)) {
    // Proccess requested memory ourside stack bounds
    while(1);
  }
  else if (cr2 >= (22 * 0x100000) && cr2 < (30 * 0x100000)) {
    // Proccess needs memory in stack
    switch_u2k();
    WRITE_CR3(identityCR3);
    AddPage(backupCR3, off, 1, 1);
    WRITE_CR3(backupCR3);
    switch_k2u();
  }
  else if (cr2 >= (30 * 0x100000) && cr2 < (32 * 0x100000)) {
  	// Proccess needs memory in heap
  	switch_u2k();
  	WRITE_CR3(identityCR3);
    AddPage(backupCR3, off, 1, 1);
    WRITE_CR3(backupCR3);
    switch_k2u();

  }
  else if (cr2 >= (32 * 0x100000)) {
    // Proccess requested memory ourside heap bounds
    while(1);
  }

  SetInterruptions(interruptionsStateBackup);
  return;
}

// MANAGMENT FUNCTIONS

uint64 AddPage(L4_TABLE* l4t, Offsets* offsets, int rw, int us){
  L3_TABLE* l3t = GetL3(l4t, offsets->L4, rw, us);
  L2_TABLE* l2t = GetL2(l3t, offsets->L3, rw, us);
  L1_TABLE* l1t = GetL1(l2t, offsets->L2, rw, us);
  uint64 page = GetPhysicalPage(l1t, offsets->L1, rw, us);
  return page;
}

L3_TABLE* GetL3(L4_TABLE* l4t, uint64 offset, int rw, int us){
	if (l4t->table[offset].p == 0) {
		l4t->table[offset].p = 1;
		l4t->table[offset].rw = rw;
		l4t->table[offset].us = us;

		L3_TABLE* l3t = (L3_TABLE*)PageAlloc();
		l4t->table[offset].addr = (uint64) ((uint64)l3t / PAGE_SIZE);
		return l3t;

	}else{
		return (L3_TABLE*) ((uint64)(l4t->table[offset].addr * PAGE_SIZE));
	}
}

L2_TABLE* GetL2(L3_TABLE* l3t, uint64 offset, int rw, int us){
	if (l3t->table[offset].p == 0) {
		l3t->table[offset].p = 1;
		l3t->table[offset].rw = rw;
		l3t->table[offset].us = us;

		L2_TABLE* l2t = (L2_TABLE*)PageAlloc();
		l3t->table[offset].addr = (uint64) ((uint64)l2t / PAGE_SIZE);
		return l2t;

	}else{
		return (L2_TABLE*) ((uint64)(l3t->table[offset].addr * PAGE_SIZE));
	}
}

L1_TABLE* GetL1(L2_TABLE* l2t, uint64 offset, int rw, int us){
	if (l2t->table[offset].p == 0) {
		l2t->table[offset].p = 1;
		l2t->table[offset].rw = rw;
		l2t->table[offset].us = us;

		L1_TABLE* l1t = (L1_TABLE*)PageAlloc();
		l2t->table[offset].addr = (uint64) ((uint64)l1t / PAGE_SIZE);
		return l1t;

	}else{
		return (L1_TABLE*) ((uint64)(l2t->table[offset].addr * PAGE_SIZE));
	}
}

uint64 GetPhysicalPage(L1_TABLE* l1t, uint64 offset, int rw, int us){
	if (l1t->table[offset].p == 0) {
		l1t->table[offset].p = 1;
		l1t->table[offset].rw = rw;
		l1t->table[offset].us = us;
    	l1t->table[offset].addr = (uint64)((uint64)PageAlloc() / PAGE_SIZE);
	}
	
	return (uint64) ((uint64)(l1t->table[offset].addr * PAGE_SIZE));
	
}

// FREE FUNCTIONS

void FreeL4(L4_TABLE* l4t){
	BackUpState();

	int i;
	for(i=0; i < 512; i++){
		L4_ENTRY entry = l4t->table[i];
		if(entry.p != 0){
			entry.p = 0;
			FreeL3((L3_TABLE*)(entry.addr * PAGE_SIZE));
		}
	}

	free((void*)l4t);

	RestoreState();
}

void FreeL3(L3_TABLE* l3t){
	int i;
	for(i=0; i < 512; i++){
		L3_ENTRY entry = l3t->table[i];
		if(entry.p != 0){
			entry.p = 0;
			FreeL2((L2_TABLE*)(entry.addr * PAGE_SIZE));
		}
	}

	free((void*)l3t);
}

void FreeL2(L2_TABLE* l2t){
	int i;
	for(i=0; i < 512; i++){
		L2_ENTRY entry = l2t->table[i];
		if(entry.p != 0){
			entry.p = 0;
			FreeL1((L1_TABLE*)(entry.addr * PAGE_SIZE));
		}
	}

	free((void*)l2t);
}

void FreeL1(L2_TABLE* l2t){
	int i;
	for(i=0; i < 512; i++){
		L2_ENTRY entry = l2t->table[i];
		if(entry.p != 0){
			entry.p = 0;

			//Checking if im trying to free kernel pages
			if(entry.addr * PAGE_SIZE > 10 * 0x100000){
				free((void*)(entry.addr * PAGE_SIZE));
			}
		}
	}

	free((void*)l2t);
}

// HELPERS

void BackUpState(){
	interruptionsStateBackup = SetInterruptions(FALSE);
	backupCR3 = READ_CR3();
	WRITE_CR3(identityCR3);
}

void RestoreState(){
	WRITE_CR3(backupCR3);
  	SetInterruptions(interruptionsStateBackup);
}