#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static const int MAX_MEMORY = 4096;
static const int IO_SIZE = 256;
static const int NB_PHASES = 5;

#define CRASH(msg) do { quit(msg, __LINE__); } while(0);

typedef struct io
{
	int buff[IO_SIZE];
	int pos;
	int last;
} io;

void quit(const char* msg, int line)
{
    printf("Crash @ line %d: %s\n", line, msg);

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
        CRASH("invalid mode");

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
        CRASH("invalid math_op");

    return 4;
}

int io_op(int *mem, int opcode, int *pc, io *std_input, io *std_output)
{
    int param = get_param(mem, opcode, pc, 1);
    int dst = pc[1];
    opcode %= 100;

    if(opcode == 3)
    {
    	if(std_input->pos > std_input->last)
    		CRASH("nothing to read"); // need to pause here

        mem[dst] = std_input->buff[std_input->pos++];
    }
    else if(opcode == 4)
    {
    	std_output->last++;
        std_output->buff[std_output->last] = param;
    }

    if(std_input->pos >= IO_SIZE)
    	CRASH("exceeded std_input size");

    if(std_output->last >= IO_SIZE)
    	CRASH("exceeded std_output size");

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

void run(const int *memory, io *std_input, io *std_output)
{
    int mem[MAX_MEMORY];
    int *pc = mem;
    int stop = 0;

    memcpy(mem, memory, sizeof(int) * MAX_MEMORY);

    while(!stop)
    {
        int opcode = *pc;

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
                CRASH("unknow opcode");
        }
    }
}

int run_amplification(const int *mem, int *phases, int use_feedbackloop)
{
    io std_input, std_output;

	std_input.buff[1] = 0;
	std_input.last = 1;
	std_output.pos = 0;
	std_output.last = -1;
    std_input.pos = 0;

    for(int i = 0; i < 5; i++)
    {
    	std_input.buff[0] = phases[i];
    	run(mem, &std_input, &std_output);
    	
    	printf("amp %d wrote %d values and read %d\n", i, std_output.last + 1, std_input.pos);

    	for(int j = 0; j <= std_output.last; j++)
    		printf("[%d] %d\n", j, std_output.buff[j]);

    	std_output.pos = 0;
    	std_output.last = -1;
    	std_input.pos = 0;
    	std_input.last = 1;
    	std_input.buff[1] = std_output.buff[0];


    	if(use_feedbackloop && i == 4)
    	{
    		//i = -1;
    	}
	}

    return std_output.buff[0];
}

int permut_phases(const int *mem, int *phases, int use_feedbackloop, int depth)
{
	int res = 0;
	int start = (5 - depth);

	for(int i = start; i < 5; i++)
	{
		int phases_cpy[5];

		memcpy(phases_cpy, phases, sizeof phases_cpy);		
		phases_cpy[i] = phases[start];
		phases_cpy[start] = phases[i];
		
		int tmp = permut_phases(mem, phases_cpy, use_feedbackloop, depth - 1);

		if(tmp > res)
			res = tmp;
	}

	if(depth == 0)
	{
		res = run_amplification(mem, phases, use_feedbackloop);

		for(int i = 0; i < 5; i++)
			printf("%d", phases[i]);

		printf(" -> %d\n", res);
		//exit(EXIT_SUCCESS);
	}

	return res;
}

void read_code(int *mem)
{
    int *pc = mem;
    char buff[8];

	do
	{
		scanf("%[-0123456789]s", buff);
		*pc++ = atoi(buff);
	} while(getchar() != EOF);
}

int main(void)
{
    static int memory[MAX_MEMORY];
	int normal_phases[] = { 0, 1, 2, 3, 4 };
	//int feedback_phases[] = { 5, 6, 7, 8, 9 };
	int feedback_phases[] = { 9,8,7,6,5 };

	read_code(memory);
	printf("Part 1: %d\n", permut_phases(memory, normal_phases, 0, 5));
	//printf("Part 2: %d\n", permut_phases(memory, feedback_phases, 1, 5));

    return EXIT_SUCCESS;
}
