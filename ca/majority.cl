uint calc(
	global uint* board,
	const  uint  board_width,
	const  uint  board_height,
	int          x,
	int          y
) {
	const int radius = 4;
	const int thresh = 21;

	uint sum = 0;
	for(int dy = -radius; dy <= radius; ++dy) {
		int delta = radius - abs(dy);
		for(int dx = -delta; dx <= delta; ++dx) {
			sum += at(x + dx, y + dy);
		}
	}
	return sum >= thresh;
}
