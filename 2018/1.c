#include <stdio.h>
#include <stdlib.h>

static const int MAX_INPUT = 2048;
static const int MAX_FREQUENCY = 1024*1024;
static const int MIN_FREQUENCY = -MAX_FREQUENCY;

static int input[MAX_INPUT];
static char deja_vu[MAX_FREQUENCY * 2 / 8];

int main(void)
{
    int len = 0;
    int iter = 0;
    int total = 0;
    int current = 0;

    while(scanf("%d", &input[len]) == 1)
        len++;
    
    for(int i = 0; i < len; i++)
        total += input[i];

    for(int i = 0;; i = ((i + 1) % len))
    {
        current += input[i];
        iter++;

        if(current > MAX_FREQUENCY || current < MIN_FREQUENCY)
        {
            printf("%d is outside the range ]%d;%d]", current, MIN_FREQUENCY, MAX_FREQUENCY);

            return EXIT_FAILURE;
        }

        int index = (current + MAX_FREQUENCY) / 8;
        int mask = 1 << ((current + MAX_FREQUENCY) % 8);

        if(deja_vu[index] & mask)
            break;

        deja_vu[index] |= mask;
    }

    printf("Total : %d\n", total);
    printf("First seen twice : %d\n", current);
    printf("Sums : %d\n", iter);

    return EXIT_SUCCESS;
}
