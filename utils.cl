#define PARAMS \
	global uint* board,\
	const  uint  board_width,\
	const  uint  board_height,\
	int          x,\
	int          y

// Spaces

uint closed(PARAMS) {
	return
		x < 0 || y < 0 || x >= board_width || y >= board_height
		? 0
		: board[x + y * board_width];
}

uint torus(PARAMS) {
	if(x < 0) x += board_width;
	if(y < 0) y += board_height;
	x %= board_width;
	y %= board_height;
	return board[x + y * board_width];
}

// Neighbourhoods

uint neumann(PARAMS) {
	uint sum = 0;
	sum += at(x - 1, y) == 1;
	sum += at(x + 1, y) == 1;
	sum += at(x, y - 1) == 1;
	sum += at(x, y + 1) == 1;
	return sum;
}

uint moore(PARAMS) {
	uint sum = neumann(board, board_width, board_height, x, y);
	sum += at(x - 1, y - 1) == 1;
	sum += at(x - 1, y + 1) == 1;
	sum += at(x + 1, y - 1) == 1;
	sum += at(x + 1, y + 1) == 1;
	return sum;
}

