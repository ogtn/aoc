#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static const int MAX_SIZE = 32768;
static const int ORIGIN = MAX_SIZE / 2;

static char map[MAX_SIZE / 4][MAX_SIZE];

int main(void)
{
	char buff[8];
	char direction;
	int pos_x = ORIGIN;
	int pos_y = ORIGIN;

	do
	{
		scanf("%c%[0123456789]s", &direction, buff);
		int len = atoi(buff);

		for(int i = 0; i < len; i++)
		{
			pos_x += (direction == 'R') - (direction == 'L');
			pos_y += (direction == 'U') - (direction == 'D');

			if(pos_x < 0 || pos_x >= MAX_SIZE || pos_y < 0 || pos_y >= MAX_SIZE)
			{
				printf("Coordinates of cable 1 out of range : [%d,%d]\n", pos_x, pos_y);

				return -1;
			}

			char mask = 1 << ((pos_x % 4) * 2);
			map[pos_x / 4][pos_y] |= mask;
		}
	} while((direction = getchar()) == ',');

	pos_x = ORIGIN;
	pos_y = ORIGIN;
	getchar();

	int best_dst = MAX_SIZE * MAX_SIZE;

	do
	{
		scanf("%c%[0123456789]s", &direction, buff);
		int len = atoi(buff);

		for(int i = 0; i < len; i++)
		{
			pos_x += (direction == 'R') - (direction == 'L');
			pos_y += (direction == 'U') - (direction == 'D');

			if(pos_x < 0 || pos_x >= MAX_SIZE || pos_y < 0 || pos_y >= MAX_SIZE)
			{
				printf("Coordinates of cable 2 out of range : [%d,%d]\n", pos_x, pos_y);

				return -1;
			}

			char mask = 1 << ((pos_x % 4) * 2);

			if(map[pos_x / 4][pos_y] & mask)
			{
				int dst = abs((ORIGIN - abs(pos_x))) + abs((ORIGIN - abs(pos_y)));

				if(dst < best_dst)
					best_dst = dst;

				mask <<= 1;
				map[pos_x / 4][pos_y] |= mask;
			}
		}
	} while((direction = getchar()) == ',');

	printf("%d\n", best_dst);

	return 0;
}
