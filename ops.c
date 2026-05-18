#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char **args = &argv[1];

    if (args[0] == NULL)
    {
        return 0;
    }

    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Syntax error: Redirection target not found\n");
                exit(EXIT_FAILURE);
            }

            char *filename = args[i + 1];

            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("Error: Output file could not be opened");
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("Error: Redirection failed");
                exit(EXIT_FAILURE);
            }

            close(fd);

            args[i] = NULL;
            break;
        }

        if (strcmp(args[i], "<") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "Syntax error: Redirection source not found\n");
                exit(EXIT_FAILURE);
            }

            char *filename = args[i + 1];

            int fd = open(filename, O_RDONLY);
            if (fd < 0)
            {
                perror("Error: Input file could not be opened");
                exit(EXIT_FAILURE);
            }

            if (dup2(fd, STDIN_FILENO) < 0)
            {
                perror("Error: Redirection failed");
                exit(EXIT_FAILURE);
            }

            close(fd);
            args[i] = NULL;
            break;
        }
    }

    if (strcmp(args[0], "mkdir") == 0)
    {
        args[0] = "./mkdir";
    }

    else if (strcmp(args[0], "rmdir") == 0)
    {
        args[0] = "./rmdir";
    }

    else if (strcmp(args[0], "ls") == 0)
    {
        args[0] = "./ls";
    }

    else if (strcmp(args[0], "touch") == 0)
    {
        args[0] = "./touch";
    }

    else if (strcmp(args[0], "cat") == 0)
    {
        args[0] = "./cat";
    }

    // add more commands here as and when they are implemented

    // else if (strcmp(args[0], "rmdir") == 0)
    // {
    //     args[0] = "./rmdir";
    // }

    if (execvp(args[0], args) == -1)
    {
        perror("Error: Command execution failed");
    }

    return 0;
}
