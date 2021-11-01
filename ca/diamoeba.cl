uint calc(PARAMS) {
	uint sum = moore(board, board_width, board_height, x, y);
	return
		at(x, y)
		? sum >= 5 && sum <= 8
		: sum == 3 || sum >= 5 && sum <= 8;
}
