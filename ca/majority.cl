uint calc(PARAMS) {
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
