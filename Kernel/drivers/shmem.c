#include <alloc.h>
void * shmem;
int sem;

void shm_init()
{
	sem=1;
	shmem = malloc(sizeof(void));
}

void* get_mem()
{
	return shmem;
}

void down()
{
	while(sem==0);
	sem--;
}

void up()
{
	if (sem==0)
		sem++;
}