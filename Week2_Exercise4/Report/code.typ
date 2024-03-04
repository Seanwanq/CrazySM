#import "sourcerer.typ": *


= Code
Code snippets are shown here.

== Code snippets for @q1 <cq1>
Code snippet for generating data points.
#code(lang: "cubic_lattice.c",
```C
#define NDIM 3
#define LENGTH 15
const float amplifier = 1.0;
const double diameter = 1.0;
int N = (int)pow(LENGTH, NDIM);
// Here I use a pointer so that I can decide 
// how much space to malloc for different dimensional lattices.
double (*r)[NDIM];
r = malloc(N * sizeof(*r));
double box[NDIM];
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
}
```
)

Code snippet for saving the data points to file `cubic.dat`.
#code(lang: "cubic_lattice.c",
```C
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
```
)

== Code snippets for @q3 <cq3>
Code snippet for reallocating memory to suit the change of `N`.
#code(lang: "fcc_lattice.c",
```c
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
```
)

Code snippet for generating FCC data points.
#code(
    lang: "fcc_lattice.c",
```C
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
    while (z < LENGTH + 0.001)
    {
        while (y < LENGTH + 0.001)
        {
            while (x < LENGTH + 0.001)
            {
                r[n][0] = x * amplifier;
                r[n][1] = y * amplifier;
                r[n][2] = z * amplifier;
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
```
)

== Code snippet for @q5 <cq5>
Code snippet for reading data files.
#code(
    lang: "hs-nvt.c",
```c
void read_data(void)
{
    /*--------- Your code goes here -----------*/
    char *fileName = "coords_step0000000.dat";
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
    }

    if (fscanf(fp, "%d\n", &n_particles) != 1)
    {
        perror("Error reading number of particles");
        fclose(fp);
    }

    for (int d = 0; d < NDIM; d++)
    {
        double temp;
        if (fscanf(fp, "%lf %lf\n", &temp, &box[d]) != 2)
        {
            perror("Error reading first 3 data line");
            fclose(fp);
        }
    }

    for (int n = 0; n < n_particles; n++)
    {
        double tempDiameter;
        for (int d = 0; d < NDIM; d++)
        {
            if (fscanf(fp, "%lf\t", &r[n][d]) != 1)
            {
                perror("Error reading coordination line");
                fclose(fp);
            }
        }
        if (fscanf(fp, "%lf\n", &tempDiameter) != 1)
        {
            perror("Error reading diameter");
            fclose(fp);
        }
    }

    fclose(fp);
}
```
)

== Code snippet for @q6 <cq6>
#code(
    lang: "hs-nvt.c",
```C
int move_particle(void)
{
    /*--------- Your code goes here -----------*/
    if (NDIM == 2)
    {
        perror("Sorry, this function only support 3-D situation.\n");
    }

    double distance = 0.0, distanceX = 0.0, distanceY = 0.0, distanceZ = 0.0;
    double randNumber = dsfmt_genrand();
    double randX = dsfmt_genrand(), randY = dsfmt_genrand(), randZ = dsfmt_genrand();

    int chosenParticle = (int)(randNumber * n_particles);

    double deltaX = delta * (2 * randX - 1);
    double deltaY = delta * (2 * randY - 1);
    double deltaZ = delta * (2 * randZ - 1);

    double newX = r[chosenParticle][0] + deltaX;
    double newY = r[chosenParticle][1] + deltaY;
    double newZ = r[chosenParticle][2] + deltaZ;

    // To implement periodic boundary condition
    if (newX < 0)
        newX += box[0];
    if (newX > box[0])
        newX -= box[0];
    if (newY < 0)
        newY += box[1];
    if (newY > box[1])
        newY -= box[1];
    if (newZ < 0)
        newZ += box[2];
    if (newZ > box[2])
        newZ -= box[2];

    r[chosenParticle][0] = newX;
    r[chosenParticle][1] = newY;
    r[chosenParticle][2] = newZ;
    return 1;
}
```
)

== Code snippet for @q7 <cq7>
#code(
    lang: "hs-nvt.c",
```C
int move_particle(void)
{
    /*--------- Your code goes here -----------*/
    if (NDIM == 2)
    {
        perror("Sorry, this function only support 3-D situation.\n");
    }

    double distance = 0.0, distanceX = 0.0, distanceY = 0.0, distanceZ = 0.0;
    double randNumber = dsfmt_genrand();
    double randX = dsfmt_genrand(), randY = dsfmt_genrand(), randZ = dsfmt_genrand();

    int chosenParticle = (int)(randNumber * n_particles);

    double deltaX = delta * (2 * randX - 1);
    double deltaY = delta * (2 * randY - 1);
    double deltaZ = delta * (2 * randZ - 1);

    double newX = r[chosenParticle][0] + deltaX;
    double newY = r[chosenParticle][1] + deltaY;
    double newZ = r[chosenParticle][2] + deltaZ;

    // To implement periodic boundary condition
    if (newX < 0)
        newX += box[0];
    if (newX > box[0])
        newX -= box[0];
    if (newY < 0)
        newY += box[1];
    if (newY > box[1])
        newY -= box[1];
    if (newZ < 0)
        newZ += box[2];
    if (newZ > box[2])
        newZ -= box[2];

    // To check overlapping
    for (int i = 0; i < n_particles; i++)
    {
        if (i == chosenParticle)
            continue;

        distanceX = fabs(newX - r[i][0]);
        distanceY = fabs(newY - r[i][1]);
        distanceZ = fabs(newZ - r[i][2]);

        if (distanceX > box[0] / 2)
            distanceX = box[0] - distanceX;
        if (distanceY > box[1] / 2)
            distanceY = box[1] - distanceY;
        if (distanceZ > box[2] / 2)
            distanceZ = box[2] - distanceZ;

        distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2) + pow(distanceZ, 2));

        if (distance < diameter)
            return 0;
    }

    r[chosenParticle][0] = newX;
    r[chosenParticle][1] = newY;
    r[chosenParticle][2] = newZ;
    return 1;
}
```
)