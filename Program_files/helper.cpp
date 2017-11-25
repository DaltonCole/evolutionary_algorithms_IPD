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

	// Set up hall-of-fame
	deque<Prisoner> hall_of_fame;

	for(int i = 0; i < Prisoner::config.fitness_evaluations; i += population.size()) {
		// Print progress
		cout << "Running run " << run_number + 1 << " of " << Prisoner::config.runs << 
			"\t" << "Fitness Evals: " << i << " / " << Prisoner::config.fitness_evaluations
		 	<< "                 " << flush << "\r";


		// Find parents
		find_parents(population, parents);

		// Make children
		make_children(parents, children);
		// Mutate children
		mutate_children(children);

		// Apply Survival Selection Strategy
		if(Prisoner::config.survival_selection_strategy == "Plus") {
			// Combine population and children
			population.insert(population.end(), children.begin(), children.end());
		} else { // Comma
			// Set population to be children
			population = children;
		}

		// Assign fitness and update fitness value
		for(int j = 0; j < population.size(); j++) {
			population[j].coevolutionary_assign_fitness(population, i, j);
		}

		// Survival Selection
		survival_selection(population);

		sort(population.begin(), population.end(), greater<Prisoner>());

		// Implement hall of fame
		inforce_hall_of_fame(population, hall_of_fame);

		// Set up log_string
		log_string += to_string(i + population.size());
		log_string += " \t" + to_string(find_average_fitness(population));
		log_string += " \t" + to_string(population[0].get_fitness()) + "\n";
		
		// If converging, quit
		if(population[0].get_fitness() == previous_fitness) {
			same_fitness_streak++;
			if(same_fitness_streak == Prisoner::config.termination_convergence) {
				break;
			}
		} else {
			previous_fitness = population[0].get_fitness();
			same_fitness_streak = 0;
		}
	}

	best_prisoner = population[0];

}

// Make moves for move queue
void make_past_moves(const int number_of_moves, deque<Move>& move_queue) {
	Move m;

	while(move_queue.size() != number_of_moves) {
		m.random_move();
		move_queue.push_back(m);
	}

	return;
}

// Apply tic for tac method
void append_tic_for_tac(deque<Move>& move_queue, const bool p_move) {
	Move m;
	m.p = p_move;
	m.o = move_queue.front().o;
	move_queue.pop_front();
	move_queue.push_back(m);

	return;
}

// Find parents using method
void find_parents(vector<Prisoner>& population, vector<Prisoner*>& parents) {
	if(Prisoner::config.parent_selection == "Fitness_Proportional_Selection") {
		fitness_proportional_selection(population, parents);
	} else if(Prisoner::config.parent_selection == "Over_Selection") {
		over_selection(population, parents);
	}
}

// Apply fitness proportional selection
void fitness_proportional_selection(vector<Prisoner>& population, vector<Prisoner*>& parents) {
	// Clear parent population
	parents.clear();
	float total_fitness = 0;
	float tracker = 0;
	int random_number;
	int minimum;

	// Find total fitness
	for(const auto& p : population) {
		int temp = p.get_fitness() + 1;
		total_fitness += abs(temp);
		if(temp < minimum) {
			minimum = temp;
		}
	}

	// While there are more parents to add
	while(parents.size() < Prisoner::config.children_count) {
		// Reset tracker
		tracker = 0;
		// Pick a random number between 0 and total fitness
		random_number = rand() % max(int(ceil(total_fitness)), 1);
		
		// When tracker is greater than random number, add that
		// prisoner to the parent population
		for(auto& p : population) {
			tracker += (abs(p.get_fitness()) + minimum + 1);
			if(tracker >= random_number) {
				parents.push_back(&p);
			}
		}
	}

	return;
}

// Apply over selection
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

// Make children using sub tree crossover
void make_children(vector<Prisoner*>& parents, vector<Prisoner>& children) {
	children.clear();
	sub_tree_crossover(parents, children);

	return;
}

// Apply sub tree crossover
void sub_tree_crossover(vector<Prisoner*>& parents, vector<Prisoner>& children) {
	Prisoner a;
	Prisoner b;
	while(children.size() < Prisoner::config.children_count) {
		// Randomly select two parents
		a = *parents[rand() % parents.size()];
		b = *parents[rand() % parents.size()];

		// Do cross over
		a.sub_tree_crossover(b);

		// Add children to children vector
		children.push_back(a);
		children.push_back(b);
	}
}

// Mutate children using sub tree mutation
void mutate_children(vector<Prisoner>& children) {
	for(auto& c : children) {
		c.sub_tree_mutation();
	}

	return;
}

// Apply survival selection algorithm
void survival_selection(vector<Prisoner>& population) {
	if(Prisoner::config.survival_selection == "Truncation") {
		truncation(population);
	} else if(Prisoner::config.survival_selection == "K_Tournament_No_Replacement") {
		k_tournament_selection_without_replacement(population);
	}

	return;
}

// Apply truncation to population
void truncation(vector<Prisoner>& population) {
	sort(population.begin(), population.end(), greater<Prisoner>());
	while(population.size() > Prisoner::config.population_size) {
		population.pop_back();
	}

	return;
}

// Apply k tournament selection without replacement
void k_tournament_selection_without_replacement(vector<Prisoner>& population) {
	vector<Prisoner> new_population;
	vector<int> random_numbers;
	int best;

	while(new_population.size() < Prisoner::config.population_size) {
		// Select up to k prisoners
		while(random_numbers.size() < Prisoner::config.k_tournament_no_replacement) {
			random_numbers.push_back(rand() % population.size());
		}
		// Find the best prisoner in the population
		find_best_in_tournament(population, random_numbers, best);
		// Add best prisoner to population
		new_population.push_back(population[best]);
		// Remote prisoner from vector (expensive)
		population.erase(population.begin() + best);
	}

	// Set population equal to new population
	population = new_population;

	return;
}

// Find the prisoner with the highest fitness given their indexes in random_numbers
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

// Find average fitness
float find_average_fitness(const vector<Prisoner>& population) {
	float average = 0;
	for(auto& p : population) {
		average += p.get_fitness();
	}

	return average / population.size();
}

// BONUS 1 AND 2
// Do hall-of-fame and detect cycling
void inforce_hall_of_fame(vector<Prisoner>& population, deque<Prisoner>& hall_of_fame) {
	while(true) {
		// Item is not in the hall of fame or is the last item
		if(find(hall_of_fame.begin(), hall_of_fame.end(), population[0]) == hall_of_fame.end()) {
			hall_of_fame.push_back(population[0]);
			break;
		} else if(hall_of_fame.back() == population[0]) {
			break;
		} else {
			cout << "cycling has occured, removing first element, adding random element" << endl;
			population[0].randomly_initalize_tree();
		}
	}
}