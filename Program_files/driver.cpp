#include <iostream>
#include <memory>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <deque>
#include "config.cpp"
#include "helper.cpp"
#include "prisoner.cpp"

using namespace std;

// Read in config file
Config Prisoner::config;
// Generate move queue (for a fair test)
deque<Move> Prisoner::move_queue = generate_move_queue();

int main() {
	vector<Prisoner> best_population;
	Prisoner best_prisoner;
	string current_log;
	string absolute_fitness_log;
	vector<string> overall_log;
	vector<string> overall_absolute_fitness_log;

	// For each run
	for(int i = 0; i < Prisoner::config.runs; i++) {
		cout << "Running run " << i + 1 << " of " << Prisoner::config.runs << flush << "\r";
		pg_run(i, best_prisoner, current_log, absolute_fitness_log);

		// Add best in population to vector
		best_population.push_back(best_prisoner);
		// Add log
		overall_log.push_back("\nRun " + to_string(i+1) + "\n" + current_log);
		// Add absolute log
		overall_absolute_fitness_log.push_back("\nRun " + to_string(i+1) + "\n" + absolute_fitness_log);
	}
	cout << endl;

	// Create solution and log files
	make_solution_file(best_population);
	make_log_file(overall_log);
	make_absolute_fitness_log_file(overall_absolute_fitness_log);

	return 0;
}