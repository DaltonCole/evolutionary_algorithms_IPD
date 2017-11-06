from random import randrange, choice
from copy import deepcopy

def fitness_value(pri, opp):
	"""Calculate fitness value

	Args:
		pri (int): If the prisoner defected or not
			0 if defected, 1 if cooperated
		opp (int): If the opponent defected or not
			0 if defected, 1 if cooperated

	Returns
		(int): The fitness value for the prisoner
	"""
	if pri == 0 and opp == 0:
		return 1
	elif pri == 0 and opp == 1:
		return 5
	elif pri == 1 and opp == 0:
		return 0
	elif pri == 1 and opp == 1:
		return 3

class Prisoner:
	"""Represents the prisoner in the Prisoner's Dilemma Problem

	Variables:
		operators (list of strings): The possible boolean operators of the tree
		pris_or_opp (list of [-1, 1]): Prisoner is a positive leaf, opponent
			is a negative leaf
		max_depth (int): The max depth the tree can have
		k (int): The number of previous rounds in memory (max)
		tree (list): The genetic programming tree
			Consists of a list of [str, [], []], where each sub-list
			is a branch consisting of the same format of the tree.
			A leaf node is an int that is either postive or negative,
			representing a prisoner or opponent. The values are bounded by
			(1, k)
		fitness (float): The average fitness value given a list of 
			opponent moves
		sequence (list of [0,1]): The sequence played so far.
			Has a max length of k
	"""
	# Possible operators
	operators = ["AND", "OR", "NOT", "XOR"]
	# Opponent is negative for leaf nodes, prisoner is positive
	pris_or_opp = [-1, 1]

	# Max depth of the tree
	max_depth = 0
	# Number of previous rounds to keep track of
	k = 0

	def __init__(self):
		"""Initalizes a Prisoner class

		Generates the Genetic Programming tree
		"""
		self.tree = self.randomly_generate_tree()

	def randomly_generate_tree(self):
		"""Generates the tree

		Returns the tree generated
		"""
		return self.generate_branch(0);

	def generate_branch(self, depth):
		""" Recursively generate each branch of the tree

		Args:
			depth (int): The current depth

		Returns:
			(list): A branch of the tree
		"""
		if depth == self.max_depth - 1:
			# Randomly select an operator
			op = choice(self.operators)
			# If two branches
			if(op != "NOT"):
				# Return a list of [operator, num, num]. -num = opponent, num = prisoner
				return [op, choice(self.pris_or_opp) * randrange(1, self.k), choice(self.pris_or_opp) * randrange(1, self.k)]
			else:
				# Return ["NOT", num]. -num = opponent, num = prisoner
				return [op, choice(self.pris_or_opp) * randrange(1, self.k)]
		else:
			# Randomly select an operator
			op = choice(self.operators)
			# If two branches
			if(op != "NOT"):
				# Return a list of [operator, branch, branch]
				return [op, self.generate_branch(depth + 1), self.generate_branch(depth + 1)]
			else:
				# Return ["NOT", branch]
				return [op, self.generate_branch(depth + 1)]

	def find_fitness(self, iterations, sequence, opponent):
		"""Find fitness of tree given a sequence of opponent moves

		Args:
			iterations (int): The number of iterations to play out
			sequence (list of [0,0]): The moves made so far
			opponent (list of int): The moves the opponent will make

		Returns:
			(int): The average fitness
		"""
		# Number of decisions made
		count = 0
		# Deep copy of the sequence so we can add to end, pop from beginning
		self.sequence = deepcopy(sequence)

		# Reset fitness
		self.fitness = 0

		# While there are rounds to play
		while count < iterations:
			# Find decision
			decision = self.evaluate()
			# Add fitness value to fitness
			self.fitness += fitness_value(decision, opponent[count])
			# Add move to begining of sequence, pop last item in list
			if len(sequence) == self.k:
				del sequence[-1]
				sequence.insert(0, [decision, opponent[count]])

			# Increment count
			count += 1


		# Take average fitness
		self.fitness /= iterations

		return self.fitness

	def evaluate(self):
		"""Find what we will do given past moves

		Returns:
			(int): 0 or 1, 0 being defect, 1 being cooperate
		"""
		return self._recursive_evaluate(self.tree)

	def _recursive_evaluate(self, branch):
		"""Recursively find the value of each branch

		Args:
			branch (list [str, branch, branch]): Branch of tree

		Returns:
			(int): 0 or 1, 0 being defect, 1 being cooperate
		"""
		# Operation
		op = branch[0]

		# Base Case
		if type(branch[1]) == int:
			if op == "AND":
				return self._get_sequence_item(branch[1]) & self._get_sequence_item(branch[2])
			elif op == "OR":
				return self._get_sequence_item(branch[1]) | self._get_sequence_item(branch[2])
			elif op == "XOR":
				return self._get_sequence_item(branch[1]) ^ self._get_sequence_item(branch[2])
			else: # NOT
				if self._get_sequence_item(branch[1]) == 1:
					return 0
				else:
					return 1
		else:
			# Recursively find bool value
			if op == "AND":
				return self._recursive_evaluate(branch[1]) & self._recursive_evaluate(branch[2])
			elif op == "OR":
				return self._recursive_evaluate(branch[1]) | self._recursive_evaluate(branch[2])
			elif op == "XOR":
				return self._recursive_evaluate(branch[1]) ^ self._recursive_evaluate(branch[2])
			else: # NOTself.
				if self._recursive_evaluate(branch[1]) == 1:
					return 0
				else:
					return 1

	def _get_sequence_item(self, number):
		"""Get item in sequence
		If prisoner, take normal value, if opponent, need to take abs() first

		Args:
			number (int): The leaf node number

		Returns:
			(int): 0 or 1, the value in sequence corresponding to number
		"""
		# If prisoner
		if number > 0:
			return self.sequence[number][0]
		else:
			return self.sequence[-number][1]

	def __str__(self):
		"""Convert tree into string in pre-order format

		Returns:
			(str): String of tree if pre-order format
		"""
		return self.sub_string(self.tree)

	def sub_string(self, branch):
		"""Recursively generate pre-order formated string

		Args:
			branch (list of [str, branch, branch]): Tree branch

		Returns:
			(str): String of "str branch branch" or "str leaf leaf"
		"""
		op = branch[0]

		# Base case:
		if type(branch[1]) == int:
			if op != "NOT":
				return "{} {} {}".format(branch[0], branch[1], branch[2])
			else:
				return "{} {}".format(branch[0], branch[1])
		else:
			if op != "NOT":
				return "{} {} {}".format(branch[0], self.sub_string(branch[1]), self.sub_string(branch[2]))
			else:
				return "{} {}".format(branch[0], self.sub_string(branch[1]))
		
	def __lt__(self, other):
		"""Define less than to be by fitness value

		Args:
			other (Prisoner): Prisoner to compare against

		Returns:
			(bool): Which one has lower fitness
		"""
		return self.fitness < other.fitness