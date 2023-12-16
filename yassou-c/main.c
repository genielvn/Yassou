#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include "error.h"
#include "constants.h"


char filename[];

typedef struct arguments
{
    bool debugging;
} Args;


int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("usage: yassou [FILE] [-d]");
        return -1;
    }

    Args arguments;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            strcpy(filename, argv[i]);
            continue;
        }
        switch (argv[i][1])
        {
            case 'd':
                if (!arguments.debugging) printf("DEBUG : Debugging enabled");
                arguments.debugging = true;
                break;
            
            default:
                printf("ERROR : Flag -%c is not valid.", argv[i][1]);
                return -1;
        }
    }

    if (strcmp(filename, "") == 0)
    {
        printf("usage: yassou [FILE] [-d]\n");
        FILEPROVIDED;
        return -1;
    }

    FILE *file = fopen(filename, "r");
    FILEEXISTCHECK;
}

