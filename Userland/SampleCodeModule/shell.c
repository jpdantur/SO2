#include <shell.h>
#include <time.h>


extern char bss;
extern char endOfBinary;

void sys_screen_saver_set(int time);
#define isnum(x) ((x)>='0' && (x)<='9'?1:0)
char * video = (char*)0xB8000 + 79 * 2;

void shell()
{
	memset(&bss, 0, &endOfBinary - &bss);
	//Solve constant space problem
	char * _sssss = "Bienvenido a la consola de arqui, la mejor consola de todas\n";

	char bff[256];
	int a;

	tCommand command;
	
	
	while (1)
	{
		print("NoPrompt OS$ ");
		a = scan(bff, 256);
		a = shell_buffer_parser(&command, bff, a);

		if (a == -1)
		{
			print("Tiro un -1\n");
		}
		else
		{
			a = shell_command_execute(&command);

			if (a == INVALID_COMMAND)
				print("Comando invalido.\n");
		}

	}
}

int shell_buffer_parser(tCommand * command, char * bff, int bff_len)
{
	char * p;
	int dispatcher = 0;
	int struct_index[3] = {0, 0, 0};

	if (bff[0] == 0 || bff[0] == ' ')
		return -1;

	for (int i = 0; i < bff_len; i++)
	{
		if (dispatcher > 2)
			return -1;

		if (bff[i] == '\n'){
			return dispatcher;
		}

		if (bff[i] == ' ')
		{
			dispatcher++;
		}
		else
		{
			switch (dispatcher){
				case 0:
					p = command->primary; 
					break;

				case 1:
					p = command->secondary;
					break;

				case 2:
					p = command->args;
					break;
			}

			p[struct_index[dispatcher]] = bff[i];
			struct_index[dispatcher]++;
			p[struct_index[dispatcher]] = 0;
		}

		if (struct_index[dispatcher] > MAX_PARAMS_LEN)
			return -1;

	}

	return dispatcher;
}

int shell_command_execute(tCommand * command)
{
	char * primary = command->primary;
	char * secondary = command->secondary;
	char * args = command->args;

	int retval;

	if (strcmp("time", primary) == 0)
	{
		shell_print_time();
		retval = 0;
	}
	else if (strcmp("set", primary) == 0)
	{
		print("Entre a set\n");
		if (strcmp("screensaver", secondary) == 0){
			if (shell_set_screen_saver_time(args) == -1){
				retval = -1;
			}else{
				print("Screen saver will set after ");
				print(args);
				print(" sec of inactivity.\n");
				retval = 0;
			}
		}
		else if (strcmp("hour", secondary) == 0)
		{
			int aux = shell_validate_time(args, 2);
			if (aux!=-1)
			{
				set_time_att(2,dtoh(aux));
				retval = 0;
			}
		}
		else if (strcmp("min", secondary) == 0)
		{
			int aux = shell_validate_time(args, 1);
			if (aux!=-1)
			{
				set_time_att(1,dtoh(aux));
				retval = 0;
			}
		}
		else if (strcmp("sec", secondary) == 0)
		{
			int aux = shell_validate_time(args, 0);
			if (aux!=-1)
			{
				set_time_att(0,dtoh(aux));
				retval = 0;
			}
		}
		else if (strcmp("time", secondary) == 0)
		{
			//*video='A';
			print("Entre a time\n");
			retval = shell_set_whole_time(args);
		}
		else
		{
			retval = -1;
		}

	}
	else
	{
		retval = -1;
	}

	flush(command);
	return retval;
}

void flush(tCommand * command){
	*(command->primary) = 0;
	*(command->secondary) = 0;
	*(command->args) = 0;
	print("Entre a flush\n");
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
	//set_time_att(type,dtoh(res));
	return res;
}

int shell_set_whole_time(char * value)
{
	//*video='A';
	//int check=0;
	int i;
	//int j=0;
	char aux[3];
	int nums[3];
	for (i=0;i<3;i++)
	{
		int j=0;
		while ((*value!=0) && (*value!=','))
		{
			if (j>=2)
			{
				return -1;
			}
			aux[j]=*value;
			value++;
			j++;
		}
		if (j<2 || (*value == 0 && i!=2) || (*value != 0 && i==2))
		{
			return -1;
		}
		aux[2] = 0;
		nums[i] = shell_validate_time(aux,2-i);
		if (nums[i]==-1)
		{
			return -1;
		}
		value++;
	}
	set_time_att(0,dtoh(nums[2]));
	set_time_att(1,dtoh(nums[1]));
	set_time_att(2,dtoh(nums[0]));

	return 0;

}
