#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
    int options;
    int verbosity = 0;

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

    if (optind >= argc)
    {
        fprintf(stderr, "Error: missing operand - directory name required\n");
        return EXIT_FAILURE;
    }

    char *dir_name = argv[optind];

    if (rmdir(dir_name) == -1)
    {
        perror("Error:rmdir failed");
        return EXIT_FAILURE;
    }

    if (verbosity) 
    {
        printf("Error: removed directory '%s'\n", dir_name);
    }

    return EXIT_SUCCESS;
}


