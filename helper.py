from prisoner import *
import json
import sys
from time import time
import multiprocessing
import os
import random
from progressBar import ProgressBar


############################## Generic Helpers ##############################
def config_parser(file_name):
	json_config_file = {}

	with open(file_name, 'r') as f:
		json_config_file = json.load(f)

	config_dict = {}

	# Save Configuration file inside config_dict
	config_dict['Config File'] = json_config_file

	# Random Seed
	config_parser_helper(config_dict, 'random_seed', 'Random Seed', json_config_file, int(time()))

	# l
	config_parser_helper(config_dict, 'l', 'l', json_config_file, 5)

	# k
	config_parser_helper(config_dict, 'k', 'k', json_config_file, 5)

	# Max depth d
	config_parser_helper(config_dict, 'd', 'd', json_config_file, 5)

	# Runs
	config_parser_helper(config_dict, 'runs', 'Runs', json_config_file, 1)

	# Log file path
	config_parser_helper(config_dict, 'log_file_path', 'Log File Path', json_config_file, './logs/' + str(config_dict['random_seed']))

	# Solution file path
	config_parser_helper(config_dict, 'solution_file_path', 'Solution File Path', json_config_file, './solutions/' + str(config_dict['random_seed']))

	# Fitness Evaluations
	config_parser_helper(config_dict, 'fitness_evaluations', 'Fitness Evaluations', json_config_file, 10000)

	return config_dict

def config_parser_helper(config_dict, key_string, json_string, json_config_file, default_value):
	"""Helps config_parser by adding an element into config_dict

	Args:
		config_dict (dict {str: val}): Dictionary of configuration values
		key_string (str): String of key value in dictionary
		json_string (str): JSON term corresponding to the key_string
		json_config_file (dict {str: val}): JSON dictionary
		default_value (any): Value for config_dict[key_string] if null in
			json_config_file[json_string]

	Returns:
		Nothing. Modifies config_dict by adding key:value 
	"""
	try:
		if json_config_file[json_string] != '' and json_config_file[json_string] != None:
			config_dict[key_string] = json_config_file[json_string]
		else:
			config_dict[key_string] = default_value
	except:
		config_dict[key_string] = default_value
		print("You need {'", end='')
		print(json_string, end='')
		print("': null} in your JSON File! I'll let  you go this", end='')
		print(" time by using ", end='')
		print(config_dict[key_string], end='')
		print(", but never again will I be this kind!")

def open_file(file):
	"""Open a file and create sub-directories as needed

	Returns:
		(_io.TextIOWrapper) Open file handle
	"""
	# If path to file does not exist, make it
	if not os.path.exists(os.path.dirname(file)):
		os.makedirs(os.path.dirname(file))
	# Return opened file
	return open(file, 'w')

def create_log_file(config_dict):
	"""Create log file
	Create a log file in the config_dict['log_file_path'] file.

	The log file has the following format:
		{
			"Problem Instance Files": "../solution/1234567890.txt",
			"Random Seed": "1234567890",
			"Config File": {
				"Random Seed": 1234567890,
				"Search Algorithm": "Random Search",
				"Runs": 20,
				"Fitness Evaluations": 20,
				"Log File Path": "./logs/1234567890.log",
				"Solution File Path": "./solution/1234567890.txt"
			}
			"Algorithm Log File Path:": "../algorithm_log/1234567890.txt"
		}

	"""
	log_dict = {}
	log_dict['Problem Instance Files'] = config_dict['solution_file_path']
	log_dict['Random Seed'] = config_dict['random_seed']
	log_dict['Config File'] = config_dict['Config File']
	log_dict['Run Time'] = config_dict['run_time']

	config_dict.pop('input_file', None)
	config_dict.pop('run_time', None)

	# Open file, create directories if needed
	opened_file = open_file(config_dict['log_file_path'])
	# Dump JSON to file in alphabetical order with pretty formatting
	json.dump(log_dict, opened_file, indent=4, sort_keys=True)
	# Close opened file
	opened_file.close()

def write_algorithm_log(file, runs, return_dict, config_dict):
	"""Write evals to algorithm log file, creating sub-directory if needed
	Writes algorithm log's \nin the following format:
		<run number><tab><fitness function value>
	These lines are only written if the fitness function value improves
	
	Args:
		file (str): File to open, including directories
		runs (int): The number of runs completed
		returning_dict (dict {int: str}): Dict to write file to
		config_dict (dict {str: value}): Configuration dictionary
	"""
	if not os.path.exists(os.path.dirname(file)):
		os.makedirs(os.path.dirname(file))

	with open(file, 'a') as f:
		#f.write('\n\n' + add_to_algorithm_log_string([], config_dict, header=True))
		f.write("\nResult Log\n")
		for i in range(runs):
			f.write(return_dict[i][0])
		# Add extra new line for parsing means
		f.write('\n')

def create_solution_file(best_board, path, run_number):
	"""Create solution file using the best board from the run

	Use the best board from a run to create a solution file. The solution file
	is formated in the following form:
		x,y,rotation
	One line is printed for each shape in the same order as they were
	originally given.

	Args:
		best_board (Board): Best board from a run
		path (str): Solution file path
		run_number (int): The run number, used to name the file

	"""
	# Open file, create directories if needed
	opened_file = open_file(path)

	# Write x,y,rotation for each shape
	for solution in best_board.get_info():
		opened_file.write(solution + '\n')

	# Close file
	opened_file.close()

def create_solution_file(best_board, path, run_number):
	"""Create solution file using the best board from the run

	Use the best board from a run to create a solution file. The solution file
	is formated in the following form:
		x,y,rotation
	One line is printed for each shape in the same order as they were
	originally given.

	Args:
		best_board (Board): Best board from a run
		path (str): Solution file path
		run_number (int): The run number, used to name the file

	"""
	# Open file, create directories if needed
	opened_file = open_file(path)

	opened_file.write("{}\n".format(best_board))

	# Close file
	opened_file.close()


############################## Algorithm Helpers ##############################
def sequence_generator(k):
	sequence = []
	options = [0, 1]

	while len(sequence) < k:
		sequence.append([choice(options), choice(options)])

	return sequence

def opponent_move_generator(l):	
	sequence = []
	options = [0, 1]

	while len(sequence) < l:
		sequence.append(choice(options))

	return sequence

def tic_for_tat(last_move, oppontent_moves):
	count = 0
	fitness = 0

	fitness += fitness_value(last_move, oppontent_moves[0])
	count += 1

	for i in range(1, len(oppontent_moves)):
		fitness += fitness_value(oppontent_moves[i-1], oppontent_moves[i])

	return fitness

def random_search(config_dict, run_number, return_dict):
	# Seed the random number generator with random_seed + run number
	# This makes each run unique
	random.seed(config_dict['random_seed'] + run_number)

	# Generate random sequence of moves
	sequence = sequence_generator(config_dict['k'])

	# Generate random opponent moves
	oppontent_moves = opponent_move_generator(config_dict['l'])

	# Define base_fitness as  tic-for-tat fitness
	base_fitness = tic_for_tat(sequence[-1][1], oppontent_moves)

	# Set max depth and k for Prioner class
	Prisoner.max_depth = config_dict['d']
	Prisoner.k = config_dict['k']

	# Initialize best prisoner and best prisoner fitness
	best_prisoner = 0
	best_fitness = -9999999999999

	# Initalize log file string
	log_file_string = '\nRun ' + str(run_number + 1) + '\n'

	### BEGIN Progress Bar Set Up ###
	# If last run, print progress bar
	print_progress_bar = False
	if run_number + 1 == config_dict['runs']:
		print_progress_bar = True

	progress_bar = 0
	if print_progress_bar:
		progress_bar = ProgressBar(config_dict['fitness_evaluations'])
		progress_bar.printProgressBar(0)
	### END Progress Bar Set Up ###

	for current_eval in range(config_dict['fitness_evaluations']):
		if print_progress_bar:
			progress_bar.printProgressBar(current_eval)
		
		prisoner = Prisoner()
		fitness = prisoner.find_fitness(config_dict['k'], config_dict['l'], sequence, oppontent_moves) - base_fitness
		if fitness > best_fitness:
			best_fitness = fitness
			best_prisoner = prisoner
			log_file_string += '{} {}\n'.format(current_eval, best_fitness)

	return_dict[run_number] = (log_file_string, best_prisoner)
		
