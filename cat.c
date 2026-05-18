#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

void cat_file(int fd, const char *filename, int verbosity, int number_lines)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int line_num = 1;
    int new_line = 1;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
    {
        if (number_lines)
        {
            for (ssize_t i = 0; i < bytes_read; i++)
            {
                if (new_line)
                {
                    printf("%6d\t", line_num++);
                    new_line = 0;
                }
                putchar(buffer[i]);
                if (buffer[i] == '\n')
                {
                    new_line = 1;
                }
            }
        }
        else
        {
            write(STDOUT_FILENO, buffer, bytes_read);
        }
    }

    if (bytes_read < 0)
    {
        perror("cat: read failed");
        if (verbosity)
        {
            fprintf(stderr, "cat: error reading '%s'\n", filename);
        }
    }
}

int main(int argc, char *argv[])
{
    int options;
    int verbosity = 0;
    int number_lines = 0; /* -n : number each output line */

    while ((options = getopt(argc, argv, "vn")) != -1)
    {
        switch (options)
        {
        case 'v':
            verbosity = 1;
            break;
        case 'n':
            number_lines = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-v] [-n] [file...]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    /* No files given — read from stdin (e.g. cat < input.txt) */
    if (optind >= argc)
    {
        cat_file(STDIN_FILENO, "stdin", verbosity, number_lines);
        return EXIT_SUCCESS;
    }

    int exit_status = EXIT_SUCCESS;

    for (int i = optind; i < argc; i++)
    {
        char *filename = argv[i];

        int fd = open(filename, O_RDONLY);
        if (fd < 0)
        {
            perror("cat: failed to open file");
            exit_status = EXIT_FAILURE;
            continue;
        }

        if (verbosity)
        {
            printf("cat: reading '%s'\n", filename);
        }

        cat_file(fd, filename, verbosity, number_lines);
        close(fd);
    }

    return exit_status;
}