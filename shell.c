#include <stdio.h>
#include <string.h>

#define MAXSIZE 99

int main (int argc, char* argv[])
{
	// function signatures
	void list_files(int, char*);

	char selector = 'c';
	char u_input[MAXSIZE] = "sample";

	char prompt[MAXSIZE] = "> ";

	// implement tokenizer here`
	
	while(selector != EOF)
	{
		printf("%s", prompt);
		scanf("%s", u_input);
		
		printf("\n");

		if (strcmp(u_input,"ls") == 0)
		{
			// perform ls command
			printf("\n");

			list_files(int argc, char* argv[]);
			printf("Performing ls operation\n");
		}
		else if (strcmp(u_input,"cd") == 0)
		{
			// perform cd command
			
			printf("\n");
			printf("Performing cd\n");
		}
		else if (strcmp(u_input,"mkdir") == 0)
		{
			// perform mkdir
			//
			printf("\n");
			printf("Performing mkdir\n");
		}
		else if (strcmp(u_input,"rmdir") == 0)
		{
			// perform rmdir
			printf("\n");
			printf("Performing rmdir\n");
		}
		else if (strcmp(u_input, "touch") == 0)
		{
			// perform touch operation or create file
			printf("\n");
			printf("Performing touch\n");
		}
		else
		{
			printf("\n");
			printf("Invalid command\n");
		}
	}	
}

void list_files(int argc, char* argv[])
{
	if (argc == 0)
	{
		
	}
}
