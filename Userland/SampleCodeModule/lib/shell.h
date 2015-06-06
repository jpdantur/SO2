#ifndef _SHELL_H
#define _SHELL_H

#define MAX_PARAMS_LEN 	20

typedef struct
{
	char primary[MAX_PARAMS_LEN + 1];
	char secondary[MAX_PARAMS_LEN + 1];
	char args[MAX_PARAMS_LEN + 1];

} tCommand;

int shell_buffer_parser(tCommand * command, char * bff, int bff_len);

#endif