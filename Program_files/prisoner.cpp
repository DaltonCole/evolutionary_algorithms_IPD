#include "prisoner.h"

// Default Constructor
Prisoner::Prisoner() {
	// Set each leaf to 0
	leaf_lhs = 0;
	leaf_rhs = 0;
	// Empty operator
	op = "";
	// No Depth
	current_depth = 0;
	// No Fitness
	fitness = 0;
}

// Copy constructor
Prisoner::Prisoner(const Prisoner & rhs) {
	// Set leaf and operator
	op = rhs.op;
	leaf_lhs = rhs.leaf_lhs;
	leaf_rhs = rhs.leaf_rhs;

	// Recursively call left branch constructor
	if(rhs.left_branch == nullptr) {
		left_branch = nullptr;
	} else {
		left_branch = unique_ptr<Prisoner> (new Prisoner(*rhs.left_branch));
	}

	// Recursively call right branch constructor
	if(rhs.right_branch == nullptr) {
		right_branch = nullptr;
	} else {
		right_branch = unique_ptr<Prisoner> (new Prisoner(*rhs.right_branch));
	}

	// Same fitness and depth
	fitness = rhs.fitness;
	current_depth = rhs.current_depth;
}

// Operator =
const Prisoner & Prisoner::operator =(const Prisoner & rhs) {
	// If not the same object
	if(this != &rhs) {
		// Copy contents
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

// Move constructor
Prisoner::Prisoner(Prisoner&& other) {	
	op = other.op;
	leaf_lhs = other.leaf_lhs;
	leaf_rhs = other.leaf_rhs;
	current_depth = other.current_depth;
	fitness = other.fitness;

	// Swap branches to this object
	left_branch.swap(other.left_branch);
	right_branch.swap(other.right_branch);
}

// Move equal
Prisoner& Prisoner::operator =(Prisoner&& other) {
	op = other.op;
	leaf_lhs = other.leaf_lhs;
	leaf_rhs = other.leaf_rhs;
	current_depth = other.current_depth;
	fitness = other.fitness;

	// Swap branches to this object
	left_branch.swap(other.left_branch);
	right_branch.swap(other.right_branch);

	return *this;
}

bool Prisoner::operator ==(const Prisoner& rhs) const {
	if(to_string(*this) == to_string(rhs)) {
		return true;
	} else {
		return false;
	}
}

// ramped half-and-half: 
void Prisoner::randomly_initalize_tree() {
	// Full method
	if(rand() % 2 == 0) {
		*this = *generate_full_tree(1);
	} else { // Grow method

		op = random_operator();
		left_branch = generate_branch(1);

		// If empty branch, initialize leaf
		if(left_branch == nullptr) {
			leaf_lhs = random_leaf();
			current_depth = 1;
		} else {
			// Increment depth
			current_depth = left_branch -> current_depth + 1;
		}
		if(op != "NOT") {
			// Go down right branch if not NOT
			right_branch = generate_branch(1);

			// If empty branch, initialize right leaf
			if(right_branch == nullptr) {
				leaf_rhs = random_leaf();
			} else {
				// Make depth max between left branch and right branch + 1
				current_depth = max(current_depth, right_branch -> current_depth + 1);
			}
		}
	}

	return;
}

// Make full tree
unique_ptr<Prisoner> Prisoner::generate_full_tree(const int depth) const {
	// The new branch
	unique_ptr<Prisoner> new_branch(new Prisoner);

	// Base case
	if(depth == config.max_depth) {
		new_branch -> op = random_operator();
		new_branch -> leaf_lhs = random_leaf();
		if(op != "NOT") {
			new_branch -> leaf_rhs = random_leaf();
		}
	} else {
		new_branch -> op = random_operator();
		new_branch -> left_branch = generate_full_tree(depth + 1);
		if(op != "NOT") {
			new_branch -> right_branch = generate_full_tree(depth + 1);
		}
	}

	return new_branch;
}

// Recursively make branches
unique_ptr<Prisoner> Prisoner::generate_branch(int depth) const {
	// The new branch
	unique_ptr<Prisoner> new_branch(new Prisoner);

	// Random chance of increasing depth
	if((config.max_depth - depth) != 0 && rand() % (config.max_depth - depth) >= 1) {
		// Recursively make branch after setting operator
		new_branch -> op = random_operator();

		// Randomly either go down left branch, or make leaf
		if(rand() % (config.max_depth - depth) >= 1) {
			// Generate branch
			new_branch -> left_branch = generate_branch(depth + 1);
			// Set depth
			new_branch -> current_depth = (new_branch -> left_branch -> current_depth) + 1;
		} else {
			// Set branch to null and randomly initalize left leaf
			new_branch -> left_branch = nullptr;
			new_branch -> leaf_lhs = random_leaf();
		}

		// If two operands are required
		if(new_branch -> op != "NOT") {
			// Randomly go down  right branch, or make leaf
			if(rand() % (config.max_depth - depth) >= 1) {
				// Generate branch
				new_branch -> right_branch = generate_branch(depth + 1);
				// Set depth
				if(new_branch -> current_depth < (new_branch -> right_branch -> current_depth) + 1) {
					new_branch -> current_depth = (new_branch -> right_branch -> current_depth) + 1;
				}
			} else {
				// Set branch to null and randomly initalize right leaf
				new_branch -> right_branch = nullptr;
				new_branch -> leaf_rhs = random_leaf();
			}
		}
	} else {
		// Base case, make leaves
		new_branch -> left_branch = nullptr;
		new_branch -> right_branch = nullptr;
		new_branch -> op = random_operator();
		new_branch -> leaf_lhs = random_leaf();
		new_branch -> current_depth = 1;

		// Make other leaf if requireing two operands
		if(new_branch -> op != "NOT") {
			new_branch -> leaf_rhs = random_leaf();
		}
	}

	return new_branch;
}

// Pick a random operator
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

// Pick a random leaf 
int Prisoner::random_leaf() const {
	int r = rand() % (config.agent_memory_length) + 1;

	if(rand() % 2 == 1) {
		return r;
	} else {
		return -r;
	}
}

// Find fitness value
void Prisoner::assign_fitness() {
	// Set current queue to the global queue
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
	//fitness -= (config.parsimony_pressure * current_depth);

	return;
}

// Find fitness in a coevolutionary manner
void Prisoner::coevolutionary_assign_fitness(vector<Prisoner>& population, int& fintess_evaulations, const int bad_index) {
	// Set current queue to the global queue
	current_move_queue = move_queue;

	int times_to_compare = min(config.coevolutionary_fitness_sampling_percentage, int(population.size() - 1));

	// Vector to hold the selected prisoners for comparison
	vector<Prisoner*> prisoners_to_compare;

	// Possible indexes to add to prisoners to compare vector
	deque<int> indexes;

	// Add possible indexes to indexes
	for(int i = 0; i < population.size(); i++) {
		if(i != bad_index) {
			indexes.push_back(i);
		}
	}

	// Shuffle indexes
	random_shuffle(indexes.begin(), indexes.end());

	// Add elements to prisoners to compare vector
	while(times_to_compare != prisoners_to_compare.size()) {
		prisoners_to_compare.push_back(&population[indexes.front()]);
		indexes.pop_front();
	}

	// For each fighting prisoner
	for(auto& p : prisoners_to_compare) {
		// Assign base queue to opponent
		p -> current_move_queue = move_queue;

		// Play 2k rounds before keeping socre
		for(int i = 0; i < 2 * config.agent_memory_length; i++) {
			append_move_to_queues(p);
		}

		// Play l - 2k move moves, while keeping track of score
		for(int i = 2 * config.agent_memory_length; i < config.iterations; i++) {
			fitness += append_move_to_queues(p);
		}
	}

	fitness /= ((config.iterations - (2 * config.agent_memory_length)) * prisoners_to_compare.size());

	fintess_evaulations += prisoners_to_compare.size();
}

// Compute both prisoner's and opponent's move and append to respecive queues
int Prisoner::append_move_to_queues(Prisoner* p) {
	bool my_move = find_value();
	bool opp_move = p -> find_value();

	// Append my move
	Move mine;
	mine.p = my_move;
	mine.o = opp_move;
	current_move_queue.pop_front();
	current_move_queue.push_back(mine);

	// Append opponent's move
	Move opponent;
	opponent.p = opp_move;
	opponent.o = my_move;
	p -> current_move_queue.pop_front();
	p -> current_move_queue.push_back(opponent);

	return fitness_function(my_move, opp_move);
}

// Helper function to find boolean value (move to make)
bool Prisoner::find_value() const {
	return recursively_find_value(*this);
}

// Recursively find what move to make
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

// Get move value out of current move queue
bool Prisoner::get_move_value(const int m) const {
	if(m > 0) {
		return current_move_queue[m - 1].p;
	} else {
		return current_move_queue[-m - 1].o;
	}
} 

// Define value for each move
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

// Getter for fitness
float Prisoner::get_fitness() const {
	return fitness;
}

// Find depth of tree
int Prisoner::assign_depth(Prisoner& p) {
	// If neither side is null
	if(p.left_branch != nullptr && p.right_branch != nullptr) {
		p.current_depth = max(assign_depth(*p.left_branch), assign_depth(*p.right_branch)) + 1;
	} else if(p.left_branch != nullptr) {
		p.current_depth = assign_depth(*p.left_branch) + 1;
	} else if(p.right_branch != nullptr) {
		p.current_depth = assign_depth(*p.right_branch) + 1;
	} else {
		p.current_depth = 1;
	}

	return p.current_depth;
}

// Apply recombination to two trees (changes both *this and other)
void Prisoner::sub_tree_crossover(Prisoner& other) {
	// Assign depths
	assign_depth(*this);
	assign_depth(other);

	// Take min value of current depth
	int random_num = rand() % min(current_depth, other.current_depth) + 1;

	// Find branches of equal height
	Prisoner* lhs = equal_level_branch(*this, random_num);
	Prisoner* rhs = equal_level_branch(other, random_num);

	swap(*lhs, *rhs);

	return;
}

// Finds branch of equal level to switch with other tree (so depth limit is kept)
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

// Helper function to mutate tree
void Prisoner::sub_tree_mutation() {
	recursive_sub_tree_mutation(*this);

	return;
}

// Recursively mutates the tree
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

string leaf_to_string(const int leaf) {
	if(leaf > 0) {
		return "P" + to_string(leaf);
	} else {
		return "O" + to_string(-leaf);
	}
}

ostream& operator <<(ostream& os, Prisoner p) {
	os << p.op << " ";
	if(p.left_branch != nullptr) {
		os << (*p.left_branch);
	} else {
		os << leaf_to_string(p.leaf_lhs) << " ";
	}

	if(p.right_branch != nullptr) {
		os << (*p.right_branch);
	} else if(p.op != "NOT"){
		os << leaf_to_string(p.leaf_rhs) << " ";
	}

	return os;
}

string to_string(const Prisoner& p) {
	string s = p.op + " ";
	if(p.left_branch != nullptr) {
		s += to_string(*p.left_branch);
	} else {
		s += (p.leaf_lhs + " ");
	}

	if(p.right_branch != nullptr) {
		s += to_string(*p.right_branch);
	} else {
		s += (p.leaf_rhs + " ");
	}

	return s;
}