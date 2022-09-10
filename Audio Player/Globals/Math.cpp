
#include "Math.h"


/*
* Compares two SDL_Rects together
*
* @param r1, The first rect to compare
* @param r2, The second rect to compare
* @return true, They are the same
* @return false, They are different
*/
bool compare(SDL_Rect r1, SDL_Rect r2) {
	return (
		r1.x == r2.x && r1.y == r2.y &&
		r1.w == r2.w && r1.h == r2.h
		);
}
