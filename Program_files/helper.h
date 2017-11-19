#ifndef HELPER_H
#define HELPER_H

#include <deque>
#include <algorithm>
#include <vector>
#include <math.h>

#include "move.h"
#include "prisoner.h"

void pg_run(const int run_number);

void make_past_moves(const int number_of_moves, deque<Move>& move_queue);

void append_tic_for_tac(deque<Move>& move_queue, const bool p_move);

void find_parents(vector<Prisoner>& population, vector<Prisoner*>& parents);

void fitness_proportional_selection(vector<Prisoner>& population, vector<Prisoner*>& parents);

void over_selection(vector<Prisoner>& population, vector<Prisoner*>& parents);

void make_children(vector<Prisoner*>& parents, vector<Prisoner>& children);

void sub_tree_crossover(vector<Prisoner*>& parents, vector<Prisoner>& children);

void mutate_children(vector<Prisoner>& children);

void survival_selection(vector<Prisoner>& population);

void truncation(vector<Prisoner>& population);


#endif