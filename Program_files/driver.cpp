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

	Prisoner a;
	a.randomly_initalize_tree();
	cout << a.op << endl;
	cout << a << endl;

	Prisoner b;
	b = a;
	cout << a << endl;
	cout << b << endl;

	return 0;
}