uint calc(PARAMS) {
	uint sum = moore(board, board_width, board_height, x, y);
	return at(x, y) ? 0 : sum == 2;
}
