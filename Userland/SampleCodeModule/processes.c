#include <processes.h>
#include <lib.h>
#include <time.h>


void semafor_down()
{
	sem_down();
	//print("Baje semaforo");
	int i;
	for (i=0;i<300000000;i++);
	//int times=0;
	//while(1);
	sem_up();
	//print("Subi");
	//fore(getppid());
	while(1);
}

void semafor_test()
{
	print("Intento bajar sem\n");
	sem_down();
	print("baje sem\n");
	sem_up();
	print("levante sem\n");
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