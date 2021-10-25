uint s1sum(
	global uint* board,
	const  uint  board_width,
	int          x,
	int          y
) {
	uint sum = 0;
	sum += at(x - 1, y - 1) == 1;
	sum += at(x - 1, y    ) == 1;
	sum += at(x - 1, y + 1) == 1;
	sum += at(x    , y - 1) == 1;
	sum += at(x    , y + 1) == 1;
	sum += at(x + 1, y - 1) == 1;
	sum += at(x + 1, y    ) == 1;
	sum += at(x + 1, y + 1) == 1;
	return sum;
}

uint calc(
	global uint* board,
	const  uint  board_width,
	int          x,
	int          y
) {
	int me = at(x, y);

	if(me == 0) {
		return s1sum(board, board_width, x, y) == 2;
	} else if(me == 1) {
		uint s = s1sum(board, board_width, x, y);
		return s >= 3 && s <= 5 ? 1 : 2;
	}
	else {
		return (me + 1) % 5;
	}
}
