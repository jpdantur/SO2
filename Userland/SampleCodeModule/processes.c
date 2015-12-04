#include <processes.h>
#include <lib.h>
#include <time.h>


void semafor_down()
{
	int* shmem = get_mem();

	fore(getpid());
	print("Proceso 1 intenta bajar semaforo\n");

	sem_down();

	fore(getpid());
	print("Proceso 1 baja semaforo:\n");
	print("    Variable en shared memory tiene valor:\n");
	print("    ");
	printint(*shmem);
	*shmem = *shmem + 2;
	print("\n    Se le suma 2 y ahora tiene valor:\n");
	print("    ");
	printint(*shmem);
	print("\n");

	sleep(5);

	sem_up();
	fore(getpid());
	print("Proceso 1 levanta semaforo\n");

	fore(getppid());
	while(1);
}

void semafor_test()
{
	int* shmem = get_mem();

	fore(getpid());
	print("Proceso 2 intenta bajar semaforo\n");

	sem_down();

	fore(getpid());
	print("Proceso 2 baja semaforo:\n");
	print("    Variable en shared memory tiene valor:\n");
	print("    ");
	printint(*shmem);
	*shmem = *shmem + 3;
	print("\n    Se le suma 3 y ahora tiene valor:\n");
	print("    ");
	printint(*shmem);
	print("\n");
	sem_up();
	fore(getpid());
	print("Proceso 2 levanta semaforo\n");

	fore(getppid());
	while(1);


}
void stack1()
{
	print("dummy");
	fore(getppid());
	while(1);
}

void stack2()
{
	int a;
	print("a vale: ");
	printint(a);
	print("\n");
	a=15;
	print("ahora a vale: ");
	printint(a);
	print("\n");
	fore(getppid());
	while(1);
}