#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "path.h"

#define MAX_PATH_LENGTH 200

char *find_executable_path(char *command)
{
	char *path;
	char *path_copy;
	char *dir;

	path = getenv("PATH");
	path_copy = strdup(path);
	dir = strtok(path_copy, ":");

	while (dir != NULL)
	{
		char full_path[MAX_PATH_LENGTH];

		snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", dir, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (strdup(full_path)); }

		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
