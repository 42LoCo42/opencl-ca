uint calc(PARAMS) {
	uint sum = moore(board, board_width, board_height, x, y);
	return
		at(x, y)
		? sum >= 1 && sum <= 4
		: sum == 3;
}
