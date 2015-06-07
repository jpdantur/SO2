#include <shell.h>
#include <time.h>
void sys_screen_saver_set(int time);
#define isnum(x) ((x)>='0' && (x)<='9'?1:0)
char * video = (char*)0xB8000 + 79 * 2;

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

	if (strcmp("time", primary) == 0)
	{
		shell_print_time();
		return 0;
	}
	else if (strcmp("set", primary) == 0)
	{
		if (strcmp("screensaver", secondary) == 0){
			if (shell_set_screen_saver_time(args) == -1)
				return -1;
			print("Screen saver will set after ");
			print(args);
			print(" sec of inactivity.\n");
			return 0;
		}
		else if (strcmp("hour", secondary) == 0)
		{
			int aux = shell_validate_time(args, 2);
			if (aux!=-1)
			{
				set_time_att(2,dtoh(aux));
				return 0;
			}
		}
		else if (strcmp("min", secondary) == 0)
		{
			int aux = shell_validate_time(args, 1);
			if (aux!=-1)
			{
				set_time_att(1,dtoh(aux));
				return 0;
			}
		}
		else if (strcmp("sec", secondary) == 0)
		{
			int aux = shell_validate_time(args, 0);
			if (aux!=-1)
			{
				set_time_att(0,dtoh(aux));
				return 0;
			}
		}
		else if (strcmp("time", secondary) == 0)
		{
			//*video='A';
			return shell_set_whole_time(args);
		}
	}
	else
	{
		return -1;
	}
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
<<<<<<< HEAD
}
=======
}
>>>>>>> cba771229d768e53ed57deed2ccf881fa8cb9b12
