typedef unsigned int uint;
typedef unsigned char uchar;
#define NULL 0

uchar* addr_from_index(uint i);

uint index_from_addr(const uchar* p);

void* allocate();

void free(void* p);