#define at(x, y) closed(board, board_width, board_height, x, y)

#include "utils.cl"
#include "ca/mazesolver.cl"

kernel void entrypoint(
	global uint* board_0,
	global uint* board_1,
	const  uint  board_ix,
	const  uint  board_width,
	const  uint  board_height
) {
	global uint* boards[]    = {board_0, board_1};
	global uint* read_board  = boards[board_ix];
	global uint* write_board = boards[1 - board_ix];

	int x = get_global_id(0);
	int y = get_global_id(1);
	uint ix = x + y * board_width;

	write_board[ix] = calc(read_board, board_width, board_height, x, y);
}
