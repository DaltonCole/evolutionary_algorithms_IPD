#include "helper.h"

void pg_run(const int run_number, Prisoner& best_prisoner, string& log_string) {

	// Make population
	vector<Prisoner> population(Prisoner::config.population_size);

	// Add past moves to population, generate tree, assign fitnesses
	for(auto& p : population) {
		p.randomly_initalize_tree();
		p.assign_fitness();
	}
	sort(population.begin(), population.end(), greater<Prisoner>());

	vector<Prisoner*> parents;
	vector<Prisoner> children;

	int previous_fitness = 0;
	int same_fitness_streak = 0;

	// Set up log_string
	log_string = "";

	for(int i = 0; i < Prisoner::config.fitness_evaluations; i += population.size()) {
		// Find parents
		//cout << "Find Parents" << endl;
		find_parents(population, parents);
		//for(auto& p : population) {
		//	cout << p << endl;
		//}

		// Make children
		//cout << "Make Children" << endl;
		make_children(parents, children);
		// Mutate children
		//cout << "Mutate Children" << endl;
		mutate_children(children);
		// Find fitnesses for children
		//cout << "Assign Fitness" << endl;
		for(auto& c : children) {
			c.assign_fitness();
			//cout << c << endl;
		}

		//cout << "Combine" << endl;
		// Combine population and children
		population.insert(population.end(), children.begin(), children.end());

		// Survival Selection
		//cout << "Survival Selection" << endl;
		survival_selection(population);

		sort(population.begin(), population.end(), greater<Prisoner>());

		// Set up log_string
		log_string += to_string(i + population.size());
		log_string += " \t" + to_string(find_average_fitness(population));
		log_string += " \t" + to_string(population[0].get_fitness()) + "\n";
		
		if(population[0].get_fitness() == previous_fitness) {
			same_fitness_streak++;
			if(same_fitness_streak == Prisoner::config.termination_convergence) {
				break;
			}
		} else {
			previous_fitness = population[0].get_fitness();
			same_fitness_streak = 0;
		}

		/*
		cout << "Population" << endl;
		for(auto& p : population) {
			cout << p << endl;
		}
		cout << endl;
		*/
	}

	best_prisoner = population[0];

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
	int minimum;

	for(const auto& p : population) {
		int temp = p.get_fitness() + 1;
		total_fitness += abs(temp);
		if(temp < minimum) {
			minimum = temp;
		}
	}

	while(parents.size() < Prisoner::config.children_count) {
		tracker = 0;
		random_number = rand() % max(int(ceil(total_fitness)), 1);
		
		for(auto& p : population) {
			tracker += (abs(p.get_fitness()) + minimum + 1);
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
	} else if(Prisoner::config.survival_selection == "K_Tournament_No_Replacement") {
		k_tournament_selection_without_replacement(population);
	}

	return;
}

void truncation(vector<Prisoner>& population) {
	sort(population.begin(), population.end(), greater<Prisoner>());
	while(population.size() > Prisoner::config.population_size) {
		population.pop_back();
	}

	return;
}

void k_tournament_selection_without_replacement(vector<Prisoner>& population) {
	vector<Prisoner> new_population;
	vector<int> random_numbers;
	int best;

	while(new_population.size() < Prisoner::config.population_size) {
		// Select up to k prisoners
		while(random_numbers.size() < Prisoner::config.k_tournament_no_replacement) {
			random_numbers.push_back(rand() % population.size());
		}
		find_best_in_tournament(population,random_numbers, best);
		new_population.push_back(population[best]);
		population.erase(population.begin() + best);
	}

	population = new_population;

	return;
}

void find_best_in_tournament(const vector<Prisoner>& population, const vector<int>& random_numbers, int& best) {
	best = random_numbers[0];

	for(auto& i : random_numbers) {
		if(population[i].get_fitness() > population[best].get_fitness()) {
			best = i;
		}
	}
	return;
}

// Make solution file
void make_solution_file(vector<Prisoner>& best_population) {
	Prisoner* best_prisoner = &best_population[0];

	for(auto& p : best_population) {
		if(p.get_fitness() > best_prisoner -> get_fitness()) {
			best_prisoner = &p;
		}
	}

	ofstream solution_file(Prisoner::config.solution_file_path);
	if(solution_file.is_open()) {
		solution_file << *best_prisoner << "\n";
		solution_file.close();
	} else {
		cout << "Error opening file: " << Prisoner::config.solution_file_path << endl;
	}

	return;
}

// Make log file
void make_log_file(vector<string>& log) {
	ofstream log_file(Prisoner::config.log_file_path);
	if(log_file.is_open()) {
		log_file << "Config File Used:\n" << endl;

		log_file << Prisoner::config << "\n";

		log_file << "Result Log\n";
		for(auto& l : log) {
			log_file << l;
		}

		log_file.close();
	} else {
		cout << "Error opening file: " << Prisoner::config.log_file_path << endl;
	}
}

float find_average_fitness(const vector<Prisoner>& population) {
	float average = 0;
	for(auto& p : population) {
		average += p.get_fitness();
	}

	return average / population.size();
}