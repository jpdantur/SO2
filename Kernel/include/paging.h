#ifndef PAG_H
#define PAG_H

#include <types.h>

typedef struct {
  unsigned long res1 	: 3;
  unsigned long pwt     : 1;
  unsigned long pcd     : 1;
  unsigned long res2 	: 7;
  unsigned long addr   	: 40;
  unsigned long res3 	: 12;
} CR_3;

typedef struct {
  unsigned long p         : 1;
  unsigned long rw        : 1;
  unsigned long us        : 1;
  unsigned long pwt       : 1;
  unsigned long pcd       : 1;
  unsigned long a         : 1;
  unsigned long ign       : 1;
  unsigned long mbz       : 2;
  unsigned long avl       : 3;
  unsigned long addr 	  : 40;
  unsigned long avail 	  : 11;
  unsigned long nx        : 1;
} L4_ENTRY;

typedef struct {
  L4_ENTRY table[512];
} L4_TABLE;

typedef struct {
  unsigned long p         : 1;
  unsigned long rw        : 1;
  unsigned long us        : 1;
  unsigned long pwt       : 1;
  unsigned long pcd       : 1;
  unsigned long a         : 1;
  unsigned long ign       : 1;
  unsigned long mbz       : 2;
  unsigned long avl       : 3;
  unsigned long addr 	  : 40;
  unsigned long avail 	  : 11;
  unsigned long nx        : 1;
} L3_ENTRY;

typedef struct {
  L3_ENTRY table[512];
} L3_TABLE;

typedef struct {
  unsigned long p         : 1;
  unsigned long rw        : 1;
  unsigned long us        : 1;
  unsigned long pwt       : 1;
  unsigned long pcd       : 1;
  unsigned long a         : 1;
  unsigned long ign1      : 1;
  unsigned long zero      : 1;
  unsigned long ign2      : 1;
  unsigned long avl       : 3;
  unsigned long addr 	  : 40;
  unsigned long avail 	  : 11;
  unsigned long nx        : 1;
} L2_ENTRY;

typedef struct {
 L2_ENTRY table[512];
} L2_TABLE;

typedef struct {
  unsigned long p         : 1;
  unsigned long rw        : 1;
  unsigned long us        : 1;
  unsigned long pwt       : 1;
  unsigned long pcd       : 1;
  unsigned long a         : 1;
  unsigned long d         : 1;
  unsigned long pat       : 1;
  unsigned long g         : 1;
  unsigned long avl       : 3;
  unsigned long addr 	  : 40;
  unsigned long avail 	  : 11;
  unsigned long nx        : 1;
} L1_ENTRY;

typedef struct {
  L1_ENTRY table[512];
} L1_TABLE;

typedef struct {
  unsigned long phyOff	 	: 12;
  unsigned long L1   		: 9;
  unsigned long L2   		: 9;
  unsigned long L3   		: 9;
  unsigned long L4		   	: 9;
  unsigned long signExtend 	: 16;
} Offsets;


// ASM FUNCTIONS
void WRITE_CR2(uint64 value);
uint64 READ_CR2();
void WRITE_CR3(uint64 value);
uint64 READ_CR2();

// PAGING FUNCTIONS
void PagingInitialize();
void FreeL4(L4_TABLE* l4t);
void FreeL3(L3_TABLE* l3t);
uint64 AllocNewProcessStack(L4_TABLE* l4t, uint64 entryPoint);
L4_TABLE* NewProcessPagination();

L1_TABLE* CreateIdentityL1Table(int readWrite, int userSupervisor, int offsetIndex);
L4_TABLE * InitializeIdentityDirectories();
L4_TABLE* NewL4Table();
uint64 AddPage(L4_TABLE* l4t, Offsets* offsets, int rw, int us);
L3_TABLE* GetL3(L4_TABLE* l4t, uint64 offset, int rw, int us);
L2_TABLE* GetL2(L3_TABLE* l3t, uint64 offset, int rw, int us);
L1_TABLE* GetL1(L2_TABLE* l2t, uint64 offset, int rw, int us);
uint64 GetPhysicalPage(L1_TABLE* l1t, uint64 offset, int rw, int us);


#endif