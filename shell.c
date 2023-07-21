#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"
#include "builtin.h"
#include "path.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10


void handle_command(char *command);
void execute_command(char *command);
void parse_command(char *command, char *argv[], int *argc);

void run_shell(void)
{
	char command[MAX_COMMAND_LENGTH];

	while (1)
	{
		printf("Simple Shell> ");

		/** Read the command from the input using fgets */
		if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL)
		{
			/** Error or end of input */
			break; }

		/** Remove trailing newline character */
		if (command[strlen(command) - 1] == '\n')
		{
			command[strlen(command) - 1] = '\0'; }

		if (is_builtin_command(command))
		{
			execute_builtin_command(command); }

		else
		{
			handle_command(command); }
	}

}

void handle_command(char *command)
{
	/** Fork a child process to execute the command */
	pid_t pid = fork();

	if (pid < 0)
	{
		/** Forking failed  */
		perror("Fork failed"); }

	else if (pid == 0)
	{
		/** Child process */
		execute_command(command);
		exit(0); }
	else
	{
		/** Parent process */
		int status;

		waitpid(pid, &status, 0); }
}

void execute_command(char *command)
{
	char *argv[MAX_ARGUMENTS + 2]; /**
					* +2 to accommodate the command
					* and the NULL terminator 
					*/
	int argc;

	/** Parse the command and its arguments */
	parse_command(command, argv, &argc);

	/** Find the executable path for the command */
	char *executable_path = find_executable_path(argv[0]);

	if (executable_path != NULL)
	{
		/** Execute the command with the arguments */
		execve(executable_path, argv, NULL);
		perror(command); /**
				  * Print the error message
				  * equivalent to the program name
				  */
		free(executable_path);
		exit(1); }
	else
	{
		printf("%s: command not found\n", command); /** Command not found in PATH */
		exit(1); }
}

void parse_command(char *command, char *argv[], int *argc)
{
	/** Tokenize the command string based on whitespaces */
	char *token = strtok(command, " ");
	*argc = 0;

	while (token != NULL && *argc < MAX_ARGUMENTS)
	{
		argv[(*argc)++] = token;
		token = strtok(NULL, " "); }

	/** Add a NULL terminator at the end of the arguments list */
	argv[*argc] = NULL;
}