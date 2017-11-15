#ifndef HELPER_H
#define HELPER_H

#include <deque>
#include <algorithm>
#include <vector>

#include "move.h"
#include "prisoner.h"

void pg_run(const int run_number);

void make_past_moves(const int number_of_moves, deque<Move>& move_queue);

void append_tic_for_tac(deque<Move>& move_queue, const bool p_move);

#endif