uint calc(
	global uint* board,
	const  uint  board_width,
	const  uint  board_height,
	int          x,
	int          y
) {
	int me = at(x, y);

	if(me == 0) {
		return moore(board, board_width, board_height, x, y) == 2;
	} else if(me == 1) {
		uint s = moore(board, board_width, board_height, x, y);
		return s >= 3 && s <= 5 ? 1 : 2;
	}
	else {
		return (me + 1) % 4;
	}
}
