#include "config.h"

// Read in parameters from a file
Config::Config() {
	string key, value;

	// Initalize defaults (not logs)
	random_seed = 0;
	iterations = 5;
	agent_memory_length = 5;
	max_depth = 5;
	runs = 1;
	fitness_evaluations = 10000;
	log_file_path = "";
	solution_file_path = "";
	population_size = 5;
	children_count = 5;
	parent_selection = "Fitness_Proportional_Selection";
	survival_selection = "Truncation";
	parsimony_pressure = 0.5;
	termination_convergence = 10000;
	over_selection = 0.2;
	mutation_rate = 0.05;

	// Open config file
	ifstream file("../default.cfg");

	// Make sure file is open
	if(file.is_open()) {
		// While there are vales
		while(file >> key >> value) {
			set_configs(key, value);
		}
	} else {
		throw "Unable to open default.cfg";
	}

	// Set log defaults if not already set
	set_logs();
}

void Config::set_configs(string & key, string & value) {
	if(value == "null") {
		return;
	}

	if(key == "Random_Seed") {
		random_seed = stoi(value);
	} else if(key == "l") {
		iterations = stoi(value);
	} else if(key == "k") {
		agent_memory_length = stoi(value);
	} else if(key == "d") {
		max_depth = stoi(value);
	} else if(key == "Runs") {
		runs = stoi(value);
	} else if(key == "Fitness_Evaluations") {
		fitness_evaluations = stoi(value);
	} else if(key == "Log_File_Path") {
		log_file_path = value;
	} else if(key == "Solution_File_Path") {
		solution_file_path = value;
	} else if(key == "Population_Size") {
		population_size = stoi(value);
	} else if(key == "Children_Count") {
		children_count = stoi(value);
	} else if(key == "Parent_Selection") {
		parent_selection = value;
	} else if(key == "Survival_Selection") {
		survival_selection = value;
	} else if(key == "Parsimony_Pressure_Penalty_Coefficent") {
		parsimony_pressure = stof(value);
	} else if(key == "Termination_Convergence_Criterion") {
		termination_convergence = stoi(value);
	} else if(key == "Over_Selection") {
		over_selection = stof(value);
	} else if(key == "Mutation_Rate") {
		mutation_rate = stof(value);
	}

	return;
}

void Config::set_logs() {
	if(log_file_path == "") {
		log_file_path = "logs/" + to_string(random_seed);
	}
	if(solution_file_path == "") {
		solution_file_path = "solutions/" + to_string(random_seed);
	}

	return;
}