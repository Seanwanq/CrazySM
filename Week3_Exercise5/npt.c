#include <stdio.h>
#include <time.h>
#include <math.h>
#include "mt19937.h"
#include "absolute-path.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define NDIM 3
#define N 1000

/* Initialization variables */
const int mc_steps = 100000;
const int output_steps = 100;
// the original is 0.6
const double packing_fraction = 0.4;
const double diameter = 1.0;
const double delta = 0.1;
/* Volume change -deltaV, delta V */
const double deltaV = 2.0;
/* Reduced pressure \beta P */
const double betaP = 10.0;
const char *init_filename = "fcc.dat";

/* Simulation variables */
int n_particles = 0;
double radius;
double particle_volume;
double r[N][NDIM];
double box[NDIM];

/* Functions */
int change_volume(void)
{
    /*--------- Your code goes here -----------*/
    double rNew[N][NDIM];
    double boxNew[NDIM];
    double lengthNew;
    double distance, distanceX, distanceY, distanceZ;
    double acceptmentRate;
    double minimum;
    double volumeOld = box[0] * box[1] * box[2];
    double volumeNew = volumeOld + (dsfmt_genrand() * 2 - 1) * deltaV;

    if (volumeNew <= 0)
    {
        return 0;
    }

    lengthNew = pow(volumeNew, 1.0 / 3.0);
    for (int i = 0; i < NDIM; i++)
    {
        boxNew[i] = lengthNew;
    }

    // rescale the atoms
    for (int i = 0; i < n_particles; i++)
    {
        for (int j = 0; j < NDIM; j++)
        {
            rNew[i][j] = r[i][j] * boxNew[j] / box[j];
        }
    }

    for (int i = 0; i < n_particles; i++)
    {
        for (int j = i + 1; j < n_particles; j++)
        {
            distanceX = fabs(rNew[j][0] - rNew[i][0]);
            distanceY = fabs(rNew[j][1] - rNew[i][1]);
            distanceZ = fabs(rNew[j][2] - rNew[i][2]);
            if (distanceX > boxNew[0] / 2)
                distanceX = boxNew[0] - distanceX;
            if (distanceY > boxNew[1] / 2)
                distanceY = boxNew[1] - distanceY;
            if (distanceZ > boxNew[2] / 2)
                distanceZ = boxNew[2] - distanceZ;

            distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2) + pow(distanceZ, 2));

            if (distance < diameter)
                return 0;
        }
    }

    acceptmentRate = exp(-betaP * (volumeNew - volumeOld) + n_particles * log(volumeNew / volumeOld));
    minimum = min(1.0, acceptmentRate);

    if (dsfmt_genrand() < minimum)
    {
        for (int i = 0; i < NDIM; i++)
        {
            box[i] = boxNew[i];
        }
        for (int i = 0; i < n_particles; i++)
        {
            for (int j = 0; j < NDIM; j++)
            {
                r[i][j] = rNew[i][j];
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

void read_data(void)
{
    /*--------- Your code goes here -----------*/
    // char *fileName = "coords_step0000000.dat";
    char absolutePath[200];
    GetAbsoluteFileName(init_filename, absolutePath);
    FILE *fp = fopen(absolutePath, "r");
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

    double distance, distanceX, distanceY, distanceZ;
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
    char buffer[128];
    sprintf(buffer, "output/coords_step%07d.dat", step);
    char absolutePath[300];
    GetAbsoluteFileName(buffer, absolutePath);
    FILE *fp = fopen(absolutePath, "w");
    int d, n;
    fprintf(fp, "%d\n", n_particles);
    for (d = 0; d < NDIM; ++d)
    {
        fprintf(fp, "%lf %lf\n", 0.0, box[d]);
    }
    for (n = 0; n < n_particles; ++n)
    {
        for (d = 0; d < NDIM; ++d)
            fprintf(fp, "%lf\t", r[n][d]);
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

    char *writtenCSV = "CrazyCSV.csv";
    char absoluteWrittenCSV[200];
    GetAbsoluteFileName(writtenCSV, absoluteWrittenCSV);
    FILE *csvFile = fopen(absoluteWrittenCSV, "w");
    fprintf(csvFile, "step\tvolume\taverage_movement_acceptance\taverage_volume_acceptance\n");

    if (n_particles == 0)
    {
        printf("Error: Number of particles, n_particles = 0.\n");
        return 0;
    }

    set_packing_fraction();

    dsfmt_seed(time(NULL));

    printf("#Step \t Volume \t Move-acceptance\t Volume-acceptance \n");

    int move_accepted = 0;
    int vol_accepted = 0;
    int step, n;
    double totalMovementAcceptance = 0.0;
    double totalVolumeAcceptance = 0.0;
    double totalVolume = 0;
    double groupVolume = 0;
    double volume = 0;
    double averageVolume = 0;

    for (step = 0; step < mc_steps; ++step)
    {
        for (n = 0; n < n_particles; ++n)
        {
            move_accepted += move_particle();
        }
        vol_accepted += change_volume();

        volume = box[0] * box[1] * box[2];
        totalVolume += volume;
        groupVolume += volume;

        if (step % output_steps == 0)
        {
            printf("%d \t %lf \t %lf \t %lf \n",
                   step, box[0] * box[1] * box[2],
                   (double)move_accepted / (n_particles * output_steps),
                   (double)vol_accepted / output_steps);
            totalMovementAcceptance += (double)move_accepted / (n_particles * output_steps);
            totalVolumeAcceptance += (double)vol_accepted / output_steps;

            fprintf(csvFile, "%d\t%lf\t%lf\t%lf\n", step, groupVolume / output_steps, (double)move_accepted / (n_particles * output_steps), (double)vol_accepted / output_steps);

            move_accepted = 0;
            vol_accepted = 0;
            write_data(step);
            groupVolume = 0;
        }
    }
    fclose(csvFile);

    averageVolume = totalVolume / mc_steps;
    printf("Average Volume = %lf,\npacking fraction = %lf,\nTotal movement acceptance = %lf,\nTotal volume acceptance = %lf.\n", averageVolume, (n_particles * (4.0 / 3.0) * M_PI * pow(diameter / 2, 3)) / averageVolume, totalMovementAcceptance / (mc_steps / output_steps), totalVolumeAcceptance / (mc_steps / output_steps));

    return 0;
}