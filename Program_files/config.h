#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Config {
	public:
		int random_seed;
		int iterations;
		int agent_memory_length;
		int max_depth;
		int runs;
		string log_file_path;
		string solution_file_path;
		int fitness_evaluations;

		// Read in parameters from a file
		Config();
		void set_configs(string & key, string & value);
		void set_logs();
};

#endif