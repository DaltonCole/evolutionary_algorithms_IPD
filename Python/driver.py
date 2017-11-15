# File name:      driver.py
# Author:         Dalton Cole

from helper import *

def main():
	# If we do not recieve 2 arguments, exit
	if len(sys.argv) != 2:
		print('Usage: python3 driver.py <config file>')
		quit()

	# Start timer
	start_time = int(time())

	# Dictionary from the config file
	config_dict = config_parser(sys.argv[1])

	# Multiprocess
	manager = multiprocessing.Manager()
	return_dict = manager.dict()
	jobs = []
	
	# Start each run as different processes. 
	# Last run should display the progress bar.
	for run in range(config_dict['runs']):
		p = multiprocessing.Process(target=random_search, args=(config_dict, run, return_dict))
		jobs.append(p)
		p.start()

	# Wait for jobs to finish
	for proc in jobs:
		proc.join()
	print()

	# Add total run time to log file
	config_dict['run_time'] = int(time()) - start_time

	# Create log file
	create_log_file(config_dict)

	# Write to algorithm log file
	write_algorithm_log(config_dict['log_file_path'], config_dict['runs'], return_dict, config_dict)

	# Find best solution
	best_solution = return_dict[0][1]
	best_index = 0
	for i in range(config_dict['runs']):
		if return_dict[i][1] > best_solution:
			best_solution = return_dict[i][1]
			best_index = i

	# Write to solution file best solution
	create_solution_file(return_dict[best_index][1], config_dict['solution_file_path'], best_index + 1)

	# Create tree grah and eval graph, store in ./graph/
	try:
		import graph
		graph.make_picture(best_solution, config_dict)
		graph.create_graph_best(config_dict, return_dict[best_index][0])
	except:
		print("Could not generate graph")



if __name__ == '__main__':
	main()

