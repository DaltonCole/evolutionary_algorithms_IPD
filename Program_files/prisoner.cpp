#include "prisoner.h"

Prisoner::Prisoner() {
	leaf_lhs = 0;
	leaf_rhs = 0;
	op = "";
	current_depth = 0;

	fitness = 0;
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

	fitness = rhs.fitness;
	current_depth = rhs.current_depth;
}

const Prisoner & Prisoner::operator =(const Prisoner & rhs) {
	if(this != &rhs) {
		op = rhs.op;
		leaf_lhs = rhs.leaf_lhs;
		leaf_rhs = rhs.leaf_rhs;
		current_depth = rhs.current_depth;
		fitness = rhs.fitness;

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

	return *this;
}

Prisoner::Prisoner(Prisoner&& other) {	
	op = other.op;
	leaf_lhs = other.leaf_lhs;
	leaf_rhs = other.leaf_rhs;
	current_depth = other.current_depth;
	fitness = other.fitness;
	left_branch.swap(other.left_branch);
	right_branch.swap(other.right_branch);
}

Prisoner& Prisoner::operator =(Prisoner&& other) {
	op = other.op;
	leaf_lhs = other.leaf_lhs;
	leaf_rhs = other.leaf_rhs;
	current_depth = other.current_depth;
	fitness = other.fitness;
	left_branch.swap(other.left_branch);
	right_branch.swap(other.right_branch);

	return *this;
}

/*
 Make a point
   Copy over x and y from the next pointer
   Make this new point pointed to by next
   Repeat with the new point as the base
  */

// ramped half-and-half: Grow method
void Prisoner::randomly_initalize_tree() {
	op = random_operator();
	left_branch = generate_branch(1);
	if(left_branch == nullptr) {
		leaf_lhs = random_leaf();
		current_depth = 1;
	} else {
		current_depth = left_branch -> current_depth + 1;
	}
	if(op != "NOT") {
		right_branch = generate_branch(1);

		if(right_branch == nullptr) {
			leaf_rhs = random_leaf();
		} else {
			current_depth = max(current_depth, right_branch -> current_depth + 1);
		}
	}

	return;
}

unique_ptr<Prisoner> Prisoner::generate_branch(int depth) const {
	unique_ptr<Prisoner> new_branch(new Prisoner);

	// Random chance of increasing depth
	if((config.max_depth - depth) != 0 && rand() % (config.max_depth - depth) >= 1) {
		new_branch -> op = random_operator();
		new_branch -> left_branch = generate_branch(depth + 1);
		new_branch -> current_depth = (new_branch -> left_branch -> current_depth) + 1;
		if(new_branch -> op != "NOT") {
			new_branch -> right_branch = generate_branch(depth + 1);
			if(new_branch -> current_depth < (new_branch -> right_branch -> current_depth) + 1) {
				new_branch -> current_depth = (new_branch -> right_branch -> current_depth) + 1;
			}
		}

	} else {
		new_branch -> left_branch = nullptr;
		new_branch -> right_branch = nullptr;
		new_branch -> op = random_operator();
		new_branch -> leaf_lhs = random_leaf();
		new_branch -> current_depth = 1;

		if(new_branch -> op != "NOT") {
			new_branch -> leaf_rhs = random_leaf();
		}
	}

	return new_branch;
}

string Prisoner::random_operator() const {
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

int Prisoner::random_leaf() const {
	int r = rand() % (config.agent_memory_length) + 1;

	if(rand() % 2 == 1) {
		return r;
	} else {
		return -r;
	}
}

void Prisoner::assign_fitness() {
	current_move_queue = move_queue;

	// Play 2k rounds;
	for(int i = 0; i < (2*config.agent_memory_length); i++) {
		append_tic_for_tac(current_move_queue, find_value());
	}

	bool decision;
	fitness = 0; 
	// Play l rounds that count towards fitness;
	for(int i = 0; i < config.iterations; i++) {
		// Make decision
		decision = find_value();
		// Add fitness value to fitness
		fitness += fitness_function(decision, current_move_queue.back().o);
		// Apply tic-for-tac
		append_tic_for_tac(current_move_queue, decision);
	}

	// Take average for fitness
	fitness /= config.iterations;

	// Add bloat control
	fitness -= (config.parsimony_pressure * current_depth);
}

bool Prisoner::find_value() const {
	return recursively_find_value(*this);
}

bool Prisoner::recursively_find_value(const Prisoner& branch) const {
	// Base case (only leaves)
	if(branch.left_branch == nullptr && branch.right_branch == nullptr) {
		if(branch.op == "AND") {
			return get_move_value(branch.leaf_lhs) & get_move_value(branch.leaf_rhs);
		} else if(branch.op == "OR") {
			return get_move_value(branch.leaf_lhs) | get_move_value(branch.leaf_rhs);
		} else if(branch.op == "XOR") {
			return get_move_value(branch.leaf_lhs) ^ get_move_value(branch.leaf_rhs);
		} else if(branch.op == "NOT") {
			return !get_move_value(branch.leaf_lhs);
		}
	} else if(branch.left_branch == nullptr && branch.right_branch != nullptr) {
		if(branch.op == "AND") {
			return get_move_value(branch.leaf_lhs) & recursively_find_value(*branch.right_branch);
		} else if(branch.op == "OR") {
			return get_move_value(branch.leaf_lhs) | recursively_find_value(*branch.right_branch);
		} else if(branch.op == "XOR") {
			return get_move_value(branch.leaf_lhs) ^ recursively_find_value(*branch.right_branch);
		} else if(branch.op == "NOT") {
			return !get_move_value(branch.leaf_lhs);
		}
	} else if(branch.left_branch != nullptr && branch.right_branch == nullptr){
		if(branch.op == "AND") {
			return recursively_find_value(*branch.left_branch) & get_move_value(branch.leaf_rhs);
		} else if(branch.op == "OR") {
			return recursively_find_value(*branch.left_branch) | get_move_value(branch.leaf_rhs);
		} else if(branch.op == "XOR") {
			return recursively_find_value(*branch.left_branch) ^ get_move_value(branch.leaf_rhs);
		}
	} else {
		if(branch.op == "AND") {
			return recursively_find_value(*branch.left_branch) & recursively_find_value(*branch.right_branch);
		} else if(branch.op == "OR") {
			return recursively_find_value(*branch.left_branch) | recursively_find_value(*branch.right_branch);
		} else if(branch.op == "XOR") {
			return recursively_find_value(*branch.left_branch) ^ recursively_find_value(*branch.right_branch);
		} else if(branch.op == "NOT") {
			return !recursively_find_value(*branch.left_branch);
		}
	}
}

bool Prisoner::get_move_value(const int m) const {
	if(m > 0) {
		return current_move_queue[m - 1].p;
	} else {
		return current_move_queue[-m - 1].o;
	}
} 

int Prisoner::fitness_function(const bool p, const bool o) const {
	if(p == true && o == true) {
		return 3;
	} else if(p == true && o == false) {
		return 0;
	} else if(p == false && o == true) {
		return 5;
	} else if(p == false && o == false) {
		return 1;
	}
}

float Prisoner::get_fitness() const {
	return fitness;
}

int Prisoner::assign_depth(Prisoner& p) {
	// If neither side is null
	if(p.left_branch != nullptr && p.right_branch != nullptr) {
		p.current_depth = max(assign_depth(*p.left_branch), assign_depth(*p.right_branch)) + 1;
	} else if(p.left_branch != nullptr) {
		p.current_depth = assign_depth(*p.left_branch) + 1;
	} else if(p.right_branch != nullptr) {
		p.current_depth = assign_depth(*p.right_branch) + 1;
	} else {
		return 1;
	}
	return p.current_depth;
}

void Prisoner::sub_tree_crossover(Prisoner& other) {
	// Assign depths
	assign_depth(*this);
	assign_depth(other);

	// Take min value of current depth
	int random_num = rand() % (min(current_depth, other.current_depth) - 1) + 1;

	// Find branches of equal height
	Prisoner* lhs = equal_level_branch(*this, random_num);
	Prisoner* rhs = equal_level_branch(other, random_num);

	//*lhs -> swap(*rhs);
	swap(*lhs, *rhs);

	return;
}

Prisoner* Prisoner::equal_level_branch(Prisoner& p, const int & goal_depth) {
	// Base case, we are at the right depth
	if(p.current_depth == goal_depth) {
		return &p;
	} else {
		// Randmly go down a branch
		if(rand() % 2 == 0) {
			// Go down left branch first
			if(p.left_branch != nullptr && p.left_branch -> current_depth >= goal_depth) {
				return equal_level_branch(*p.left_branch, goal_depth);
			} else {
				return equal_level_branch(*p.right_branch, goal_depth);
			}
		} else {
			// Go down right branch first
			if(p.right_branch != nullptr && p.right_branch -> current_depth >= goal_depth) {
				return equal_level_branch(*p.right_branch, goal_depth);
			} else {
				return equal_level_branch(*p.left_branch, goal_depth);
			}
		}
	}
}

void Prisoner::sub_tree_mutation() {
	recursive_sub_tree_mutation(*this);

	return;
}

void Prisoner::recursive_sub_tree_mutation(Prisoner& branch) {
	// Change op
	if(rand() % 100 < int(100.0 * config.mutation_rate)) {
		string old_op = op;
		op = random_operator();
		if(op == "NOT") {
			branch.right_branch.reset();
			branch.leaf_rhs = 0;
		} else if(old_op == "NOT" && op != "NOT") {
			branch.right_branch = generate_branch(branch.current_depth + 1);
			if(branch.right_branch -> current_depth >= branch.current_depth) {
				branch.current_depth = branch.right_branch -> current_depth + 1;
			}
		}
	}

	// Base case, left branch
	if(branch.left_branch == nullptr) {
		// Left leaf
		if(rand() % 100 < int(100.0 * config.mutation_rate)) {
			branch.leaf_lhs = random_leaf();
		}
	} else {
		// Left branch
		if(rand() % 100 < int(100.0 * config.mutation_rate)) {
			recursive_sub_tree_mutation(*branch.left_branch);
		}
	}

	// Base case, right branch
	if(branch.right_branch == nullptr) {
		// Right leaf
		if(op != "NOT" && rand() % 100 < int(100.0 * config.mutation_rate)) {
			branch.leaf_rhs = random_leaf();
		}
	} else {
		// Right branch
		if(rand() % 100 < int(100.0 * config.mutation_rate)) {
			recursive_sub_tree_mutation(*branch.right_branch);
		}
	}

	return;
}


deque<Move> generate_move_queue() {
	// Generate sequence of past moves;
	deque<Move> m;
	make_past_moves(Prisoner::config.agent_memory_length, m);

	return m;
}

bool Prisoner::less_than(const Prisoner& rhs) const {
	return fitness < rhs.fitness;
}

bool Prisoner::operator <(const Prisoner& rhs) {
	return fitness < rhs.fitness;
}

bool operator <(const Prisoner& a, const Prisoner& b) {
	return a.fitness < b.fitness;
}

bool operator >(const Prisoner& a, const Prisoner& b) {
	return a.fitness > b.fitness;
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
