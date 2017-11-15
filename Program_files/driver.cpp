#include <iostream>
#include <memory>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "config.cpp"
#include "helper.cpp"
#include "prisoner.cpp"

using namespace std;

// Read in config file
Config Prisoner::config;

int main() {
	// Initalize random seed
	srand (time(NULL));

	for(int i = 0; i < Prisoner::config.runs; i++) {
		pg_run(i);
	}

	return 0;
}