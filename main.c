#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

char* get_executable_path(void)
{
	ssize_t len;
	char *path;
	path = malloc(PATH_MAX);

	if (path == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");

		return (NULL); }


	len = readlink("/proc/self/exe", path, PATH_MAX - 1);

	if (len != -1)
	{
		path[len] = '\0';
		return (path); }

	else
	{
		free(path);
		perror("readlink");
		return (NULL); }

}

/**
 * main - run our shell
 *
 *Return: 0.
 */

int main(void)
{
	char* program_name;
	
	program_name = get_executable_path();

	if (program_name == NULL)
	{
		return (1); }

	run_shell(program_name);
	free(program_name);
	return (0);
}
