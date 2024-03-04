#include "absolute-path.h"
// #include <unistd.h>
#include <string.h>

void GetAbsoluteFileName(char *fileName, char *buffer)
{
    char directory[200];
    getcwd(directory, 200);

    char *target = "build";
    char *ptr = strstr(directory, target);
    if (ptr != NULL)
    {
        int index = ptr - directory;
        strncpy(buffer, directory, index);
        buffer[index] = '\0';
        strcat(buffer, fileName);
    }
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        if (buffer[i] == '\\')
        {
            buffer[i] = '/';
        }
    }
}