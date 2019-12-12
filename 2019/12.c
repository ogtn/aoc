#include <stdio.h>
#include <stdlib.h>

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
}

int main(void)
{
	static planet planets[NB_PLANETS];

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
	
	for(int step = 0; step < 1000; step++)
	{
		run_step(planets);
		
		for(int i = 0; i < NB_PLANETS; i++)
		{
			planets[i].pos.x += planets[i].vel.x;
			planets[i].pos.y += planets[i].vel.y;
			planets[i].pos.z += planets[i].vel.z;
		}
	}

	int total_energy = 0;

	for(int i = 0; i < NB_PLANETS; i++)
	{
		int energy = (abs(planets[i].pos.x) + abs(planets[i].pos.y) + abs(planets[i].pos.z)) *
			(abs(planets[i].vel.x) + abs(planets[i].vel.y) + abs(planets[i].vel.z));

		total_energy += energy;

		//printf("%d %d %d: E = %d\n", planets[i].pos.x ,planets[i].pos.y, planets[i].pos.z, energy);
	}

	printf("Part 1: total energy = %d\n", total_energy);

	return EXIT_SUCCESS;
}
