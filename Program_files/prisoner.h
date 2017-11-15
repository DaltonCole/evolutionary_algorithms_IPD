#ifndef PRISONER_H
#define PRISONER_H

#include <iostream>
#include <memory>
#include <stdlib.h>     /* srand, rand */
#include "config.h"

using namespace std;

class Prisoner;
ostream& operator <<(ostream& os, Prisoner p);

class Prisoner {
	public:
		Prisoner();
		Prisoner(const Prisoner & rhs);
		const Prisoner & operator =(const Prisoner & rhs);

		void randomly_initalize_tree();
		unique_ptr<Prisoner> generate_branch(int depth);
		string random_operator();
		int random_leaf();

		friend ostream& operator <<(ostream& os, Prisoner p);
	//private:
		static Config config;

		string op;

		int leaf_lhs;
		int leaf_rhs;

		unique_ptr<Prisoner> left_branch;
		unique_ptr<Prisoner> right_branch;

		int current_depth;
};

#endif