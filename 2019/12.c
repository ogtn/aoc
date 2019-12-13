#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct planet
{
	int pos[3];
	int vel[3];
} planet;

static const int NB_PLANETS = 4;

void run_step(planet *planets, int *use_coordinates)
{
	for(int i = 0; i < NB_PLANETS; i++)
	{
		for(int j = i + 1; j < NB_PLANETS; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				if(use_coordinates[k])
				{
					if(planets[i].pos[k] > planets[j].pos[k])
					{
						planets[i].vel[k]--;
						planets[j].vel[k]++;
					}

					if(planets[i].pos[k] < planets[j].pos[k])
					{
						planets[i].vel[k]++;
						planets[j].vel[k]--;
					}
				}
			}	
		}
	}

	for(int i = 0; i < NB_PLANETS; i++)
	{
		for(int k = 0; k < 3; k++)
			if(use_coordinates[k])
				planets[i].pos[k] += planets[i].vel[k];
	}
}

int get_energy(const planet *planets)
{
	int total_energy = 0;

	for(int i = 0; i < NB_PLANETS; i++)
	{
		int energy = (abs(planets[i].pos[0]) + abs(planets[i].pos[1]) + abs(planets[i].pos[2])) *
			(abs(planets[i].vel[0]) + abs(planets[i].vel[1]) + abs(planets[i].vel[2]));
	
		total_energy += energy;
	}

	return total_energy;
}

void print_planets(const planet *planets)
{
	puts("==============");

	for(int i = 0; i < NB_PLANETS; i++)
	{
		printf("pos: %d %d %d - vel: %d %d %d\n", 
			planets[i].pos[0] ,planets[i].pos[1], planets[i].pos[2],
			planets[i].vel[0] ,planets[i].vel[1], planets[i].vel[2]);
	}

	puts("==============");
}

void read_input(planet *planets)
{
	for(int i = 0; i < NB_PLANETS; i++)
	{
		char buff[8];

		getchar();getchar();getchar();
		scanf("%[-0-9]s", buff);
		planets[i].pos[0] = atoi(buff);
		getchar();getchar();getchar();getchar();
		scanf("%[-0-9]s", buff);
		planets[i].pos[1] = atoi(buff);
		getchar();getchar();getchar();getchar();
		scanf("%[-0-9]s", buff);
		planets[i].pos[2] = atoi(buff);
		getchar();getchar();getchar();
	}
}

int main(void)
{
	static planet planets[NB_PLANETS];
	static planet planets_tmp[NB_PLANETS];

	read_input(planets);
	memcpy(planets_tmp, planets, sizeof planets);
	int use_coordinates[3] = {1, 1, 1};

	for(int step = 0; step < 1000; step++)
		run_step(planets_tmp, use_coordinates);

	printf("Part 1: total energy = %d\n", get_energy(planets_tmp));

	memcpy(planets_tmp, planets, sizeof planets);
	print_planets(planets_tmp);
	int steps[3] = { 0 };

	do
	{
		run_step(planets_tmp, use_coordinates);

		for(int i = 0; i < 3; i++)
		{
			if(use_coordinates[i])
				steps[i]++;

			int same = 1;

			for(int j = 0; j < NB_PLANETS; j++)
				same &= planets_tmp[j].pos[i] == planets[j].pos[i] && planets_tmp[j].vel[i] == planets[j].vel[i];

			if(same)
				use_coordinates[i] = 0;
		}
	}
	while(use_coordinates[0] || use_coordinates[1] || use_coordinates[2]);

	long period = (long)steps[0] * steps[1] * steps[2];
	printf("Periods: %d %d %d\nPeriod: %ld\n", steps[0], steps[1], steps[2], period);

	return EXIT_SUCCESS;
}
