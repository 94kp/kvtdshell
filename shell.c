#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>

#define MAXSIZE 99
#define MAXARGS 16

void tokenizer(char *input, char **args);
void handle_cd(char **args);

static char prev_dir[PATH_MAX] = "";
static char shell_start_dir[PATH_MAX] = "";

int main ()
{
	char selector = 'c';
	char u_input[MAXSIZE] = "sample";
	char *args[MAXARGS];
	char prompt[MAXSIZE] = "> ";

	if (getcwd(shell_start_dir, sizeof(shell_start_dir)) == NULL)
	{
		perror("getcwd");
		return EXIT_FAILURE;
	}
	
	while(selector != EOF)
	{
		printf("%s", prompt);
		fflush(stdout);

		if (fgets(u_input, sizeof(u_input), stdin) == NULL)
		{
			printf("\n");
			break;
		}

		tokenizer(u_input, args);

		if (args[0] == NULL)
		{
			continue;
		}

		if (strcmp(args[0], "exit") == 0)
		{
			break;
		}

		/* cd must be handled as a built-in command because it changes
		   the current directory of the shell process itself. */
		if (strcmp(args[0], "cd") == 0)
		{
			handle_cd(args);
			continue;
		}

		pid_t pid = fork();

		if (pid < 0)
		{
			perror("fork() failed");
		}
		else if (pid == 0)
		{
			for (int i = MAXARGS - 1; i > 0; i--)
			{
				args[i] = args[i - 1];
			}

			char ops_path[PATH_MAX];
			if (strlen(shell_start_dir) + strlen("/ops") >= sizeof(ops_path))
			{
				fprintf(stderr, "Error: shell path is too long\n");
				exit(EXIT_FAILURE);
			}
			strcpy(ops_path, shell_start_dir);
			strcat(ops_path, "/ops");
			args[0] = ops_path;

			if (execvp(args[0], args) == -1)
			{
				perror("failed to initialise parser for operators");
			}
			exit(EXIT_FAILURE);
		}
		else
		{
			int status;
			waitpid(pid, &status, 0);
		}
	}

	return 0;
}

void handle_cd(char **args)
{
	char current_dir[PATH_MAX];
	char target_dir[PATH_MAX];
	char *target = NULL;

	if (args[2] != NULL)
	{
		fprintf(stderr, "cd: too many arguments\n");
		return;
	}

	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("cd");
		return;
	}

	if (args[1] == NULL || strcmp(args[1], "~") == 0)
	{
		target = getenv("HOME");
		if (target == NULL)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return;
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		if (prev_dir[0] == '\0')
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return;
		}
		target = prev_dir;
	}
	else if (strncmp(args[1], "~/", 2) == 0)
	{
		char *home = getenv("HOME");
		if (home == NULL)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return;
		}
		snprintf(target_dir, sizeof(target_dir), "%s/%s", home, args[1] + 2);
		target = target_dir;
	}
	else
	{
		target = args[1];
	}

	if (chdir(target) != 0)
	{
		perror("cd");
		return;
	}

	strncpy(prev_dir, current_dir, sizeof(prev_dir) - 1);
	prev_dir[sizeof(prev_dir) - 1] = '\0';
}

void tokenizer(char *input, char **args)
{
	int i = 0;
	char *token = strtok(input, " \t\n\r");
	
	while (token != NULL && i < MAXARGS - 1)
	{
		args[i++] = token;
		token = strtok(NULL, " \t\n\r");
	}
	args[i] = NULL;
}