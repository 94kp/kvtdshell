#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void create_parent_dirs(char *path) 
{
    char temp[256];
    char *p = NULL;
    size_t len;

    // printf("test print");

    snprintf(temp, sizeof(temp), "%s", path);
    len = strlen(temp);
    
    // strip trailing slashes
    if (temp[len - 1] == '/') 
    {
        temp[len - 1] = '\0';
    }

    for (p = temp + 1; *p; p++)
    {
        if (*p == '/') 
        {
            *p = '\0'; // temporarily set this as end of the string
            
            mkdir(temp, 0777);
            
            *p = '/';
        }
    }
    // Create the final target directory
    mkdir(temp, 0777);
}

int main(int argc, char *argv[]) 
{
    int options;
    int verbosity = 0;
    int p_flag = 0;

    while ((options = getopt(argc, argv, "vp")) != -1) 
    {
        switch (options) 
        {
            case 'v':
                verbosity = 1;
                break;
            case 'p':
                p_flag = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-v] [-p] directory_name\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (optind >= argc) 
    {
        fprintf(stderr, "mkdir: missing operand\n");
        return EXIT_FAILURE;
    }

    char *dir_name = argv[optind];

    if (p_flag)
    {
        create_parent_dirs(dir_name);
        if (verbosity) 
        {
            printf("Error: created directory structure for '%s'\n", dir_name);
        }
    } 
    else 
    {
        if (mkdir(dir_name, 0777) == -1) 
        {
            perror("Error: mkdir failed");
            return EXIT_FAILURE;
        }
        if (verbosity) 
        {
            printf("mkdir: created directory '%s'\n", dir_name);
        }
    }

    return EXIT_SUCCESS;
}
