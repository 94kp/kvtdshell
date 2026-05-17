#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    int options;
    int verbosity = 0;

    // 1. Parse options using getopt
    while ((options = getopt(argc, argv, "v")) != -1) 
    {
        switch (options)
        {
            case 'v':
                verbosity = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-v] directory_name\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    // Ensure a directory name was actually passed
    if (optind >= argc)
    {
        fprintf(stderr, "rmdir: missing operand (directory name required)\n");
        return EXIT_FAILURE;
    }

    char *dir_name = argv[optind];

    // 2. Execute the rmdir system call
    // Note: The standard rmdir system call will fail if the directory is not empty.
    if (rmdir(dir_name) == -1) 
    {
        perror("rmdir failed");
        return EXIT_FAILURE;
    }

    // 3. Handle verbose flag output
    if (verbosity) 
    {
        printf("rmdir: removed directory '%s'\n", dir_name);
    }

    return EXIT_SUCCESS;
}


