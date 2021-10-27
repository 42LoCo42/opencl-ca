#include <poll.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#include "clutils.h"

size_t board_width;
size_t board_height;
size_t board_cells;

const char* source = "#include \"kernel.cl\"";

struct termios term;

double ceil(double d) {
	int i = d;
	return d > i ? i + 1 : i;
}

int sgn(int i) {
	return i < 0 ? -1 : 1;
}

void printBoard(unsigned int board[]) {
	printf("\e[H");
	for(size_t y = 0; y < board_height; ++y) {
		for(size_t x = 0; x < board_width; ++x) {
			int cell = board[x + y * board_width];
			printf("\e[38;5;%dm█", cell);
		}
		if(y < board_height - 1) putchar('\n'); // don't break after last line
	}
}

void termInputMode(int val) {
	if(val) {
		term.c_lflag |= ECHO | ICANON;
		printf("\e[25h\n");
	} else {
		term.c_lflag &= ~(ECHO | ICANON);
		printf("\e[25l\n");
	}
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

void exitfunc() {
	termInputMode(1);
}

int readNum(const char* prompt, int* out) {
	int rc = 0;
	termInputMode(1);

	char* line = readline(prompt);
	if(line) {
		char* invalid;
		long new = strtol(line, &invalid, 0);
		if(*line != 0 && *invalid == 0) *out = new; // from readline(3)
		free(line);
		rc = 1;
	}

	termInputMode(0);
	return rc;
}

int main() {
	tcgetattr(STDIN_FILENO, &term);
	termInputMode(0);
	atexit(exitfunc);

	// read terminal size
	struct winsize winsize;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsize);
	board_width  = winsize.ws_col;
	board_height = winsize.ws_row;
	board_cells = board_width * board_height;

	int rc = 0;

	const size_t bytes = sizeof(unsigned int) * board_cells;
	unsigned int* board = malloc(bytes);

	// fill board randomly
	srand(time(NULL));
	for(size_t i = 0; i < board_width * board_height; ++i) {
		board[i] = rand() % 2 == 0;
	}

	unsigned int board_ix = 0; // first read from board 0

	OpenCLData ocl = clFullSetup(source, "entrypoint");

	cl_mem mem_board_0;
	cl_mem mem_board_1;
	cl_mem* boards[] = {&mem_board_0, &mem_board_1};

	eref(mem_board_0 = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, bytes, NULL, &rc));
	eref(mem_board_1 = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, bytes, NULL, &rc));

	// write initial board
	eset(clEnqueueWriteBuffer(ocl.queue, mem_board_0, CL_TRUE, 0, bytes, board, 0, NULL, NULL));

	eset(clSetKernelArg(ocl.kernel, 0, sizeof(cl_mem), &mem_board_0));
	eset(clSetKernelArg(ocl.kernel, 1, sizeof(cl_mem), &mem_board_1));
	eset(clSetKernelArg(ocl.kernel, 2, sizeof(unsigned int), &board_ix));
	eset(clSetKernelArg(ocl.kernel, 3, sizeof(unsigned int), &board_width));
	eset(clSetKernelArg(ocl.kernel, 4, sizeof(unsigned int), &board_height));

	int poll_timeout = -1;
	int poll_timeout_storage = 25;

	int running    = 1; // if the program is running
	int automatic  = 0; // if the CA advances automatically
	int iterations = 1; // how many CA iterations per step

	printBoard(board);
	while(running) {
		int do_step = 0;

		struct pollfd poll_stdin = {.fd = STDIN_FILENO, .events = POLLIN};
		poll(&poll_stdin, 1, poll_timeout);
		if(poll_stdin.revents & POLLIN) {
			char c = getchar();
			switch(c) {
				case 'q': running = 0; break;
				case ' ': do_step = 1; break;
				case '\n':
					// toggle automatic
					automatic ^= 1;
					// swap timeouts
					poll_timeout ^= poll_timeout_storage;
					poll_timeout_storage ^= poll_timeout;
					poll_timeout ^= poll_timeout_storage;
					break;
				case 'i': readNum("\r\e[mIterations: ", &iterations); break;
				case 't': readNum("\r\e[mTimeout (in ms): ", &poll_timeout); break;
			}
		} else if(automatic) do_step = 1;

		if(do_step) {
			for(int i = 0; i < iterations; ++i) {
				size_t global_sizes[] = {board_width, board_height};

				eset(clEnqueueNDRangeKernel(ocl.queue, ocl.kernel, 2, NULL, global_sizes, NULL, 0, NULL, NULL));
				eset(clFinish(ocl.queue));
				board_ix ^= 1; // switch boards
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
	free(board);
}
