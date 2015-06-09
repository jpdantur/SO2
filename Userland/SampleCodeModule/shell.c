#include <shell.h>
#include <time.h>


extern char bss;
extern char endOfBinary;

void sys_screen_saver_set(int time);

void shell()
{
	memset(&bss, 0, &endOfBinary - &bss);
	//Solves constant space problem
	char * _sssss = "Bienvenido a la consola de arqui, la mejor consola de todas\n";

	char bff[256];
	char name[21];
	int a;

	print ("Introducir nombre de usuario (Max 20 caracteres)>");
	a = scan(name, 21);
	
	while(a==0)
	{
		print("Tiene que introducir un nombre!\n");
		print ("Introducir nombre de usuario (Max 20 caracteres)>");
		a = scan(name, 21);
	}
	remove_new_line(name);
	tCommand command;
	
	while (1)
	{
		//Print prompt
		print(name);
		print(">");

		a = scan(bff, 256);

		if (a != 0){

			a = shell_buffer_parser(&command, bff, a);

			if (a != -1){
				a = shell_command_execute(&command);

				if (a == INVALID_COMMAND)
					print("Comando invalido.\n");
			}else{
				print("Comando invalido.\n");
			}
		}
	}
}
void remove_new_line(char * value)
{
	while(*value)
	{
		if (*value == '\n')
			*value = 0;
		value++;
	}
}
int shell_buffer_parser(tCommand * command, char * bff, int bff_len)
{
	flush(command);
	char * p;
	int dispatcher = 0;
	int struct_index[2] = {0, 0};

	if (bff[0] == 0 || bff[0] == ' ')
		return -1;

	for (int i = 0; i < bff_len; i++)
	{

		if (bff[i] == '\n'){
			return dispatcher;
		}

		if (bff[i] == ' ' && dispatcher == 0)
		{
			if (bff[i+1]=='\n')
			{
				p[struct_index[dispatcher]] = bff[i];
				struct_index[dispatcher]++;
				p[struct_index[dispatcher]] = 0;
			}
			dispatcher++;
		}
		else
		{
			if (dispatcher == 0)
				p=command->primary;
			else
				p=command->args;

			p[struct_index[dispatcher]] = bff[i];
			struct_index[dispatcher]++;
			p[struct_index[dispatcher]] = 0;
		}

		if (struct_index[dispatcher] > GET_MAX_LEN(dispatcher))
		{
			return -1;
		}

	}

	return dispatcher;
}

int shell_command_execute(tCommand * command)
{
	char * primary = command->primary;
	char * args = command->args;
	int retval;

	if (strcmp("time", primary) == 0 && *args==0)
	{
		shell_print_time();
		retval = 0;
	}
	else if (strcmp("set_screensaver", primary) == 0)
	{
		//print("Entre a set\n");
		//if (strcmp("screensaver", secondary) == 0){
		if (shell_set_screen_saver_time(args) == -1)
		{
			retval = -1;
		}
		else
		{
			print("El protector de pantalla se activara despues de ");
			print(args);
			print(" segundos de inactividad.\n");
			retval = 0;
		}
	}
	else if (strcmp("set_hour", primary) == 0)
	{
		int aux = shell_validate_time(args, 2);
		if (aux != -1)
		{
			set_time_att(2,dtoh(aux));
			retval = 0;
		}
	}
	else if (strcmp("set_min", primary) == 0)
	{
		int aux = shell_validate_time(args, 1);
		if (aux != -1)
		{
			set_time_att(1,dtoh(aux));
			retval = 0;
		}
	}
	else if (strcmp("set_sec", primary) == 0)
	{
		int aux = shell_validate_time(args, 0);
		if (aux != -1)
		{
			set_time_att(0,dtoh(aux));
			retval = 0;
		}
	}
	else if (strcmp("set_time", primary) == 0)
	{
		retval = shell_set_whole_time(args);
	}
	else if (strcmp("echo", primary) == 0)
	{
		print(args);
		putchar('\n');
	}
	else if (strcmp("test", primary) == 0 && *args==0)
	{
		print("Prueba int: ");
		printint(20);
		putchar('\n');
		print("Prueba String: ");
		print("Esto es un string constante de prueba enviado mediante print.\n");
		print("Prueba hexadecimal: ");
		printhex(0x32);
		putchar('\n');
		retval=0;
	}
	else if (strcmp("help", primary) == 0 && *args==0)
	{
		print("Menu de Ayuda\n");
		print("Configurar screensaver: set_screensaver [segs]\n");
		print("Ver la hora del sistema: time\n");
		print("Setear la hora del sistema: set_time [hora - HH:MM:SS]\n");
		print("Setear hora/min/seg del sistema (Por separado): set_[hour| min|sec] [value]\n");
		print("Imprimir en pantalla: echo [value]\n");
		print("Probar funciones de impresion: test\n");
		retval = 0;
	}
	else
	{
		retval = -1;
	}

	return retval;
}

void flush(tCommand * command){
	*(command->primary) = 0;
	*(command->args) = 0;
}

void shell_print_time(void)
{
	int v_time[3] = {get_time(HOUR), get_time(MINUTES), get_time(SECONDS)};

	for (int i = 0; i < 3; i++){
		
		if (v_time[i] < 10)
			putchar('0');

		printint(v_time[i]);

		if (i != 2)
			putchar(':');
	}

	putchar('\n');
}


//Receives time in sec
int shell_set_screen_saver_time(char * time)
{
	int num = 0;

	while (*time){
		if (*time < '0' || *time > '9')
			return -1;

		num *= 10;
		num += *time - '0';

		time++;
	}
	if (num == 0)
		return -1;
	sys_screen_saver_set((int)(num * 18.18182));
	return 0;
}

int shell_validate_time(char * value, int type)
{
	int i;
	int res = 0;
	while (*value)
	{
		if (!isnum(*value))
			return -1;
		
		res *= 10;
		res += *value - '0';
		i++;
		value++;
	}
	if (res >= (type == 2? 24 : 60))
		return -1;

	return res;
}

int shell_set_whole_time(char * value)
{
	int i;
	char aux[3];
	int nums[3];

	for (i = 0; i < 3; i++)
	{
		int j = 0;
		while ((*value != 0) && (*value != ':'))
		{
			if (j >= 2)
			{
				return -1;
			}
			aux[j] =* value;
			value++;
			j++;
		}
		if (j < 2 || (*value == 0 && i != 2) || (*value != 0 && i == 2))
		{
			return -1;
		}
		aux[2] = 0;
		nums[i] = shell_validate_time(aux,2-i);
		if (nums[i] == -1)
		{
			return -1;
		}
		value++;
	}
	set_time_att(0, dtoh(nums[2]));
	set_time_att(1, dtoh(nums[1]));
	set_time_att(2, dtoh(nums[0]));

	return 0;

}
