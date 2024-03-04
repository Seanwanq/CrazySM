#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "mt19937.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define NDIM 3
#define N 1000

/* Initialization variables */
const int mc_steps = 300;
const int output_steps = 100;
const double packing_fraction = 0.49;
const double diameter = 1.0;
const double delta = 0.06;
const char *init_filename = "fcc.dat";

double average_acceptance = 0.0;

/* Simulation variables */
int n_particles = 0;
double radius;
double particle_volume;
double r[N][NDIM];
double box[NDIM];

/* Functions */
void read_data(void)
{
    /*--------- Your code goes here -----------*/
    // char *fileName = "C:/Users/seanw/OneDrive/Files/Master EXPH/Master Year 1/Modeling and Simulation/homework/Week2_Exercise4/fcc.dat";
    char *fileName = "C:/Users/seanw/OneDrive/Files/Master EXPH/Master Year 1/Modeling and Simulation/homework/Week2_Exercise4/coords_step0000000.dat";
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

    if (n_particles > N)
    {
        n_particles = N;
    }

    for (int d = 0; d < NDIM; d++)
    {
        double temp1, temp2;
        if (fscanf(fp, "%lf %lf\n", &temp1, &temp2) != 2)
        {
            perror("Error reading first 3 data line");
            fclose(fp);
        }
        box[d] = temp2 - temp1;
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

void write_data(int step)
{
    char buffer[300];
    sprintf(buffer, "C:/Users/seanw/OneDrive/Files/Master EXPH/Master Year 1/Modeling and Simulation/homework/Week2_Exercise4/output_data/coords_step%07d.dat", step);
    // sprintf(buffer, "coords_step%07d.dat", step);
    FILE *fp = fopen(buffer, "w");
    int d, n;
    fprintf(fp, "%d\n", n_particles);
    for (d = 0; d < NDIM; ++d)
    {
        fprintf(fp, "%lf %lf\n", 0.0, box[d]);
    }
    for (n = 0; n < n_particles; ++n)
    {
        for (d = 0; d < NDIM; ++d)
            fprintf(fp, "%f\t", r[n][d]);
        fprintf(fp, "%lf\n", diameter);
    }
    fclose(fp);
}

void set_packing_fraction(void)
{
    double volume = 1.0;
    int d, n;
    for (d = 0; d < NDIM; ++d)
        volume *= box[d];

    double target_volume = (n_particles * particle_volume) / packing_fraction;
    double scale_factor = pow(target_volume / volume, 1.0 / NDIM);

    for (n = 0; n < n_particles; ++n)
    {
        for (d = 0; d < NDIM; ++d)
            r[n][d] *= scale_factor;
    }
    for (d = 0; d < NDIM; ++d)
        box[d] *= scale_factor;
}

int main(int argc, char *argv[])
{

    assert(packing_fraction > 0.0 && packing_fraction < 1.0);
    assert(diameter > 0.0);
    assert(delta > 0.0);

    radius = 0.5 * diameter;

    if (NDIM == 3)
        particle_volume = M_PI * pow(diameter, 3.0) / 6.0;
    else if (NDIM == 2)
        particle_volume = M_PI * pow(radius, 2.0);
    else
    {
        printf("Number of dimensions NDIM = %d, not supported.", NDIM);
        return 0;
    }

    read_data();

    if (n_particles == 0)
    {
        printf("Error: Number of particles, n_particles = 0.\n");
        return 0;
    }

    set_packing_fraction();

    dsfmt_seed(time(NULL));

    int accepted = 0;
    int step, n;
    // TODO
    int temp_number = mc_steps / 100;
    for (step = 0; step < mc_steps; ++step)
    {
        for (n = 0; n < n_particles; ++n)
        {
            accepted += move_particle();
        }

        if (step % output_steps == 0)
        {
            // TODO Uncomment to execute the following line.
            printf("Step %d. Move acceptance: %lf.\n", step, (double)accepted / (n_particles * output_steps));
            average_acceptance += ((double)accepted / (n_particles * output_steps)) / temp_number;
            accepted = 0;
            // TODO Uncomment to execute the following line.
            write_data(step);
        }
    }

    // TODO
    printf("Average acceptance is %lf\n", average_acceptance);
    return 0;
}