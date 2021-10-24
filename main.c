#include <time.h>
#include <unistd.h>

#include "clutils.h"

const char* source =
#include "kernel.cl"
;

const size_t board_width = 75;

double ceil(double d) {
	int i = d;
	return d > i ? i + 1 : i;
}

void printBoard(unsigned int board[]) {
	printf("\e[H");
	for(size_t i = 0; i < board_width + 2; ++i) {
		putchar('-');
	}
	putchar('\n');

	for(size_t i = 0; i < board_width; ++i) {
		putchar('|');
		for(size_t j = 0; j < board_width; ++j) {
			int cell = board[j + i * board_width];
			printf("%s", cell ? "█" : " ");
		}
		puts("|");
	}

	for(size_t i = 0; i < board_width + 2; ++i) {
		putchar('-');
	}
	putchar('\n');
}

void cursorOn() {
	printf("\e[?25h");
}

int main() {
	printf("\e[?25l");
	atexit(cursorOn);

	int rc = 0;

	unsigned int board[board_width * board_width];
	const size_t bytes = sizeof(board);

	srand(time(NULL));
	for(size_t i = 0; i < board_width * board_width; ++i) {
		board[i] = rand() % 2 == 0;
	}

	unsigned int board_ix = 0;

	OpenCLData ocl = clFullSetup(source, "entrypoint");

	cl_mem mem_board_0;
	cl_mem mem_board_1;
	cl_mem* boards[] = {&mem_board_0, &mem_board_1};

	eref(mem_board_0 = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, bytes, NULL, &rc));
	eref(mem_board_1 = clCreateBuffer(ocl.context, CL_MEM_READ_WRITE, bytes, NULL, &rc));

	eset(clEnqueueWriteBuffer(ocl.queue, mem_board_0, CL_TRUE, 0, bytes, board, 0, NULL, NULL));

	eset(clSetKernelArg(ocl.kernel, 0, sizeof(cl_mem), &mem_board_0));
	eset(clSetKernelArg(ocl.kernel, 1, sizeof(cl_mem), &mem_board_1));
	eset(clSetKernelArg(ocl.kernel, 2, sizeof(unsigned int), &board_ix));
	eset(clSetKernelArg(ocl.kernel, 3, sizeof(unsigned int), &board_width));

	size_t global_sizes[] = {board_width, board_width};

	printBoard(board);
	// getchar();
	for(;;) {
		eset(clEnqueueNDRangeKernel(ocl.queue, ocl.kernel, 2, NULL, global_sizes, NULL, 0, NULL, NULL));
		eset(clFinish(ocl.queue));

		board_ix = 1 - board_ix;
		eset(clSetKernelArg(ocl.kernel, 2, sizeof(unsigned int), &board_ix));

		eset(clEnqueueReadBuffer(ocl.queue, *boards[board_ix], CL_TRUE, 0, bytes, board, 0, NULL, NULL));
		printBoard(board);

		// if(getchar() == 'q') break;
		usleep(125000);
	}

	printf("\n");
	eset(clReleaseMemObject(mem_board_1));
	eset(clReleaseMemObject(mem_board_0));
	clFullCleanup(ocl);
}
