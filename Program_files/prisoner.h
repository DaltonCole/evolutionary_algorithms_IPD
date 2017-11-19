#ifndef PRISONER_H
#define PRISONER_H

#include <iostream>
#include <memory>
#include <deque>
#include <stdlib.h>     /* srand, rand */
#include <math.h>
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

		void set_move_queue(const deque<Move>& m_q);


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

		void sub_tree_crossover(Prisoner& other);
		void sub_tree_mutation();
		void recursive_sub_tree_mutation(Prisoner& branch);

		bool less_than(const Prisoner& rhs) const;
		bool operator <(const Prisoner& rhs);
		friend bool operator <(const Prisoner& a, const Prisoner& b);
		friend bool operator >(const Prisoner& a, const Prisoner& b);

		friend ostream& operator <<(ostream& os, Prisoner p);

		static Config config;
	//private:
		

		string op;

		int leaf_lhs;
		int leaf_rhs;

		unique_ptr<Prisoner> left_branch;
		unique_ptr<Prisoner> right_branch;

		int current_depth;

		float fitness;

		deque<Move> move_queue;
};

#endif