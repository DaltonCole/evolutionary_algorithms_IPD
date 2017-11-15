#include "prisoner.h"

Prisoner::Prisoner() {
	leaf_lhs = 0;
	leaf_rhs = 0;
	op = "";
}

Prisoner::Prisoner(const Prisoner & rhs) {
	op = rhs.op;
	leaf_lhs = rhs.leaf_lhs;
	leaf_rhs = rhs.leaf_rhs;

	if(rhs.left_branch == nullptr) {
		left_branch = nullptr;
	} else {
		left_branch = unique_ptr<Prisoner> (new Prisoner(*rhs.left_branch));
	}

	if(rhs.right_branch == nullptr) {
		right_branch = nullptr;
	} else {
		right_branch = unique_ptr<Prisoner> (new Prisoner(*rhs.right_branch));
	}
}

const Prisoner & Prisoner::operator =(const Prisoner & rhs) {
	if(this != &rhs) {
		op = rhs.op;
		leaf_lhs = rhs.leaf_lhs;
		leaf_rhs = rhs.leaf_rhs;

		left_branch = unique_ptr<Prisoner> (new Prisoner(*rhs.left_branch));
		right_branch = unique_ptr<Prisoner> (new Prisoner(*rhs.right_branch));

		/*
		bool end = false;
		left_branch = unique_ptr<Prisoner> (new Prisoner);

		unique_ptr<Prisoner> p (new Prisoner);
		left_branch
		while(!end) {

		}
		*/
	}

	return *this;
}

/*
 Make a point
   Copy over x and y from the next pointer
   Make this new point pointed to by next
   Repeat with the new point as the base
  */

void Prisoner::randomly_initalize_tree() {
	op = random_operator();
	left_branch = generate_branch(1);
	if(op != "NOT") {
		right_branch = generate_branch(1);
	}
}

unique_ptr<Prisoner> Prisoner::generate_branch(int depth) {
	unique_ptr<Prisoner> new_branch(new Prisoner);

	// Random chance of increasing depth
	if(rand() % (config.max_depth - depth) >= 1) {
		new_branch -> op = random_operator();
		new_branch -> current_depth = depth + 1;
		new_branch -> left_branch = generate_branch(depth + 1);
		if(new_branch -> op != "NOT") {
			new_branch -> right_branch = generate_branch(depth + 1);
		}
	} else {
		new_branch -> left_branch = nullptr;
		new_branch -> right_branch = nullptr;
		new_branch -> op = random_operator();
		new_branch -> leaf_lhs = random_leaf();

		if(new_branch -> op != "NOT") {
			new_branch -> leaf_rhs = random_leaf();
		}
	}

	return new_branch;
}

string Prisoner::random_operator() {
	int r = rand() % 4;

	if(r == 0){
		return "AND";
	} else if(r == 1) {
		return "OR";
	} else if(r == 2) {
		return "XOR";
	} else {
		return "NOT";
	}
}

int Prisoner::random_leaf() {
	int r = rand() % (config.agent_memory_length + 1) + 1;

	if(rand() % 2 == 1) {
		return r;
	} else {
		return -r;
	}
}


ostream& operator <<(ostream& os, Prisoner p) {
	os << p.op << " ";
	if(p.left_branch != nullptr) {
		os << (*p.left_branch);
	} else {
		os << p.leaf_lhs << " ";
	}

	if(p.right_branch != nullptr) {
		os << (*p.right_branch);
	} else if(p.op != "NOT"){
		os << p.leaf_rhs << " ";
	}

	return os;
}