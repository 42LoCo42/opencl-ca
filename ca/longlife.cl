uint calc(
	global uint* board,
	const  uint  board_width,
	const  uint  board_height,
	int          x,
	int          y
) {
	uint sum = moore(board, board_width, board_height, x, y);
	return
		at(x, y)
		? sum == 5
		: sum >= 3 && sum <= 5;
}
