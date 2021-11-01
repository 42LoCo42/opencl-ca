#include "../mazestates.h"

#define isSearch(x, y) _isSearch(board, board_width, board_height, x, y)
#define isDeath(x, y)  _isDeath(board, board_width, board_height, x, y)

uint _isSearch(PARAMS) {
	uint cell = at(x, y);
	return cell == START || (cell >= SEARCH_UP && cell <= SEARCH_RIGHT);
}

uint _isDeath(PARAMS) {
	uint cell = at(x, y);
	return cell == DECAY || (cell >= ROUTE_UP && cell <= ROUTE_RIGHT);
}

uint calc(PARAMS) {
	uint me = at(x, y);
	switch(me) {
		case CLOSED:
		case ROUTE_UP:
		case ROUTE_DOWN:
		case ROUTE_LEFT:
		case ROUTE_RIGHT:
			return me;

		case OPEN:
			return
				isSearch(x, y - 1)   ? SEARCH_UP
				: isSearch(x, y + 1) ? SEARCH_DOWN
				: isSearch(x - 1, y) ? SEARCH_LEFT
				: isSearch(x + 1, y) ? SEARCH_RIGHT
				: me;

		case START:
			return isDeath(x, y - 1) || isDeath(x, y + 1) || isDeath(x - 1, y) || isDeath(x + 1, y) ? ROUTE_UP : START;

		case END:
			return
				isSearch(x, y - 1)   ? ROUTE_UP
				: isSearch(x, y + 1) ? ROUTE_DOWN
				: isSearch(x - 1, y) ? ROUTE_LEFT
				: isSearch(x + 1, y) ? ROUTE_RIGHT
				: me;

		case SEARCH_UP:
			if(isDeath(x, y - 1)) return DECAY;
			goto case_all_search;
		case SEARCH_DOWN:
			if(isDeath(x, y + 1)) return DECAY;
			goto case_all_search;
		case SEARCH_LEFT:
			if(isDeath(x - 1, y)) return DECAY;
			goto case_all_search;
		case SEARCH_RIGHT:
			if(isDeath(x + 1, y)) return DECAY;

case_all_search:
			return
				at(x, y - 1) == ROUTE_DOWN || at(x, y + 1) == ROUTE_UP || at(x - 1, y) == ROUTE_RIGHT || at(x + 1, y) == ROUTE_LEFT
				? me + 4
				: me;

		case DECAY: return OPEN;
	}
}
