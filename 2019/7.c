#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static const int MAX_MEMORY = 4096;
static const int IO_SIZE = 16;
static const int NB_PHASES = 5;

#define CRASH(msg) do { quit(msg, __LINE__); } while(0);

typedef struct io
{
	int buff[IO_SIZE];
	int pos;
	int last;
} io;

typedef struct context
{
	int *mem;
	int *pc;
	io *std_input;
	io *std_output;
} context;

void quit(const char* msg, int line)
{
	printf("Crash @ line %d: %s\n", line, msg);

	exit(EXIT_FAILURE);
}

int get_param(int *mem, int *pc, int position)
{
	int digit_pos = pow(10, position + 1);
	int mode = (*pc / digit_pos) % 10;
	int value;

	if(mode == 0)
		value = mem[pc[position]];
	else if(mode == 1)
		value = pc[position];
	else
		CRASH("invalid mode");

	return value;
}

void math_op(context *ctx, int opcode)
{
	int param1 = get_param(ctx->mem, ctx->pc, 1);
	int param2 = get_param(ctx->mem, ctx->pc, 2);
	int dst = ctx->pc[3];

	if(opcode == 1)
		ctx->mem[dst] = param1 + param2;
	else if(opcode == 2)
		ctx->mem[dst] = param1 * param2;
	else
		CRASH("invalid math_op");

	ctx->pc += 4;
}

int io_op(context *ctx, int opcode)
{
	int param = get_param(ctx->mem, ctx->pc, 1);
	int dst = ctx->pc[1];

	if(opcode == 3)
	{
		if(ctx->std_input->pos > ctx->std_input->last)
			return 1;

		ctx->mem[dst] = ctx->std_input->buff[ctx->std_input->pos++];

		if(ctx->std_input->pos >= IO_SIZE)
			CRASH("exceeded std_input size");
	}
	else if(opcode == 4)
	{
		if(ctx->std_output->last >= IO_SIZE - 1)
			CRASH("exceeded std_output size");

		ctx->std_output->last++;
		ctx->std_output->buff[ctx->std_output->last] = param;
	}

	ctx->pc += 2;

	return 0;
}

void jmp_op(context *ctx, int opcode)
{
	int param1 = get_param(ctx->mem, ctx->pc, 1);
	int param2 = get_param(ctx->mem, ctx->pc, 2);
	int dst = ctx->pc[3];

	if(opcode == 5)
		ctx->pc = param1 ? &ctx->mem[param2] : ctx->pc + 3;
	else if(opcode == 6)
		ctx->pc = !param1 ? &ctx->mem[param2] : ctx->pc + 3;
	else if(opcode == 7)
	{
		ctx->mem[dst] = param1 < param2; 
		ctx->pc += 4;
	}
	else if(opcode == 8)
	{
		ctx->mem[dst] = param1 == param2;
		ctx->pc += 4;
	}
}

int run(context *ctx)
{
	int halt = 0;
	int io_blocked = 0;

	while(!halt && !io_blocked)
	{
		int opcode = *(ctx->pc) % 100;

		switch(opcode)
		{
			case 1:
			case 2:
				math_op(ctx, opcode);
				break;

			case 3:
			case 4:
				io_blocked = io_op(ctx, opcode);
				break;

			case 5:
			case 6:
			case 7:
			case 8:
				jmp_op(ctx, opcode);
				break;

			case 99:
				ctx->pc++;
				halt = 1;
				break;

			default:
				CRASH("unknow opcode");
		}
	}

	return io_blocked;
}

int run_amplification(const int *mem, int *phases, int use_feedbackloop)
{
	io io_buffers[5];
	context contexts[5];

	for(int i = 0; i < 5; i++)
	{
		io_buffers[i].buff[0] = phases[i];
		io_buffers[i].last = 0;
		io_buffers[i].pos = 0;

		contexts[i].mem = malloc(sizeof(int) * MAX_MEMORY);
		memcpy(contexts[i].mem, mem, sizeof(int) * MAX_MEMORY);
		contexts[i].pc = contexts[i].mem;
		contexts[i].std_input = &io_buffers[i % 5];
		contexts[i].std_output = &io_buffers[(i + 1) % 5];
	}

	io_buffers[0].buff[1] = 0;
	io_buffers[0].last = 1;

	for(int i = 0; i < 5; i++)
	{
		int io_blocked = run(&contexts[i]);

		if(use_feedbackloop && i == 4 && io_blocked)
			i = -1;
	}

	for(int i = 0; i < 5; i++)
		free(contexts[i].mem);

	return io_buffers[0].buff[io_buffers[0].last];
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
		res = run_amplification(mem, phases, use_feedbackloop);

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
	int feedback_phases[] = { 5, 6, 7, 8, 9 };

	read_code(memory);
	printf("Part 1: %d\n", permut_phases(memory, normal_phases, 0, 5));
	printf("Part 2: %d\n", permut_phases(memory, feedback_phases, 1, 5));

	return EXIT_SUCCESS;
}
