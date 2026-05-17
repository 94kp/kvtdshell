#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h> 

#define MAXSIZE 99
#define MAXARGS 16

int main (int argc, char* argv[])
{
	// function signatures
	//void list_files(int, char*);
	void tokenizer(char *input, char **args);

	char selector = 'c';
	char u_input[MAXSIZE] = "sample";
	char *args[MAXARGS];

	char prompt[MAXSIZE] = "> ";

	// implement tokenizer here`
	
	while(selector != EOF)
	{
		printf("%s", prompt);
		fflush(stdout);
		// scanf("%s", u_input); // not the best option to use here

		if (fgets(u_input, sizeof(u_input), stdin) == NULL) 
		{
            printf("\n");
            break;
        }
		
		// printf("\n");

		// run tokenizer here to figure out what command has been entered.

		// int cmd_num = tokenizer(u_input);
		tokenizer(u_input, args);

		if (args[0] == NULL) 
		{
            continue;
        }

		if (strcmp(args[0], "exit") == 0) 
		{
            break;
        }

		if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL) {
                fprintf(stderr, "cd: missing arguments, require path\n");
            } else if (chdir(args[1]) != 0) {
                perror("Operation Failed");
            }
            continue;
        }

		pid_t pid = fork();

		if (pid < 0) {
            perror("fork() failed");
        } 
        else if (pid == 0) 
        {

            if (strcmp(args[0], "ls") == 0) {
                args[0] = "./ls";
            } 
            else if (strcmp(args[0], "mkdir") == 0) {
                args[0] = "./mkdir";
            }
            else if (strcmp(args[0], "rmdir") == 0) {
                args[0] = "./rmdir";
            }
            else if (strcmp(args[0], "touch") == 0) {
                args[0] = "./touch";
            }

            if (execvp(args[0], args) == -1) {
                perror("Invalid command");
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

	// 	if (strcmp(u_input,"ls") == 0)
	// 	{
	// 		// perform ls command
	// 		printf("\n");

	// 		//list_files(int argc, char* argv[]);
	// 		printf("Performing ls operation\n");
	// 	}
	// 	else if (strcmp(u_input,"cd") == 0)
	// 	{
	// 		// perform cd command
			
	// 		printf("\n");
	// 		printf("Performing cd\n");
	// 	}
	// 	else if (strcmp(u_input,"mkdir") == 0)
	// 	{
	// 		// perform mkdir
	// 		//
	// 		printf("\n");
	// 		printf("Performing mkdir\n");
	// 	}
	// 	else if (strcmp(u_input,"rmdir") == 0)
	// 	{
	// 		// perform rmdir
	// 		printf("\n");
	// 		printf("Performing rmdir\n");
	// 	}
	// 	else if (strcmp(u_input, "touch") == 0)
	// 	{
	// 		// perform touch operation or create file
	// 		printf("\n");
	// 		printf("Performing touch\n");
	// 	}
	// 	else
	// 	{
	// 		printf("\n");
	// 		printf("Invalid command\n");
	// 	}
	// }	
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

