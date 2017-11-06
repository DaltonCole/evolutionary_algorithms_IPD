import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from collections import OrderedDict

from graphviz import Digraph
from uuid import uuid4 as uu

def make_picture(best_solution, config_dict, yes_view=False):
	dot = Digraph(comment="Prison Tree")
	node = str(uu())
	dot.node(node, str(best_solution.tree[0]))
	recursive_dot(dot, best_solution.tree, node)
	dot.render('graph/tree/{}'.format(config_dict['random_seed']), view=yes_view)


def recursive_dot(dot, branch, node):
	# Base Case
	if type(branch[1]) == int:
		dot.node(node, branch[0])
		node1 = str(uu())
		dot.node(node1, dot_name(branch[1]))
		dot.edge(node, node1)
		if branch[0] != "NOT":
			node2 = str(uu())
			dot.node(node2, dot_name(branch[2]))
			dot.edge(node, node2)
	else:
		dot.node(node, branch[0])
		node1 = str(uu())
		dot.edge(node, node1)
		recursive_dot(dot, branch[1], node1)
		if branch[0] != "NOT":
			node2 = str(uu())
			dot.edge(node, node2)
			recursive_dot(dot, branch[2], node2)

			
def dot_name(number):
	if number > 0:
		return "P {}".format(number)
	else:
		return "O {}".format(-number)

#make_picture(best_solution, config_dict)
#create_graph(config_dict, return_dict)

def create_graph_best(config_dict, best_log, show=False):
	"""Creates a plot of averages and best points of eval vs fitness value

	Average and best fitness values are ploted on the same plot. They are saved
	to ./graphs/graphs/<random seed>.pdf
	The average fitness values are saved to ./graphs/points/<random seed>

	Args:
		config_dict (dict {str, val}): Configuraiton dictionary
		return_dict (dict {str, val}): Return dictionary from EA
		show (bool): The the resulting plot
	"""	
	# Create dictionaries to store values
	best_dict = {}

	best_log = best_log.split('\n')

	for line in best_log:
		if line == '' or 'Run' in line:
			continue
		sub_line = line.split()
		if len(sub_line) == 0:
			continue
		ev, best = int(sub_line[0]), int(sub_line[1])

		best_dict[ev] = best

	best_dict = OrderedDict(sorted(best_dict.items()))

	eval_best = []
	best_list = []

	for key, value in best_dict.items():
		eval_best.append(key)
		best_list.append(value)

	eval_best.append(config_dict['fitness_evaluations'])
	best_list.append(best_list[-1])

	# Plot values
	plt.plot(eval_best, best_list, label='Best')

	# Add information to graphs
	plt.xlabel('Eval')
	plt.ylabel('Fitness')
	plt.legend(loc='lower right')

	# Save figure
	plt.savefig('./graph/chart/' + str(config_dict['random_seed']) + '.pdf')

	# Show if true
	if show == True:
		plt.show()

	# Clear graph memory
	plt.clf()


def create_graph_ave(config_dict, return_dict, show=False):
	"""Creates a plot of averages and best points of eval vs fitness value

	Average and best fitness values are ploted on the same plot. They are saved
	to ./graphs/graphs/<random seed>.pdf
	The average fitness values are saved to ./graphs/points/<random seed>

	Args:
		config_dict (dict {str, val}): Configuraiton dictionary
		return_dict (dict {str, val}): Return dictionary from EA
		show (bool): The the resulting plot
	"""	
	# Create dictionaries to store values
	average_dict = {}
	best_dict = {}

	# For each eval in algorithm log
	for i in range(config_dict['runs']):
		# Split string by newline
		line = return_dict[i][0].split('\n')
		# For each line
		for l in line:
			# If line only had a new line, or if Run is in it, continue
			if l == '' or 'Run' in l:
				continue
			# Split line into parts
			sub_line = l.split()
			# Grab eval, ..., ave, best from line
			ev, ave, best = int(sub_line[0]), int(sub_line[-2]), int(sub_line[-1])

			# If eval is not in dict, add it
			if ev not in average_dict:
				average_dict[ev] = []
				best_dict[ev] = []

			# Add value in dictionaries
			average_dict[ev].append(ave)
			best_dict[ev].append(best)

	# Normalize and order average and best dictionaries
	eval_ave = []
	ave_list = []

	eval_best = []
	best_list = []

	average_dict = OrderedDict(sorted(average_dict.items()))
	best_dict = OrderedDict(sorted(best_dict.items()))

	for key, value in average_dict.items():
		eval_ave.append(key)
		ave_list.append(sum(average_dict[key]) / len(average_dict[key]))

	for key, value in best_dict.items():
		eval_best.append(key)
		best_list.append(sum(best_dict[key]) / len(best_dict[key]))

	# Plot values
	plt.plot(eval_ave, ave_list, label='Local Average')
	plt.plot(eval_best, best_list, label='Local Best')

	# Add information to graphs
	plt.xlabel('Eval')
	plt.ylabel('Fitness')
	plt.legend(loc='lower right')

	# Save figure
	plt.savefig('./graph/chart/' + str(config_dict['random_seed']) + '.pdf')

	# Show if true
	if show == True:
		plt.show()

	# Clear graph memory
	plt.clf()