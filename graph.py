import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from collections import OrderedDict

from graphviz import Digraph
from uuid import uuid4 as uu

def make_picture(best_solution, config_dict, yes_view=False):
	"""Creates a tree graph cooresponding to the best GP tree

	Args:
		best_solution (Prisoner): Prisoner with the best fitness
		config_dict (str: value): Dictionary of configurations
		yes_view (bool): Display tree after generating

	Return:
		Nothing. Saves tree in ./graph/tree/<random seed>.pdf
	"""
	# Declair tree
	dot = Digraph(comment="Prison Tree")
	# Make root node
	node = str(uu())
	dot.node(node, str(best_solution.tree[0]))
	# Recursively make rest of tree
	recursive_dot(dot, best_solution.tree, node)
	# Save tree, display if yew_view is true
	dot.render('graph/tree/{}'.format(config_dict['random_seed']), view=yes_view)


def recursive_dot(dot, branch, node):
	"""Recursively generates the dot graph

	Args:
		dot (graphviz): Graph
		branch (list of lists): Branch from Prisoner tree
		node (str): Name of the node
	"""
	# Base Case, leaf nodes are ints
	if type(branch[1]) == int:
		# Make left node
		dot.node(node, branch[0])
		node1 = str(uu())
		dot.node(node1, dot_name(branch[1]))
		# Add edge from parent to child
		dot.edge(node, node1)
		# If right node, do the same
		if branch[0] != "NOT":
			node2 = str(uu())
			dot.node(node2, dot_name(branch[2]))
			dot.edge(node, node2)
	else:
		# Make left node
		dot.node(node, branch[0])
		node1 = str(uu())
		dot.edge(node, node1)
		# Follow left node
		recursive_dot(dot, branch[1], node1)
		# If right node
		if branch[0] != "NOT":
			# Make right node
			node2 = str(uu())
			dot.edge(node, node2)
			# Follow right node
			recursive_dot(dot, branch[2], node2)

def dot_name(number):
	"""Name leaf node P or O if referring to prisoner or opponent

	Args:
		number (int): Leaf node value. Positive for prisoner, negative 
			for opponent

	Returns:
		(str): P or O + " " + number
	"""
	if number > 0:
		return "P {}".format(number)
	else:
		return "O {}".format(-number)


def create_graph_best(config_dict, best_log, show=False):
	"""Creates a plot of the best points of eval vs fitness value

	Average and best fitness values are ploted on the same plot. They are saved
	to ./graph/chart/<random seed>.pdf

	Args:
		config_dict (dict {str, val}): Configuraiton dictionary
		best_log (str): Best log from return dict
		show (bool): The the resulting plot
	"""	
	# Create dictionaries to store values
	best_dict = {}

	# Split by new line
	best_log = best_log.split('\n')

	# For each line
	for line in best_log:
		# If line is not a fitness, skip
		if line == '' or 'Run' in line:
			continue

		# Split line into ev, and best
		sub_line = line.split()
		if len(sub_line) == 0:
			continue

		# Add fitness to dict
		ev, best = float(sub_line[0]), float(sub_line[1])
		best_dict[ev] = best

	# Order dict by eval from least to greatest
	best_dict = OrderedDict(sorted(best_dict.items()))

	# Add elements to list to be ploted
	eval_best = []
	best_list = []
	for key, value in best_dict.items():
		eval_best.append(key)
		best_list.append(value)

	# Make last eval be part of list so it is graphed
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