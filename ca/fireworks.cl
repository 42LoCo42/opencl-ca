uint calc(PARAMS) {
	int me = at(x, y);

	if(me == 0) {
		uint s = moore(board, board_width, board_height, x, y);
		return s == 1 || s == 3;
	} else if(me == 1) {
		uint s = moore(board, board_width, board_height, x, y);
		return s == 2 ? 1 : 2;
	}
	else {
		return (me + 1) % 21;
	}
}
