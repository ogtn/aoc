#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static const int MAX_MEMORY = 1 << 16;
static const int IO_SIZE = 16;
static const int NB_PHASES = 5;

#define CRASH(msg) do { quit(msg, __LINE__); } while(0);

typedef struct io
{
	long buff[IO_SIZE];
	int pos;
	int last;
} io;

typedef struct context
{
	long *mem;
	long *pc;
	int base;
	io *std_input;
	io *std_output;
} context;

void quit(const char* msg, int line)
{
	printf("Crash @ line %d: %s\n", line, msg);

	exit(EXIT_FAILURE);
}

long get_param(context *ctx, int position)
{
	int digit_pos = pow(10, position + 1);
	int mode = (*(ctx->pc) / digit_pos) % 10;
	long value;

	if(mode == 0)
		value = ctx->mem[ctx->pc[position]];
	else if(mode == 1)
		value = ctx->pc[position];
	else if(mode == 2)
		value = ctx->mem[ctx->pc[position] + ctx->base];
	else
		CRASH("invalid mode");

	return value;
}

int get_addr(context *ctx, int position)
{
	int digit_pos = pow(10, position + 1);
	int mode = (*(ctx->pc) / digit_pos) % 10;
	int addr;

	if(mode == 0)
		addr = ctx->pc[position];
	else if(mode == 2)
		addr = ctx->pc[position] + ctx->base;
	else
		CRASH("invalid mode");

	if(addr >= MAX_MEMORY)
		CRASH("memory overflow");

	if(addr < 0)
		CRASH("memory underflow");

	return addr;
}

void math_op(context *ctx, int opcode)
{
	long param1 = get_param(ctx, 1);
	long param2 = get_param(ctx, 2);
	int addr = get_addr(ctx, 3);

	if(opcode == 1)
		ctx->mem[addr] = param1 + param2;
	else if(opcode == 2)
		ctx->mem[addr] = param1 * param2;
	else
		CRASH("invalid math_op");

	ctx->pc += 4;
}

int io_op(context *ctx, int opcode)
{
	long param = get_param(ctx, 1);
	int addr = get_addr(ctx, 3);

	if(opcode == 3)
	{
		if(ctx->std_input->pos > ctx->std_input->last)
			return 1;

		ctx->mem[addr] = ctx->std_input->buff[ctx->std_input->pos++];

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
	long param1 = get_param(ctx, 1);
	long param2 = get_param(ctx, 2);
	int addr = get_addr(ctx, 3);

	if(opcode == 5)
		ctx->pc = param1 ? &ctx->mem[param2] : ctx->pc + 3;
	else if(opcode == 6)
		ctx->pc = !param1 ? &ctx->mem[param2] : ctx->pc + 3;
	else if(opcode == 7)
	{
		ctx->mem[addr] = param1 < param2; 
		ctx->pc += 4;
	}
	else if(opcode == 8)
	{
		ctx->mem[addr] = param1 == param2;
		ctx->pc += 4;
	}
}

void set_base(context *ctx)
{
	long param = get_param(ctx, 1);

	ctx->base += param;
	ctx->pc += 2;
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

			case 9:
				set_base(ctx);
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

long run_part1(const long *mem, long input)
{
	io io_buffers[2];
	context ctx;

	ctx.mem = malloc(sizeof *mem * MAX_MEMORY);
	memcpy(ctx.mem, mem, sizeof *mem * MAX_MEMORY);
	ctx.pc = ctx.mem;
	ctx.base = 0;
	ctx.std_input = &io_buffers[0];
	ctx.std_input->buff[0] = input;
	ctx.std_input->last = 0;
	ctx.std_input->pos = 0;
	ctx.std_output = &io_buffers[1];
	ctx.std_output->last = -1;
	ctx.std_output->pos = 0;

	run(&ctx);
	free(ctx.mem);

	return ctx.std_output->buff[ctx.std_output->last];
}

void read_code(long *mem)
{
	long *pc = mem;
	char buff[32];

	do
	{
		scanf("%[-0123456789]s", buff);
		*pc++ = atol(buff);
	} while(getchar() != EOF);
}

int main(void)
{
	static long memory[MAX_MEMORY];

	read_code(memory);
	printf("Part 1: %ld\n", run_part1(memory, 1));
	printf("Part 2: %ld\n", run_part1(memory, 2));

	return EXIT_SUCCESS;
}
