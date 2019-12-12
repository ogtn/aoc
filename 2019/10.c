#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int WIDTH = 24;
static const int HEIGHT = 24;

typedef struct asteroid
{
	char x;
	char y;
	int on_sight;
} asteroid;

int euclide(int a, int b)
{
	while(b)
	{
		int tmp = b;
		b = a % b;
		a = tmp;
	}

	return abs(a);
}

int main(void)
{
	int asteroids_map[WIDTH][HEIGHT];
	static asteroid asteroids[WIDTH * HEIGHT];
	int nb_asteroids = 0;

	memset(asteroids_map, -1, sizeof asteroids_map);

	for(int j = 0; j < HEIGHT; j++)
	{
		for(int i = 0; i < WIDTH; i++)
		{
			if(getchar() == '#')
			{
				asteroids[nb_asteroids].x = i;
				asteroids[nb_asteroids].y = j;
				asteroids_map[i][j] = nb_asteroids;
				nb_asteroids++;
			}
		}

		getchar();
		getchar();
	}

	for(int i = 0; i < nb_asteroids; i++)
	{
		for(int j = i + 1; j < nb_asteroids; j++)
		{
			int dir_x = asteroids[j].x - asteroids[i].x;
			int dir_y = asteroids[j].y - asteroids[i].y;

			int gcd = euclide(dir_x, dir_y);

			dir_x /= gcd;
			dir_y /= gcd;

			int pos_x = asteroids[i].x + dir_x;
			int pos_y = asteroids[i].y + dir_y;

			while(pos_x >= 0 && pos_x < WIDTH && pos_y >= 0 && pos_y < HEIGHT)
			{
				int current = asteroids_map[pos_x][pos_y];

				if(current != -1)
				{
					if(current == j)
					{
						asteroids[i].on_sight++;
						asteroids[j].on_sight++;
					}

					break;
				}

				pos_x += dir_x;
				pos_y += dir_y;
			}
		}
	}

	int max = 0;
	int best = -1;

	for(int i = 0; i < nb_asteroids; i++)
	{
		if(asteroids[i].on_sight > max)
		{
			max = asteroids[i].on_sight;
			best = i;
		}
	}

	printf("Best is %d with %d\n", best, max);

	return EXIT_SUCCESS; 
}
