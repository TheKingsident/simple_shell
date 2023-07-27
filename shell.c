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


void handle_command(char *command, char *program_name);
void execute_command(char *command, char *program_name);
void parse_command(char *command, char *argv[], int *argc);
char *get_program_name(char *arg);

/**
 * run_shell - run the shell
 * @program_name: name of the programe
 *
 * Return: Nothing.
 */
void run_shell(char *program_name)
{
	char command[MAX_COMMAND_LENGTH];
	FILE *input_stream;
	int interactive_mode;

	interactive_mode = isatty(fileno(stdin));

	if (interactive_mode)
		input_stream = stdin;
	else
	{
		input_stream = freopen(NULL, "r", stdin);
	}

	while (1)
	{
		if (interactive_mode)
			printf("Simple Shell> ");

		/** Read the command from the input using fgets */
		if (fgets(command, MAX_COMMAND_LENGTH, input_stream) == NULL)
		{
			/** Error or end of input */
			break; }

		/** Remove trailing newline character */
		if (command[strlen(command) - 1] == '\n')
		{
			command[strlen(command) - 1] = '\0'; }

		if (is_builtin_command(command))
		{
			execute_builtin_command(command);
			continue;
		}

		else
		{
			handle_command(command, program_name); }

	}

	if (!interactive_mode)
	{
		fclose(input_stream); }

}


/**
 * handle_command - Handle the command
 * @command: THE COMAND
 * @program_name: the name of the programe
 *
 * Return: Nothing
 */
void handle_command(char *command, char *program_name)
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
		execute_command(command, program_name);
		exit(0); }
	else
	{
		/** Parent process */
		int status;

		waitpid(pid, &status, 0); }
}


/**
 * execute_command - execute the command
 * @command: the command
 * @program_name: the name th programme
 *
 * Return: Nothing.
 */
void execute_command(char *command, char *program_name)
{
	char *argv[MAX_ARGUMENTS + 2]; /**
					* +2 to accommodate the command
					* and the NULL terminator
					*/
	int argc;
	char *executable_path;

	/** Parse the command and its arguments */
	parse_command(command, argv, &argc);

	program_name = get_program_name(program_name);

	if (strchr(command, '/') != NULL)
	{
		if (access(command, X_OK) == 0)
		{
			execve(command, argv, NULL);
			fprintf(stderr, "%s: execution failed\n", argv[0]);

			exit(1); }
		else
		{
			printf("%s: No such file or directory\n", argv[0]);
			exit(1); }
	}

	/** Find the executable path for the command */
	executable_path = find_executable_path(argv[0]);

	if (executable_path != NULL)
	{
		if (execve(executable_path, argv, NULL) == -1)
		{
			perror(program_name);
			free(executable_path);
			exit(1); }
	}

	else
	{
		fprintf(stderr, "%s: %s: not found\n", program_name, command);
		free(executable_path);
		exit(1); }

		/** Execute the command with the arguments */
		/**
		 *execve(executable_path, argv, NULL);
		 *fprintf(stderr, "%s: %s: not found\n", program_name, command);
		 *
		 *free(executable_path);
		 *exit(1); }
	* else
	*{
		*fprintf(stderr, "%s: %d: %s: not found\n", program_name,
fileno(stderr), command); * Command not found in PATH
		//free(executable_path);
		//exit(1); }
		*/
}

/**
 * get_program_name - read the command from
 * @arg: argument
 *
 * Return: the name of the programe
 */
char *get_program_name(char *arg)
{


	char *program_name;
	char *last_slash;

	program_name = arg;
	last_slash = strrchr(arg, '/');

	if (last_slash != NULL)
	{
		program_name = last_slash + 1; }

	return (program_name);
}


/**
 * parse_command - tokenize the command
 * @command: the command
 * @argv: array of variable arguments
 * @argc: lenght of argv
 *
 *Return: Nothing.
 */
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
