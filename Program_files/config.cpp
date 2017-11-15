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