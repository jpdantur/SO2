
#include <paging.h>

static uint64 identityCR3;

/*struct*/ L1_TABLE l1_tables[5];

//Auxiliary
static uint64 backupCR3;
static int interruptionsStateBackup;

//INITIALIZATION FUNCTIONS

void PagingInitialize(){

	//We create 5 initial tables so that we can preserve identity in the first 10 MiB
	//If we modify this we should also modify NewL4Table
	l1_tables[0] = CreateIdentityL1Table(1,0,0);
	l1_tables[1] = CreateIdentityL1Table(1,0,1);
	l1_tables[2] = CreateIdentityL1Table(1,1,2); //Userland
	l1_tables[3] = CreateIdentityL1Table(1,1,3); //Page allocation space
	l1_tables[4] = CreateIdentityL1Table(1,1,4);

	identityCR3 = (uint64) InitializeIdentityDirectories();
	WRITE_CR3(identityCR3);

}

L1_TABLE* CreateIdentityL1Table(int readWrite, int userSupervisor, int offsetIndex) {
  L1_TABLE* l1t = (L1_table*)allocate();
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
	L4_TABLE* l4t = (L4_TABLE*) allocate();
	L3_TABLE* l3t = (L3_TABLE*) allocate();

	//L4.Addr -> L3
	l4t->table[0].p = 1;
  	l4t->table[0].rw = 1;
  	l4t->table[0].us = 0;
  	l4t->table[0].addr = (uint64)((uint64)l3t/PAGE_SIZE);

	L2_TABLE* l2t = (L2_TABLE*) allocate();

	//L3.Addr -> L2
	l3t->table[0].p = 1;
  	l3t->table[0].rw = 1;
  	l3t->table[0].us = 0;
  	l3t->table[0].addr = (uint64)((uint64)l2t/PAGE_SIZE);

  	//L2.Addr -> L1
  	//L1.Addr = physical addr ( Identity )
  	//ASK: 257?
  	int i;
  	int j;
  	for (i = 0; i < 256; i++) {
	    L1_TABLE* l1t = (L1_TABLE*)allocate();
	    l2t->table[i].p = 1;
	    l2t->table[i].us = 0;
	    l2t->table[i].rw = 0;
	    l2t->table[i].addr = ((unit64)l1t/PAGE_SIZE);

	    for (j = 0; j < 512; j++) {
	      l1t->table[j].p = 1;
	      l1t->table[j].us = 0;
	      l1t->table[j].rw = 0;
	      l1t->table[j].addr = (uint64)(((i * 2 * 0x100000)+(j * PAGE_SIZE ))/PAGE_SIZE);
	    }
  	}
}

// NEW PROCESS FUNCTIONS

L4_TABLE* NewProcessPagination(){
	BackUpState();
	L4_TABLE* cr3 = NewL4Table();
	RestoreState();
	return cr3;
}

L4_TABLE* NewL4Table(){
	L4_TABLE* l4t = (L4_TABLE*) allocate();
	L3_TABLE* l3t = (L3_TABLE*) allocate();

	//L4.Addr -> L3
	l4t->table[0].p = 1;
  	l4t->table[0].rw = 1;
  	l4t->table[0].us = 0;
  	l4t->table[0].addr = (uint64)((uint64)l3t/PAGE_SIZE);

	L2_TABLE* l2t = (L2_TABLE*) allocate();

	//L3.Addr -> L2
	l3t->table[0].p = 1;
  	l3t->table[0].rw = 1;
  	l3t->table[0].us = 0;
  	l3t->table[0].addr = (uint64)((uint64)l2t/PAGE_SIZE);
  	int i;
  	for (i = 0; i < 5; i++) {
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

// FAULT HANDLER
void PageFaultHandler(uint64 error, uint64 cr2) {
  BackUpState();

  Offsets* off = malloc(sizeof(Offsets));
  off->phyOff 	= cr2 & 0x0000000000000FFF;
  off->L1    	= ((cr2 & 0x00000000001FF000) >> 12) & 0x00000000000001FF;
  off->L2    	= ((cr2 & 0x000000003FE00000) >> 21) & 0x00000000000001FF;
  off->L3    	= ((cr2 & 0x0000007FC0000000) >> 30) & 0x00000000000001FF;
  off->L4    	= ((dcr2ir & 0x0000FF8000000000) >> 39) & 0x00000000000001FF;

  if (cr2 < (22 * 0x100000)) {
    // Proccess requested memory ourside stack bounds
    //TODO: Change this
    signal_send(task_get_current(), SIGKILL);
  }
  else if (cr2 >= (22 * 0x100000) && cr2 < (30 * 0x100000)) {
    // Proccess needs memory in stack
    AddPage(backupCR3, off, 1, 1);
    WRITE_CR3(backupCR3);
  }
  else if (cr2 >= (30 * 0x100000) && cr2 < (32 * 0x100000)) {
  	// Proccess needs memory in heap
    AddPage(backupCR3, off, 1, 1);
    WRITE_CR3(backupCR3);
  }
  else if (cr2 >= (32 * 0x100000)) {
    // Proccess requested memory ourside heap bounds
    //TODO: Change this
    signal_send(task_get_current(), SIGKILL);
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

L3_TABLE* GetL3(L4_TABLE* l4t, unit64 offset, int rw, int us){
	if (l4t->table[offset].p == 0) {
		l4t->table[offset].p = 1;
		l4t->table[offset].rw = rw;
		l4t->table[offset].us = us;

		L3_TABLE* l3t = (L3_TABLE*)allocate();
		l4t->table[offset].addr = (uint64) ((uint64)l3t / PAGE_SIZE);
		return l3t;

	}else{
		return (L3_TABLE*) ((uint64)(l4t->table[offset].addr * PAGE_SIZE));
	}
}

L2_TABLE* GetL2(L3_TABLE* l3t, unit64 offset, int rw, int us){
	if (l3t->table[offset].p == 0) {
		l3t->table[offset].p = 1;
		l3t->table[offset].rw = rw;
		l3t->table[offset].us = us;

		L2_TABLE* l2t = (L2_TABLE*)allocate();
		l3t->table[offset].addr = (uint64) ((uint64)l2t / PAGE_SIZE);
		return l2t;

	}else{
		return (L2_TABLE*) ((uint64)(l3t->table[offset].addr * PAGE_SIZE));
	}
}

L1_TABLE* GetL1(L2_TABLE* l2t, unit64 offset, int rw, int us){
	if (l2t->table[offset].p == 0) {
		l2t->table[offset].p = 1;
		l2t->table[offset].rw = rw;
		l2t->table[offset].us = us;

		L1_TABLE* l1t = (L1_TABLE*)allocate();
		l2t->table[offset].addr = (uint64) ((uint64)l1t / PAGE_SIZE);
		return l1t;

	}else{
		return (L1_TABLE*) ((uint64)(l2t->table[offset].addr * PAGE_SIZE));
	}
}

uint64 GetPhysicalPage(L1_TABLE* l1t, unit64 offset, int rw, int us){
	if (l1t->table[offset].p == 0) {
		l1t->table[offset].p = 1;
		l1t->table[offset].rw = rw;
		l1t->table[offset].us = us;

		unit64 newPage = uint64 allocate();
		l1t->table[offset].addr = (uint64) (l1t / PAGE_SIZE);
		return newPage;

	}else{
		return (uint64) ((uint64)(l1t->table[offset].addr * PAGE_SIZE));
	}
}

void FreeL4(L4_TABLE* l4t){
	BackUpState();
	int i;
	for(i=0; i < 512; i++){
		L4_ENTRY entry = l4t->table[i];
		if(entry.p != 0){
			entry.p = 0;
			FreeL3((void*)(p.addr * PAGE_SIZE));
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
			FreeL2((void*)(p.addr * PAGE_SIZE));
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
			FreeL1((void*)(p.addr * PAGE_SIZE));
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
			if(p.addr * PAGE_SIZE > 10 * 0x100000){
				free((void*)(p.addr * PAGE_SIZE));
			}
		}
	}

	free((void*)l2t);
}

// HELPERS

void BackUpState(){
  	interruptionsStateBackup = SetInterruptions(FALSE);
  	backupCR3 = READ_CR3();
  	WRITE_CR3(originalCR3);
}

void RestoreState(){
	WRITE_CR3(backupCR3);
  	SetInterruptions(interruptionsStateBackup);
}

// TEST

void l4_table_test() {
  L4_TABLE* l4_table = NewL4Table();
  L3_TABLE* l3_table = (L3_TABLE*)((l4_table->table[0].addr)*PAGE_SIZE);
  L2_TABLE* l2_table = (L2_TABLE*)((l3_table->table[0].addr)*PAGE_SIZE);
  L1_TABLE* _0_to_2_mb = (L1_TABLE*)((l2_table->table[0].addr)*PAGE_SIZE);
  L1_TABLE* _2_to_4_mb = (L1_TABLE*)((l2_table->table[1].addr)*PAGE_SIZE);
  L1_TABLE* _4_to_6_mb = (L1_TABLE*)((l2_table->table[2].addr)*PAGE_SIZE);
  L1_TABLE* _6_to_8_mb = (L1_TABLE*)((l2_table->table[3].addr)*PAGE_SIZE);
  L1_TABLE* _8_to_10_mb = (L1_TABLE*)((l2_table->table[4].addr)*PAGE_SIZE);
  L1_TABLE* _10_to_12_mb = (L1_TABLE*)((l2_table->table[5].addr)*PAGE_SIZE);
  print_l1(_0_to_2_mb, 1);
  print_l1(_2_to_4_mb, 1);
  print_l1(_4_to_6_mb, 1);
  print_l1(_6_to_8_mb, 1);
  print_l1(_8_to_10_mb, 1);
}

void print_l1(PM_L1_TABLE* table, int amount) {
  uint64_t* aux = (uint64_t*)table;
  int i;
  for (i = 488; i < 512; i++) {
    __print_debug("L1[");
   // __video_debug((char) i);
    __print_debug("]: ");
  }
}