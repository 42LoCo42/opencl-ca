#include <poll.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "clutils.h"

const size_t board_width = 200;

const char* source = "#include \"kernel.cl\"";
struct termios term_attrs;

double ceil(double d) {
	int i = d;
	return d > i ? i + 1 : i;
}

int sgn(int i) {
	return i < 0 ? -1 : 1;
}

void printBoard(unsigned int board[]) {
	printf("\e[H");
	for(size_t i = 0; i < board_width; ++i) {
		for(size_t j = 0; j < board_width; ++j) {
			int cell = board[j + i * board_width];
			printf("\e[38;5;%dm█", cell);
		}
		putchar('\n');
	}
}

void printSpeed(int speed) {
	printf("\r\e[mSpeed: %d", speed);
	fflush(stdout);
}

void exitfunc() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_attrs);
	printf("\e[?25h"); // enable cursor
}

int main() {
	tcgetattr(STDIN_FILENO, &term_attrs);
	struct termios raw = term_attrs;
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	printf("\e[?25l"); // disable cursor
	atexit(exitfunc);

	int rc = 0;

	unsigned int board[board_width * board_width];
	const size_t bytes = sizeof(board);

	// fill board randomly
	srand(time(NULL));
	for(size_t i = 0; i < board_width * board_width; ++i) {
		board[i] = rand() % 2 == 0;
	}

	unsigned int board_ix = 0; // first read from board 0

	OpenCLData ocl = clFullSetup(source, "entrypoint");

	cl_mem mem_board_0;
	cl_mem mem_board_1;

	eref(mem_board_0 = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, bytes, NULL, &rc));
	eref(mem_board_1 = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, bytes, NULL, &rc));

	// write initial board
	eset(clEnqueueWriteBuffer(ocl.queue, mem_board_0, CL_TRUE, 0, bytes, board, 0, NULL, NULL));

	eset(clSetKernelArg(ocl.kernel, 0, sizeof(cl_mem), &mem_board_0));
	eset(clSetKernelArg(ocl.kernel, 1, sizeof(cl_mem), &mem_board_1));
	eset(clSetKernelArg(ocl.kernel, 2, sizeof(unsigned int), &board_ix));
	eset(clSetKernelArg(ocl.kernel, 3, sizeof(unsigned int), &board_width));

	int poll_timeout = -1;

	int running   = 1; // if the program is running
	int automatic = 0; // if the CA advances automatically
	int speed     = 1; // how many CA iterations per step

	printBoard(board);
	while(running) {
		int do_step = 0;

		struct pollfd poll_stdin = {.fd = STDIN_FILENO, .events = POLLIN};
		poll(&poll_stdin, 1, poll_timeout);
		if(poll_stdin.revents & POLLIN) {
			char c = getchar();
			switch(c) {
				case 'q': running = 0; break;
				case '\n':
					poll_timeout = poll_timeout - sgn(poll_timeout) * 126;
					automatic = 1 - automatic;
					break;
				case '+': speed++;    printSpeed(speed); break;
				case '-': speed--;    printSpeed(speed); break;
				case '*': speed *= 2; printSpeed(speed); break;
				case '_': speed /= 2; printSpeed(speed); break;
				case ' ': do_step = 1; break;
			}
		} else if(automatic) do_step = 1;

		if(do_step) {
			cl_mem* boards[] = {&mem_board_0, &mem_board_1};
			for(int i = 0; i < speed; ++i) {
				size_t global_sizes[] = {board_width, board_width};

				eset(clEnqueueNDRangeKernel(ocl.queue, ocl.kernel, 2, NULL, global_sizes, NULL, 0, NULL, NULL));
				eset(clFinish(ocl.queue));
				board_ix = 1 - board_ix; // switch boards
				eset(clSetKernelArg(ocl.kernel, 2, sizeof(unsigned int), &board_ix));
			}

			// read and display board
			eset(clEnqueueReadBuffer(ocl.queue, *boards[board_ix], CL_TRUE, 0, bytes, board, 0, NULL, NULL));
			printBoard(board);
		}
	}

	eset(clReleaseMemObject(mem_board_1));
	eset(clReleaseMemObject(mem_board_0));
	clFullCleanup(ocl);
}
