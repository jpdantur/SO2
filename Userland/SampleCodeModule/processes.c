#include <processes.h>
#include <lib.h>
#include <time.h>


void semafor_down()
{
	sem_down();
	print("\nP1: Baje semaforo\n");
	int i;
	for (i=0;i<300000000;i++);
	//int times=0;
	//while(1);
	sem_up();
	//while(1)
	print("P1: Subi sem \n");
	//fore(getppid());
	while(1);
}

void semafor_test()
{
	print("P2: Intento bajar sem\n");
	sem_down();
	print("P2: baje sem\n");
	sem_up();
	//while(1);
	print("P2: levante sem\n");
	fore(getppid());
	while(1);
}
void stack1()
{
	int a;
	print("a vale: ");
	printint(a);
	print("\n");
	a=10;
	print("ahora a vale: ");
	printint(a);
	print("\n");
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