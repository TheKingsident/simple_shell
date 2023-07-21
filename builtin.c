#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtin.h"

void execute_builtin_command(char *command)
{
	if (strcmp(command, "exit") == 0)
	{
		printf("Exiting shell\n");
		exit(0); }
	
	else if (strcmp(command, "env") == 0)
	{
		system("env"); }

}

int is_builtin_command(char *command)
{
	return (strcmp(command, "exit") == 0 ||
			strcmp(command, "env") == 0);

}
