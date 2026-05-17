#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>    // For getopt
#include <sys/stat.h>   // For the actual mkdir() system call
#include <sys/types.h>

int main(int argc, char* argv[])
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

	if (optind >= argc) // get next index of array
	{
        fprintf(stderr, "mkdir: missing operand (directory name required)\n");
        return EXIT_FAILURE;
    }

    char *dir_name = argv[optind];

	if (mkdir(dir_name, 0777) == -1) 
	{
        perror("mkdir failed");
        return EXIT_FAILURE;
    }

	if (verbosity) 
	{
        printf("mkdir: created directory '%s'\n", dir_name);
    }

	return EXIT_SUCCESS;
}
