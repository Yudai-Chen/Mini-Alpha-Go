#pragma once

#include<utility>

//the coordinate
typedef std::pair<short, short> Coord;
enum State { WHITE, BLACK, VALID, INVALID };

// coordination change of a move
const short delta[8][2] = { { -1, 0 },{ -1, 1 },{ 0, 1 },{ 1, 1 },{ 1, 0 },{ 1, -1 },{ 0, -1 },{ -1, -1 } };   //clockwise

/*
* model of a position
*/
class Position
{
public:
	State state;
	/*
	* if a direction is valid, in the following order
	*   7 0 1 
	*   6 * 2
	*   5 4 3
	*/
	bool valid_dir[8];

	Position() {
		state = INVALID;
	}
};
