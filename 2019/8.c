#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int WIDTH = 25;
static const int HEIGHT = 6;

int main(void)
{
	char img[WIDTH * HEIGHT];
	int best_zeros = 1 << 30;
	int layer = 0;
	int best_layer;
	int hash;
	char c;
	int cpt[3] = { 0 };

	memset(img, '2', sizeof img);

	while((c = getchar()) != EOF)
	{
		int pos = cpt[0] + cpt[1] + cpt[2];

		cpt[c - '0']++;

		if(img[pos] == '2')
			img[pos] = c;

		if(pos == (WIDTH * HEIGHT - 1))
		{
			if(cpt[0] < best_zeros)
			{
				best_layer = layer;
				best_zeros = cpt[0];
				hash = cpt[1] * cpt[2];
			}

			layer++;
			cpt[0] = cpt[1] = cpt[2] = 0;
		}
	}

	printf("Part 1 : layer %d has fewer zeros (%d), hash = %d\n", best_layer, best_zeros, hash);
	printf("Part 2 :\n");

	for(int j = 0; j < HEIGHT; j++)
	{
		for(int i = 0; i < WIDTH; i++)
			putchar(img[i + WIDTH * j] == '1' ? 'X' : ' ');

		putchar('\n');
	}

	return EXIT_SUCCESS;
}
