#pragma once;
struct Tile {
	int left;
	int right;
	int top;
	int bottom;

	bool isdone;

	Tile(){}
	Tile(int left, int right, int top, int bottom)
		: left(left),
		  right(right),
		  top(top),
		  bottom(bottom) {
	}
};