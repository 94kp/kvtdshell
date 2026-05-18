#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    (void)argc;

    char helper_dir[1024] = ".";
    char mkdir_path[1200];
    char rmdir_path[1200];

    /* shell.c executes ops using its absolute path.  Use that path to find
       helper binaries even after cd changes the current working directory. */
    char *last_slash = strrchr(argv[0], '/');
    if (last_slash != NULL)
    {
        size_t dir_len = last_slash - argv[0];
        if (dir_len >= sizeof(helper_dir))
        {
            dir_len = sizeof(helper_dir) - 1;
        }
        strncpy(helper_dir, argv[0], dir_len);
        helper_dir[dir_len] = '\0';
    }

    snprintf(mkdir_path, sizeof(mkdir_path), "%s/mkdir", helper_dir);
    snprintf(rmdir_path, sizeof(rmdir_path), "%s/rmdir", helper_dir);

    char **args = &argv[1];

    if (args[0] == NULL)
    {
        return 0;
    }

    for (int i = 0; args[i] != NULL; i++) 
    {
        int append_mode = 0;

        if (strcmp(args[i], ">") == 0)
        {
            append_mode = 0;
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            append_mode = 1;
        }
        else
        {
            continue;
        }

        if (args[i + 1] == NULL)
        {
            fprintf(stderr, "Syntax error: Redirection target not found\n");
            exit(EXIT_FAILURE);
        }

        char *filename = args[i + 1];

        int flags = O_WRONLY | O_CREAT;

        if (append_mode)
        {
            flags |= O_APPEND;
        }
        else
        {
            flags |= O_TRUNC;
        }

        int fd = open(filename, flags, 0644);
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

    if (strcmp(args[0], "mkdir") == 0) 
    {
        args[0] = mkdir_path;
    }

    else if (strcmp(args[0], "rmdir") == 0) 
    {
        args[0] = rmdir_path;
    }

    /* Do not rewrite ls to ./ls because this project does not build a local
       ls binary. Leaving it as "ls" lets execvp find the system ls. */

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

