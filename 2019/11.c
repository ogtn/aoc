#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static const int MAX_MEMORY = 1 << 16;
static const int IO_SIZE = 2;

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
	long inst_cpt;
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
		CRASH("invalid address mode");

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
	if(opcode == 3)
	{
		if(ctx->std_input->pos > ctx->std_input->last)
			return 1;

		ctx->mem[get_addr(ctx, 1)] = ctx->std_input->buff[ctx->std_input->pos++];

		if(ctx->std_input->pos >= IO_SIZE)
			CRASH("exceeded std_input size");
	}
	else if(opcode == 4)
	{
		if(ctx->std_output->last >= IO_SIZE - 1)
			CRASH("exceeded std_output size");

		ctx->std_output->last++;
		ctx->std_output->buff[ctx->std_output->last] = get_param(ctx, 1);
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

		//printf("mem[%d] = [%ld, %ld, %ld, %ld]\n", (int)(ctx->pc - ctx->mem),
		//	ctx->pc[0], ctx->pc[1], ctx->pc[2], ctx->pc[3]);

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

		ctx->inst_cpt++;
	}

	return io_blocked;
}

typedef enum direction
{
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT
} direction;

static const int WIDTH = 100;
static const int HEIGHT = 100;
static char map[WIDTH][HEIGHT];

void run_day_11(const long *mem)
{
	int x = WIDTH / 2;
	int y = HEIGHT / 2;
	direction dir = DIR_UP;

	io io_buffers[2];
	context ctx;
	int is_running = 1;

	ctx.mem = malloc(sizeof *mem * MAX_MEMORY);
	memcpy(ctx.mem, mem, sizeof *mem * MAX_MEMORY);
	ctx.pc = ctx.mem;
	ctx.base = 0;
	ctx.inst_cpt = 0;
	ctx.std_input = &io_buffers[0];
	ctx.std_output = &io_buffers[1];

	while(is_running)
	{
		ctx.std_input->buff[0] = map[x][y] ? map[x][y] - 1 : 0;
		ctx.std_input->last = 0;
		ctx.std_input->pos = 0;

		ctx.std_output->last = -1;
		ctx.std_output->pos = 0;

		is_running = run(&ctx);
		map[x][y] = ctx.std_output->buff[0] + 1;
		
		if(ctx.std_output->buff[1] == 0)
			dir += 3;
		else
			dir++;

		dir %= 4;

		if(dir == DIR_UP)
			y--;
		if(dir == DIR_DOWN)
			y++;
		if(dir == DIR_LEFT)
			x--;
		if(dir == DIR_RIGHT)
			x++;
	}

	free(ctx.mem);
	printf("Instructions executed: %ld\n", ctx.inst_cpt);
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

void print_map(void)
{
	putchar('+');

	for(int i = 0; i < WIDTH; i++)
		putchar('-');

	putchar('+');
	putchar('\n');

	for(int j = 0; j < HEIGHT; j++)
	{
		putchar('|');

		for(int i = 0; i < WIDTH; i++)
			putchar(map[i][j] == 2 ? 'X' : ' ');

		putchar('|');
		putchar('\n');
	}

	putchar('+');

	for(int i = 0; i < WIDTH; i++)
		putchar('-');

	putchar('+');
	putchar('\n');
}


int main(void)
{
	static long memory[MAX_MEMORY];

	read_code(memory);

	run_day_11(memory);
	print_map();

	int cpt = 0;

	for(int j = 0; j < HEIGHT; j++)
		for(int i = 0; i < WIDTH; i++)
			cpt += !!map[i][j];

	printf("Part 1: %d\n", cpt);

	memset(map, 0, sizeof map);
	map[WIDTH / 2][HEIGHT / 2] = 2;
	run_day_11(memory);
	printf("Part 2:\n");
	print_map();

	return EXIT_SUCCESS;
}
