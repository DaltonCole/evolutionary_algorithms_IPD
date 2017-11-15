#include "helper.h"

void pg_run(const int run_number) {
	// Generate sequence of past moves;
	deque<Move> move_queue;
	make_past_moves(Prisoner::config.agent_memory_length, move_queue);

	// Make population
	vector<Prisoner> population(1);//Prisoner::config.population_size);

	// Add past moves to population, generate tree
	for(auto& p : population) {
		p.set_move_queue(move_queue);
		p.randomly_initalize_tree();
	}

	for(auto d : move_queue) {
		cout << d.p << " " << d.o << endl;
	}

	for(auto& p : population) {
		cout << p << endl;
		cout << (p.find_value()) << endl;
	}
	
	

	/*
	for(int i = 0; i < Prisoner::config.fitness_evaluations; i++) {

	}
	*/
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


/*
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
*/