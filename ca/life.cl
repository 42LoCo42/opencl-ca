uint calc(PARAMS) {
	uint sum = moore(board, board_width, board_height, x, y);
	return sum == 3 || sum == 2 && at(x, y);
}
