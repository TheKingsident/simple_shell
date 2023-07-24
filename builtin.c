#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtin.h"

/**
 * execute_builtin_command - execute the built in commande
 * @command: the commend should be executed
 *
 * Return: Nothing
 */

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

/**
 * is_builtin_command - verified if the command is a builtin commande
 * @command: the command that will be tested
 *
 * Return: O or 1 in function of fail or success.
 */
int is_builtin_command(char *command)
{
	return (strcmp(command, "exit") == 0 ||
			strcmp(command, "env") == 0);

}
