#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>

#define NDIM 3
// #define LENGTH diameter *sqrt(2) * 5
#define LENGTH 5

const float amplifier = 1.0;
const double diameter = 1.0;

int N;

double (*r)[NDIM];
double box[NDIM];

char *writtenFileName = "cubic.dat";
char writtenFileNameAbsolute[300];

void ConstructCubicLattice();

void ConstructFCCLattice();

void WriteData();

void GetAbsoluteFileName(char *, char *);

void ReallocForFCC();

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

    // ConstructFCCLattice();

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

void ConstructFCCLattice()
{
    ReallocForFCC();
    if (NDIM == 2)
    {
        perror("Sorry, we only provide 3 dimensional method.");
    }
    int n = 0;
    double x = 0.0, y = 0.0, z = 0.0;
    int yNumber = 1, zNumber = 1;
    while (z < LENGTH - 0.5 + 0.001)
    {
        while (y < LENGTH - 0.5 + 0.001)
        {
            while (x < LENGTH - 0.5 + 0.001)
            {
                r[n][0] = x * amplifier;
                r[n][1] = y * amplifier;
                r[n][2] = z * amplifier;

                // printf("Number %d: %lf   %lf   %lf\n", n, r[n][0], r[n][1], r[n][2]);

                x += diameter * sqrt(2);
                n += 1;
            }

            y += diameter * sqrt(2) / 2;
            yNumber += 1;
            if (!(((yNumber % 2 == 0) && !(zNumber % 2 == 0)) || (!(yNumber % 2 == 0) && (zNumber % 2 == 0))))
            {
                x = 0;
            }
            else
            {
                x = diameter * sqrt(2) / 2;
            }
        }

        z += diameter * sqrt(2) / 2;
        zNumber += 1;
        yNumber = 1;
        y = 0;
        if (!(((yNumber % 2 == 0) && !(zNumber % 2 == 0)) || (!(yNumber % 2 == 0) && (zNumber % 2 == 0))))
        {
            x = 0;
        }
        else
        {
            x = diameter * sqrt(2) / 2;
        }
    }

    N = n;

    for (int i = 0; i < NDIM; i++)
    {
        box[i] = LENGTH * amplifier;
    }

    WriteData();
}

void WriteData()
{
    // FILE *fp = fopen(writtenFileNameAbsolute, "w");
    FILE *fp = fopen(writtenFileName, "w");
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
        // TODO
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

void ReallocForFCC()
{
    if (NDIM == 2)
    {
        perror("Sorry, we only provide 3 dimensional method.");
    }
    int n = 0;
    double x = 0.0, y = 0.0, z = 0.0;
    int yNumber = 1, zNumber = 1;
    while (z < LENGTH + 0.001)
    {
        while (y < LENGTH + 0.001)
        {
            while (x < LENGTH + 0.001)
            {
                x += diameter * sqrt(2);
                n += 1;
            }

            y += diameter * sqrt(2) / 2;
            yNumber += 1;
            if (!(((yNumber % 2 == 0) && !(zNumber % 2 == 0)) || (!(yNumber % 2 == 0) && (zNumber % 2 == 0))))
            {
                x = 0;
            }
            else
            {
                x = diameter * sqrt(2) / 2;
            }
        }

        z += diameter * sqrt(2) / 2;
        zNumber += 1;
        yNumber = 1;
        y = 0;
        if (!(((yNumber % 2 == 0) && !(zNumber % 2 == 0)) || (!(yNumber % 2 == 0) && (zNumber % 2 == 0))))
        {
            x = 0;
        }
        else
        {
            x = diameter * sqrt(2) / 2;
        }
    }

    N = n;

    double(*temp)[NDIM] = realloc(r, N * sizeof(*r));
    if (temp == NULL)
    {
        free(r);
        perror("Memory reallocation failed.");
    }
    r = temp;
}