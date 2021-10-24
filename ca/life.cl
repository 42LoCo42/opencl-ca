uint calc(
	global uint* board,
	const  uint  board_width,
	int          x,
	int          y
) {
	uint sum = 0;
	sum += at(x - 1, y - 1);
	sum += at(x - 1, y    );
	sum += at(x - 1, y + 1);
	sum += at(x    , y - 1);
	sum += at(x    , y + 1);
	sum += at(x + 1, y - 1);
	sum += at(x + 1, y    );
	sum += at(x + 1, y + 1);
	return sum == 3 || sum == 2 && at(x, y);
}
