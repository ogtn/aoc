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

int run_part1(const int *mem)
{
	io io_buffers[2];
	context ctx;

	ctx.mem = malloc(sizeof(int) * MAX_MEMORY);
	memcpy(ctx.mem, mem, sizeof(int) * MAX_MEMORY);
	ctx.pc = ctx.mem;
	ctx.std_input = &io_buffers[0];
	ctx.std_input->last = -1;
	ctx.std_input->pos = 0;
	ctx.std_output = &io_buffers[1];
	ctx.std_output->last = -1;
	ctx.std_output->pos = 0;

	run(&ctx);
	free(ctx.mem);

	return ctx.std_output->buff[ctx.std_output->last];
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

	read_code(memory);
	printf("Part 1: %d\n", run_part1(memory));

	return EXIT_SUCCESS;
}
