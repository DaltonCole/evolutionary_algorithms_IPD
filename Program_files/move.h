//////////////////////////////////////////////////////////////////////
/// @file move.h
/// @author Dalton Cole, CS5201 A assignment 2b
/// @brief Class declaration of Move
//////////////////////////////////////////////////////////////////////

#ifndef MOVE_H
#define MOVE_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

struct Move {
	bool p; // Prisoner
	bool o; // Opponent

	void random_move() {
		p = (rand() % 2 == 1)? true : false;
		o = (rand() % 2 == 1)? true : false;
	}
};

#endif