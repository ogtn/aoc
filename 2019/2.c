#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const int MAX_MEMORY = 1024;
static int memory[MAX_MEMORY];

int run(int noun, int verb)
{
    int mem[MAX_MEMORY];
    int *pc = mem;

    memcpy(mem, memory, sizeof(int) * MAX_MEMORY);
	pc[1] = noun;
	pc[2] = verb;

    for(;;)
    {
        int opcode = *pc;

        if(opcode == 99)
            break;

        int rhs = mem[pc[1]];
        int lhs = mem[pc[2]];
        int dst = pc[3];

        if(opcode == 1)
            mem[dst] = rhs + lhs;
        else
            mem[dst] = rhs * lhs;

        pc += 4;
    }

    return mem[0];
}

int main(void)
{
    int *pc = memory;
    char buff[8];

	do
	{
		scanf("%[0123456789]s", buff);
		*pc++ = atoi(buff);
	} while(getchar() != EOF);

    printf("Part 1: answer = %d\n", run(12, 2));

    for(int verb = 0; verb < 99; verb++)
    {
        for(int noun = 0; noun < 99; noun++)
        {
            if(run(noun, verb) == 19690720)
            {
                int answer = 100 * noun + verb;
                printf("Part 2: noun = %d, verb = %d, answer = %d\n", noun, verb, answer);

                return 0;
            }
        }   
    }

    return -1;
}
