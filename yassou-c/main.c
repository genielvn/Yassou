#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
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
    bool file_found = false;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            if (file_found) {
                printf("ERROR : Only one file for compiling.\n");
                return -1;
            }
            strcpy(filename, argv[i]);
            file_found = true;
            continue;
        }
        switch (argv[i][1])
        {
            case 'd':
                if (!arguments.debugging) printf("DEBUG : Debugging enabled\n");
                arguments.debugging = true;
                break;
            
            default:
                printf("ERROR : Flag -%c is not valid.", argv[i][1]);
                return -1;
        }
    }

    if (!file_found)
    {
        printf("usage: yassou [FILE] [-d]\n");
        FILEPROVIDED;
        return -1;
    }

    if (!get_file_extension(filename, ".yass"))
    {
        printf("ERROR : File %s is not a valid YASS file.", filename);
        return -1;
    }

    FILE *file = fopen(filename, "r");
    FILEEXISTCHECK;
}


int get_file_extension(const char* filename, const char* ext)
{
    int path_idx = strlen(filename) - 1;
    int ext_idx = strlen(ext) - 1;

    // search extension from the last character
    for (; path_idx >= 0 && ext_idx >= 0; path_idx--, ext_idx--) {
        printf("%c=%c", filename[path_idx], ext[ext_idx]);
        if (tolower(filename[path_idx]) != tolower(ext[ext_idx])) {
            return false;
        }
    }
    return true;
}