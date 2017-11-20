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
deque<Move> Prisoner::move_queue = generate_move_queue();

int main() {
	vector<Prisoner> best_population;
	Prisoner best_prisoner;
	string current_log;
	vector<string> overall_log;

	for(int i = 0; i < Prisoner::config.runs; i++) {
		cout << "Running run " << i + 1 << " of " << Prisoner::config.runs << flush << "\r";
		pg_run(i, best_prisoner, current_log);

		best_population.push_back(best_prisoner);
		overall_log.push_back("\nRun " + to_string(i+1) + "\n" + current_log);
	}
	cout << endl;

	make_solution_file(best_population);
	make_log_file(overall_log);

	return 0;
}