//////////////////////////////////////////////////////////////////////
/// @file prisoner.h
/// @author Dalton Cole, CS5201 A assignment 2b
/// @brief Class declaration of Prisoner
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/// @class Prisoner
/// @brief Represents a prisoner in the prisoner's Dilemma problem
//////////////////////////////////////////////////////////////////////

#ifndef PRISONER_H
#define PRISONER_H

#include <iostream>
#include <memory>
#include <deque>
#include <stdlib.h>     /* srand, rand */
#include <math.h>
#include <algorithm>
#include "config.h"
#include "move.h"

using namespace std;

class Prisoner;
ostream& operator <<(ostream& os, Prisoner p);

class Prisoner {
	public:
		Prisoner();
		Prisoner(const Prisoner& rhs);
		const Prisoner & operator =(const Prisoner& rhs);
		Prisoner(Prisoner&& other);
		Prisoner& operator =(Prisoner&& other);


		void randomly_initalize_tree();
		unique_ptr<Prisoner> generate_branch(int depth) const;
		string random_operator() const;
		int random_leaf() const;

		void assign_fitness();
		bool find_value() const;
		bool recursively_find_value(const Prisoner& branch) const;
		bool get_move_value(const int m) const;
		int fitness_function(const bool p, const bool o) const;

		float get_fitness() const;

		int assign_depth(Prisoner& p);
		void sub_tree_crossover(Prisoner& other);
		Prisoner* equal_level_branch(Prisoner& p, const int & goal_depth);
		void sub_tree_mutation();
		void recursive_sub_tree_mutation(Prisoner& branch);

		friend deque<Move> generate_move_queue();

		bool less_than(const Prisoner& rhs) const;
		bool operator <(const Prisoner& rhs);
		friend bool operator <(const Prisoner& a, const Prisoner& b);
		friend bool operator >(const Prisoner& a, const Prisoner& b);

		friend ostream& operator <<(ostream& os, Prisoner p);

		static Config config;
		static deque<Move> move_queue;


	private:
		

		string op;

		int leaf_lhs;
		int leaf_rhs;

		unique_ptr<Prisoner> left_branch;
		unique_ptr<Prisoner> right_branch;

		int current_depth;

		float fitness;

		deque<Move> current_move_queue;
};

#endif

//////////////////////////////////////////////////////////////////////
/// @fn 
/// @brief 
/// @pre 
/// @post 
/// @param 
/// @return 
//////////////////////////////////////////////////////////////////////