#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>

#define NDIM 3
#define LENGTH 15

const float amplifier = 1.0;
const double diameter = 1.0;

int N;

double (*r)[NDIM];
double box[NDIM];

char *writtenFileName = "xyz.dat";
char writtenFileNameAbsolute[200];

void ConstructCubicLattice();

void WriteData();

void GetAbsoluteFileName(char *, char *);

int main()
{
    N = (int)pow(LENGTH, NDIM);

    r = malloc(N * sizeof(*r));

    if (r == NULL)
    {
        perror("Memory allocation failed.");
        return -1;
    }

    GetAbsoluteFileName(writtenFileName, writtenFileNameAbsolute);

    ConstructCubicLattice();

    free(r);
}

void ConstructCubicLattice()
{
    int n = 0;
    if (NDIM == 2)
    {
        for (int i = 0; i < LENGTH; i++)
        {
            for (int j = 0; j < LENGTH; j++)
            {
                r[n][0] = i * amplifier;
                r[n][1] = j * amplifier;
                n++;
            }
        }
    }
    else if (NDIM == 3)
    {
        for (int i = 0; i < LENGTH; i++)
        {
            for (int j = 0; j < LENGTH; j++)
            {
                for (int k = 0; k < LENGTH; k++)
                {
                    r[n][0] = i * amplifier;
                    r[n][1] = j * amplifier;
                    r[n][2] = k * amplifier;
                    n++;
                }
            }
        }
    }

    for (int i = 0; i < NDIM; i++)
    {
        box[i] = LENGTH * amplifier;
    }

    WriteData();
}

void WriteData()
{
    FILE *fp = fopen(writtenFileNameAbsolute, "w");
    printf("File created.\n");
    fprintf(fp, "%d\n", N);
    for (int d = 0; d < NDIM; d++)
    {
        fprintf(fp, "%lf %lf\n", 0.0, box[d]);
    }
    for (int n = 0; n < N; n++)
    {
        for (int d = 0; d < NDIM; d++)
        {
            fprintf(fp, "%lf\t", r[n][d]);
        }
        fprintf(fp, "%lf\n", diameter);
    }
    fclose(fp);
}

void GetAbsoluteFileName(char *fileName, char *buffer)
{
    char directory[200];
    getcwd(directory, 200);

    char *target = "\\build";
    char *ptr = strstr(directory, target);
    if (ptr != NULL)
    {
        int index = ptr - directory;
        strncpy(buffer, directory, index);
        buffer[index] = '\0';
        strcat(buffer, "\\");
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