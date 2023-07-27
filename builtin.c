#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtin.h"
#include <limits.h>

/**
 * execute_builtin_command - execute the built in commande
 * @command: the commend should be executed
 *
 * Return: Nothing
 */

void execute_builtin_command(char *command)
{
	char *command_copy;
	char *command_name;
	char *argument;

	command_copy = strdup(command);
	command_name = strtok(command_copy, " ");
	argument = strtok(NULL, " ");

	if (strcmp(command_name, "exit") == 0)
	{
		int exit_status = 0;

		if (argument != NULL)
		{
			char *end_ptr;
			long status;

			status = strtol(argument, &end_ptr, 10);

			if (*end_ptr == '\0' && end_ptr != argument &&
					status >= INT_MIN && status <= INT_MAX)
			{
				exit_status = (int)status; }

			else
			{
				printf("Invalid exit status: %s\n", argument);
				free(command_copy);
				return; }
		}

		free(command_copy);
		exit(exit_status);
	}

	else if (strcmp(command_name, "env") == 0)
	{
		system("env"); }
	free(command_copy);
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
