//////////////////////////////////////////////////////////////////////
/// @file helper.h
/// @author Dalton Cole, CS5201 A assignment 2b
/// @brief Declaration of Helper functions
//////////////////////////////////////////////////////////////////////

#ifndef HELPER_H
#define HELPER_H

#include <deque>
#include <algorithm>
#include <vector>
#include <math.h>
#include <fstream>

#include "move.h"
#include "prisoner.h"

void pg_run(const int run_number, Prisoner& best_prisoner, string& log_string, string& absolute_string);

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

void k_tournament_selection_without_replacement(vector<Prisoner>& population);

void find_best_in_tournament(const vector<Prisoner>& population, const vector<int>& random_numbers, int& best);

void make_solution_file(vector<Prisoner>& best_population);

float find_average_fitness(const vector<Prisoner>& population);

void make_log_file(vector<string>& log);

void make_absolute_fitness_log_file(vector<string>& overall_absolute_fitness_log);

void inforce_hall_of_fame(vector<Prisoner>& population, deque<Prisoner>& hall_of_fame);

#endif