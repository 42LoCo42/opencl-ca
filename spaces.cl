uint closed(
	global uint* board,
	const  uint  board_width,
	int          x,
	int          y
) {
	return
		x < 0 || y < 0 || x >= board_width || y >= board_width
		? 0
		: board[x + y * board_width];
}

uint torus(
	global uint* board,
	const  uint  board_width,
	int          x,
	int          y
) {
	// torus
	if(x < 0) x += board_width;
	if(y < 0) y += board_width;
	x %= board_width;
	y %= board_width;
	return board[x + y * board_width];
}
