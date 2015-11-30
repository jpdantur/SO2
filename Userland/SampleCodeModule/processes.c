#include <processes.h>
#include <lib.h>


void proc1()
{
	char * buf = malloc();
	print("Estoy en el proceso1");
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