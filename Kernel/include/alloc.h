#ifndef ALLOC_H
#define ALLOC_H

#include <types.h>

void* malloc(int length);

void free(void* p);

void PageManagmentInitialize();

void* PageAlloc();

void PageFree(void* pageDir);

void * memset_long(void * destiation, uint64 c, uint64 length);

#endif