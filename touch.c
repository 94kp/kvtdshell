#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <utime.h>

int main(int argc, char *argv[])
{
    int options;
    int verbosity = 0;
    int no_create = 0; // -c : do not create file if it doesn't exist

    while ((options = getopt(argc, argv, "vc")) != -1)
    {
        switch (options)
        {
        case 'v':
            verbosity = 1;
            break;
        case 'c':
            no_create = 1;
            break;
        default:
            fprintf(stderr, "Usage: %s [-v] [-c] file...\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (optind >= argc)
    {
        fprintf(stderr, "touch: missing operand\n");
        return EXIT_FAILURE;
    }

    int exit_status = EXIT_SUCCESS;

    for (int i = optind; i < argc; i++)
    {
        char *filename = argv[i];
        struct stat st;
        int file_exists = (stat(filename, &st) == 0);

        if (!file_exists)
        {
            if (no_create)
            {
                // -c flag: silently skip files that don't exist
                continue;
            }

            // Create the file
            int fd = open(filename, O_WRONLY | O_CREAT, 0644);
            if (fd < 0)
            {
                perror("touch: failed to create file");
                exit_status = EXIT_FAILURE;
                continue;
            }
            close(fd);

            if (verbosity)
            {
                printf("touch: created '%s'\n", filename);
            }
        }
        else
        {
            // File exists — update its access and modification timestamps
            if (utime(filename, NULL) == -1)
            {
                perror("touch: failed to update timestamp");
                exit_status = EXIT_FAILURE;
                continue;
            }

            if (verbosity)
            {
                printf("touch: updated timestamp for '%s'\n", filename);
            }
        }
    }

    return exit_status;
}