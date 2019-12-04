#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int min = 171309;
	int max = 643603;
	int cpt1 = 0;
	int cpt2 = 0;

	for(int i = min; i <= max; i++)
	{
		char word[7];
		char last;
		int n = 1;
		int is_ordered = 1;
		int has_double = 0;
		int has_exact_double = 0;

		snprintf(word, sizeof word, "%d", i);
		last = word[0];

		for(int j = 1; j < 6; j++)
		{
			if(word[j-1] > word[j])
				is_ordered = 0;

			if(word[j-1] == word[j])
				has_double = 1;
			
			if(word[j] == last)
				n++;
			else
			{				
				if(n == 2)
					has_exact_double = 1;

				n = 1;
				last = word[j];
			}
		}

		if(n == 2)
			has_exact_double = 1;

		if(is_ordered && has_double)
		{
			cpt1++;

			if(has_exact_double)
				cpt2++;
		}
	}

	printf("Part 1 : %d\n", cpt1);
	printf("Part 2 : %d\n", cpt2);

	return 0;
}
