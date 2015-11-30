#include <types.h>
#include <alloc.h>
#include <video.h>
#include <kernel.h>
#include <const.h>
#include <paging.h>

//We're going to map 512 MiB of memory, so we'll use around 1MiB for pages ( 131072 to be precise )
static uint64* pagesStackStart = (uint64*)(10 * 0x100000);
static uint64* pagesStackEnd = (uint64*)(12 * 0x100000);
static uint64* pagesStackCurrent;

//malloc will go from 6 to 10 MiB. This should be enough
static void* mallocStart = (6 * 0x100000);
static void* lastMalloc;

void* malloc(int length)
{
	bool interruptions = SetInterruptions(FALSE);


	lastMalloc = mallocStart;
	if((mallocStart + length) > pagesStackStart){
		video_print("ERROR ---------> OutOfMemory");
	}
	mallocStart += length * sizeof(char);

	SetInterruptions(interruptions);

	return lastMalloc;
}
 

 void free(void* p)
 {
 	//We're doing secuential malloc. So free doesnt make sense ( Not an optimal approach of course )
 	//TODO: Change malloc implementation in the future
 } 

void PageManagmentInitialize() {

	uint64 freeMemoryStart = (uint64)pagesStackEnd;
	pagesStackCurrent = pagesStackStart;
	uint64 pageAmount = (512 * 0x100000 - freeMemoryStart) / PAGE_SIZE;

	//Initialize page stack pointing to pages ( not yet defined ) of ALL memory available ( 512 MiB )
	uint64 i;
	for (i = 0 ; i < pageAmount; i++) {
		*pagesStackCurrent = freeMemoryStart + (i * PAGE_SIZE);
		pagesStackCurrent++;
	}

}

void* PageAlloc() {
	if (pagesStackCurrent-1 == pagesStackStart)
		video_print("ERROR ---------> OutOfMemory");

	pagesStackCurrent--;

	//Initialize page with 0's
	memset_long((void*)*pagesStackCurrent, 0, 512);
	return (void*)(*pagesStackCurrent);
}

void PageFree(void* pageDir) {
	// We asume we're working with already aligned directions here...
	*pagesStackCurrent = (uint64)pageDir;
	pagesStackCurrent++;
}

void * memset_long(void * destiation, uint64 c, uint64 length) {
	uint64_t * dst = (uint64_t*)destiation;

	for (uint64_t i = 0; i < length; i++) {
		dst[i] = c;
	}

	return destiation;
}