#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static const int MAX_SIZE = 32768;
static const int ORIGIN = MAX_SIZE / 2;
static char map[MAX_SIZE / 4][MAX_SIZE];

typedef struct segment
{
	short len;
	char direction;
} segment;

int read(segment *segments)
{
	int size = 0;
	char buff[8];

	do
	{
		scanf("%c%[0123456789]s", &segments[size].direction, buff);
		segments[size++].len = atoi(buff);
	} while(getchar() == ',');

	return size;
}

void read_and_mark(segment *segments, int size, int id)
{
	int pos_x = ORIGIN;
	int pos_y = ORIGIN;

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < segments[i].len; j++)
		{
			pos_x += (segments[i].direction == 'R') - (segments[i].direction == 'L');
			pos_y += (segments[i].direction == 'U') - (segments[i].direction == 'D');

			if(pos_x < 0 || pos_x >= MAX_SIZE || pos_y < 0 || pos_y >= MAX_SIZE)
			{
				printf("Coordinates out of range : [%d,%d]\n", pos_x, pos_y);
				exit(-1);
			}

			char mask = 1 << ((pos_x % 4) * 2 + id);
			map[pos_x / 4][pos_y] |= mask;
		}
	}
}

int find(segment *segments, int size)
{
	int best_dst = MAX_SIZE * MAX_SIZE;
	int pos_x = ORIGIN;
	int pos_y = ORIGIN;

	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < segments[i].len; j++)
		{
			pos_x += (segments[i].direction == 'R') - (segments[i].direction == 'L');
			pos_y += (segments[i].direction == 'U') - (segments[i].direction == 'D');

			char mask = 0x3 << ((pos_x % 4) * 2);

			if((map[pos_x / 4][pos_y] & mask) == mask)
			{
				int dst = abs((ORIGIN - abs(pos_x))) + abs((ORIGIN - abs(pos_y)));

				if(dst < best_dst)
					best_dst = dst;
			}
		}
	}

	return best_dst;
}

int main(void)
{
	segment wire_1[512], wire_2[512];
	int wire_1_size, wire_2_size;

	wire_1_size = read(wire_1);
	getchar();
	wire_2_size = read(wire_2);
	read_and_mark(wire_1, wire_1_size, 0);
	read_and_mark(wire_2, wire_2_size, 1);
	printf("Part 1: %d\n", find(wire_1, wire_1_size));

	return 0;
}
