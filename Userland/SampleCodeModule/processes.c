#include <processes.h>
#include <lib.h>


void proc1()
{
	char * buf = malloc();
	while(1)
	{
		print("Estoy en el proceso 1");
		scan(buf,20);
	}
}