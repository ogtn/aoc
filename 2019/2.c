#include <stdio.h>
#include <string.h>

static const int MAX_MEMORY = 1024;
static int memory[MAX_MEMORY] = {1,0,0,3,1,1,2,3,1,3,4,3,1,5,0,3,2,6,1,19,1,5,19,23,2,6,23,27,1,27,5,31,2,9,31,35,1,5,35,39,2,6,39,43,2,6,43,47,1,5,47,51,2,9,51,55,1,5,55,59,1,10,59,63,1,63,6,67,1,9,67,71,1,71,6,75,1,75,13,79,2,79,13,83,2,9,83,87,1,87,5,91,1,9,91,95,2,10,95,99,1,5,99,103,1,103,9,107,1,13,107,111,2,111,10,115,1,115,5,119,2,13,119,123,1,9,123,127,1,5,127,131,2,131,6,135,1,135,5,139,1,139,6,143,1,143,6,147,1,2,147,151,1,151,5,0,99,2,14,0,0};

void run(int *mem)
{
    int *pc = mem;

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
}

int main(void)
{
    int *pc = memory;

    /*
    while(scanf("%d%c", pc, &follow) == 2)
    {
        printf("%d\n", *pc);
        pc++;
    }

    scanf("%d", pc);
    */

    pc = memory;
    memory[1] = 12;
    memory[2] = 2;
    int mem[MAX_MEMORY];
    memcpy(mem, memory, sizeof(int) * MAX_MEMORY);
    run(mem);

    printf("Memory[0] = %d\n", memory[0]);

    for(int verb = 0; verb < 99; verb++)
    {
        for(int noun = 0; noun < 99; noun++)
        {
            int mem[MAX_MEMORY];
            memcpy(mem, memory, sizeof(int) * MAX_MEMORY);
            mem[1] = noun;
            mem[2] = verb;

            run(mem);

            if(mem[0] == 19690720)
            {
                int answer = 100 * noun + verb;
                printf("noun = %d, verb = %d, answer = %d\n", noun, verb, answer);

                return 0;
            }
        }   
    }

    return -1;
}
