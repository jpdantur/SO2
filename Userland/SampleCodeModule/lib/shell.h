#ifndef _SHELL_H
#define _SHELL_H

#define MAX_COMMAND_LEN 	20
#define MAX_ARGS_LEN		255
#define GET_MAX_LEN(x)	(x==0?MAX_COMMAND_LEN:MAX_ARGS_LEN)

#define INVALID_COMMAND -1

typedef struct
{
	char primary[MAX_COMMAND_LEN + 1];
	char args[MAX_ARGS_LEN + 1];

} tCommand;

int shell_buffer_parser(tCommand * command, char * bff, int bff_len);
int shell_command_execute(tCommand * command);
void shell(void);

#endif