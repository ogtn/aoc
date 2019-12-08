#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static const int MAX_MEMORY = 4096;

#define CRASH() do { quit(__LINE__); } while(0);

void quit(int line)
{
    printf("Crash @ line %d\n", line);

    exit(EXIT_FAILURE);
}

int get_param(int *mem, int opcode, int *pc, int position)
{
    int digit_pos = pow(10, position + 1);
    int mode = (opcode / digit_pos) % 10;
    int value;

    if(mode == 0)
        value = mem[pc[position]];
    else if(mode == 1)
        value = pc[position];
    else
        CRASH();

    return value;
}

int math_op(int *mem, int opcode, int *pc)
{
    int param1 = get_param(mem, opcode, pc, 1);
    int param2 = get_param(mem, opcode, pc, 2);
    int dst = pc[3];
    opcode %= 100;

    if(opcode == 1)
        mem[dst] = param1 + param2;
    else if(opcode == 2)
        mem[dst] = param1 * param2;
    else
        CRASH();

    return 4;
}

int io_op(int *mem, int opcode, int *pc, int *std_input, int *std_output)
{
    int param = get_param(mem, opcode, pc, 1);
    int dst = pc[1];
    opcode %= 100;

    if(opcode == 3)
        mem[dst] = *std_input;
    else if(opcode == 4)
        *std_output = param;

    return 2;
}

int *jmp_op(int *mem, int opcode, int *pc)
{
    int param1 = get_param(mem, opcode, pc, 1);
    int param2 = get_param(mem, opcode, pc, 2);
    int dst = pc[3];

    int *next_pc;
    opcode %= 100;

    if(opcode == 5)
        next_pc = param1 ? &mem[param2] : pc + 3;
    else if(opcode == 6)
        next_pc = !param1 ? &mem[param2] : pc + 3;
    else if(opcode == 7)
    {
        mem[dst] = param1 < param2; 
        next_pc = pc + 4;
    }
    else if(opcode == 8)
    {
        mem[dst] = param1 == param2;
        next_pc = pc + 4;
    }

    return next_pc;
}

void run(const int *memory, int *std_input, int *std_output)
{
    int mem[MAX_MEMORY];
    int *pc = mem;
    int stop = 0;

    memcpy(mem, memory, sizeof(int) * MAX_MEMORY);

    while(!stop)
    {
        int opcode = *pc;

        //printf("mem[%d] = [%d, %d, %d, %d]\n", (int)(pc - mem), pc[0], pc[1], pc[2], pc[3]);

        switch(opcode % 100)
        {
            case 1:
            case 2:
                pc += math_op(mem, opcode, pc);
                break;

            case 3: 
            case 4:
                pc += io_op(mem, opcode, pc, std_input, std_output);
                break;

            case 5:
            case 6:
            case 7:
            case 8:
                pc = jmp_op(mem, opcode, pc);
                break;

            case 99:
                pc++;
                stop = 1;
                break;

            default:
                CRASH();
        }
    }
}

int main(void)
{
    static int memory[MAX_MEMORY];
    int *pc = memory;
    char buff[8];
    int std_input, std_output;

	do
	{
		scanf("%[-0123456789]s", buff);
		*pc++ = atoi(buff);
	} while(getchar() != EOF);

    std_input = 1;
    run(memory, &std_input, &std_output);
    printf("Part 1: %d\n", std_output);
    std_input = 5;
    run(memory, &std_input, &std_output);
    printf("Part 2: %d\n", std_output);

    return EXIT_SUCCESS;
}
