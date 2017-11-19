#include "helper.h"

void pg_run(const int run_number) {
	// Generate sequence of past moves;
	deque<Move> move_queue;
	make_past_moves(Prisoner::config.agent_memory_length, move_queue);

	// Make population
	vector<Prisoner> population(Prisoner::config.population_size);

	// Add past moves to population, generate tree, assign fitnesses
	for(auto& p : population) {
		p.set_move_queue(move_queue);
		p.randomly_initalize_tree();
		p.assign_fitness();
	}

	/*
	for(auto& p : population) {
		cout << p << endl;
		cout << (p.fitness) << endl;
	}
	*/	

	vector<Prisoner*> parents;
	vector<Prisoner> children;

	for(int i = 0; i < Prisoner::config.fitness_evaluations; i += population.size()) {
		// Find parents
		find_parents(population, parents);

		// Make children
		make_children(parents, children);
		// Mutate children
		cout << "Children Population Before Mutation" << endl;
		for(auto& p : children) {
			p.assign_fitness();
			cout << p.get_fitness() << endl;
		}

		mutate_children(children);
		// Find fitnesses for children
		for(auto& c : children) {
			c.assign_fitness();
		}
		cout << "Orignal Population:" << endl;
		for(auto& p : population) {
			cout << p.get_fitness() << endl;
		}
		
		cout << "Children Population" << endl;
		for(auto& p : children) {
			cout << p.get_fitness() << endl;
		}
		

		// Combine population and children
		population.insert(population.end(), children.begin(), children.end());

		cout << "Combined Population" << endl;
		for(auto& p : population) {
			cout << p.get_fitness() << endl;
		}

		// Survival Selection
		survival_selection(population);

		sort(population.begin(), population.end(), less<Prisoner>());
		while(population.size() > Prisoner::config.population_size) {
			population.pop_back();
		}
	}
}

void make_past_moves(const int number_of_moves, deque<Move>& move_queue) {
	Move m;

	while(move_queue.size() != number_of_moves) {
		m.random_move();
		move_queue.push_back(m);
	}

	return;
}

void append_tic_for_tac(deque<Move>& move_queue, const bool p_move) {
	Move m;
	m.p = p_move;
	m.o = move_queue.front().o;
	move_queue.pop_front();
	move_queue.push_back(m);

	return;
}

void find_parents(vector<Prisoner>& population, vector<Prisoner*>& parents) {
	if(Prisoner::config.parent_selection == "Fitness_Proportional_Selection") {
		fitness_proportional_selection(population, parents);
	} else if(Prisoner::config.parent_selection == "Over_Selection") {
		over_selection(population, parents);
	}
}

void fitness_proportional_selection(vector<Prisoner>& population, vector<Prisoner*>& parents) {
	// Clear parent population
	parents.clear();
	float total_fitness = 0;
	float tracker = 0;
	int random_number;

	for(const auto& p : population) {
		total_fitness += p.get_fitness();
	}

	while(parents.size() < Prisoner::config.children_count) {
		tracker = 0;
		
		random_number = rand() % int(ceil(total_fitness));
		
		for(auto& p : population) {
			tracker += p.get_fitness();
			if(tracker >= random_number) {
				parents.push_back(&p);
			}
		}
	}

	return;
}

void over_selection(vector<Prisoner>& population, vector<Prisoner*>& parents) {
	parents.clear();

	int choosen_pop;

	// Sort population
	sort(population.begin(), population.end(), less<Prisoner>());

	// Populate 80% of parents with the top x percent of population
	while(parents.size() < (float(Prisoner::config.children_count) * 0.8)) {
		choosen_pop = (rand() % int(population.size() * Prisoner::config.over_selection)) + 
			int(population.size() * (1.0 - Prisoner::config.over_selection));

		parents.push_back(&population[choosen_pop]);
	}
	// Populate 20% of parents with the bottom x percent of population
	while(parents.size() <= (float(Prisoner::config.children_count) * 0.8)) {
		choosen_pop = (rand() % int(population.size() * (1- Prisoner::config.over_selection)));

		parents.push_back(&population[choosen_pop]);
	}

	return;
}

void make_children(vector<Prisoner*>& parents, vector<Prisoner>& children) {
	cout << "PARENT FITNESS" << endl;
	for(auto& p : parents) {
		cout << p -> get_fitness() << endl;
	}
	children.clear();
	sub_tree_crossover(parents, children);

	return;
}

void sub_tree_crossover(vector<Prisoner*>& parents, vector<Prisoner>& children) {
	Prisoner a;
	Prisoner b;
	while(children.size() < Prisoner::config.children_count) {
		// Randomly select two parents
		a = *parents[rand() % parents.size()];
		b = *parents[rand() % parents.size()];

		a.sub_tree_crossover(b);

		children.push_back(a);
		children.push_back(b);
	}
}

void mutate_children(vector<Prisoner>& children) {
	for(auto& c : children) {
		c.sub_tree_mutation();
	}

	return;
}

void survival_selection(vector<Prisoner>& population) {
	if(Prisoner::config.survival_selection == "Truncation") {
		truncation(population);
	}

	return;
}

void truncation(vector<Prisoner>& population) {
	sort(population.begin(), population.end(), greater<Prisoner>());
	///*
	cout << "Sorted Population" << endl;
	for(auto& a: population) {
		cout << a.get_fitness() << endl;
	}
	cout << "-" << endl;
	//*/
	while(population.size() > Prisoner::config.population_size) {
		population.pop_back();
	}

	cout << "Purged Population" << endl;
	for(auto& a: population) {
		cout << a.get_fitness() << endl;
	}
	cout << endl;

	return;
}