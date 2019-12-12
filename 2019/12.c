#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vec3
{
	short x, y, z;
} vec3;

typedef struct planet
{
	vec3 pos;
	vec3 vel;
} planet;

static const int NB_PLANETS = 4;

void run_step(planet *planets)
{
	for(int i = 0; i < NB_PLANETS; i++)
	{
		for(int j = i + 1; j < NB_PLANETS; j++)
		{
			if(planets[i].pos.x > planets[j].pos.x)
			{
				planets[i].vel.x--;
				planets[j].vel.x++;
			}

			if(planets[i].pos.x < planets[j].pos.x)
			{
				planets[i].vel.x++;
				planets[j].vel.x--;
			}

			if(planets[i].pos.y > planets[j].pos.y)
			{
				planets[i].vel.y--;
				planets[j].vel.y++;
			}

			if(planets[i].pos.y < planets[j].pos.y)
			{
				planets[i].vel.y++;
				planets[j].vel.y--;
			}

			if(planets[i].pos.z > planets[j].pos.z)
			{
				planets[i].vel.z--;
				planets[j].vel.z++;
			}

			if(planets[i].pos.z < planets[j].pos.z)
			{
				planets[i].vel.z++;
				planets[j].vel.z--;
			}
		}
	}

	for(int i = 0; i < NB_PLANETS; i++)
	{
		planets[i].pos.x += planets[i].vel.x;
		planets[i].pos.y += planets[i].vel.y;
		planets[i].pos.z += planets[i].vel.z;
	}
}

int get_energy(const planet *planets)
{
	int total_energy = 0;

	for(int i = 0; i < NB_PLANETS; i++)
	{
		int energy = (abs(planets[i].pos.x) + abs(planets[i].pos.y) + abs(planets[i].pos.z)) *
			(abs(planets[i].vel.x) + abs(planets[i].vel.y) + abs(planets[i].vel.z));

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
			planets[i].pos.x ,planets[i].pos.y, planets[i].pos.z,
			planets[i].vel.x ,planets[i].vel.y, planets[i].vel.z);
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
		planets[i].pos.x = atoi(buff);
		getchar();getchar();getchar();getchar();
		scanf("%[-0-9]s", buff);
		planets[i].pos.y = atoi(buff);
		getchar();getchar();getchar();getchar();
		scanf("%[-0-9]s", buff);
		planets[i].pos.z = atoi(buff);
		getchar();getchar();getchar();
	}
}

int main(void)
{
	static planet planets[NB_PLANETS];
	static planet planets_tmp[NB_PLANETS];

	read_input(planets);
	memcpy(planets_tmp, planets, sizeof planets);

	for(int step = 0; step < 1000; step++)
		run_step(planets_tmp);

	printf("Part 1: total energy = %d\n", get_energy(planets_tmp));

	memcpy(planets_tmp, planets, sizeof planets);

	print_planets(planets_tmp);

	for(long step = 0; step < 4686774924; step++)
	{
		run_step(planets_tmp);

		/*
		if(step % 10000000 == 0)
		{
			printf("Step %ld:\n", step);
			print_planets(planets_tmp);
		}
		*/
	}

	puts("end :");
	print_planets(planets_tmp);

	return EXIT_SUCCESS;
}
