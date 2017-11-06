from random import randrange, choice
from copy import deepcopy

def fitness_value(pri, opp):
	if pri == 0 and opp == 0:
		return 1
	elif pri == 0 and opp == 1:
		return 5
	elif pri == 1 and opp == 0:
		return 0
	elif pri == 1 and opp == 1:
		return 3

class Prisoner:
	operators = ["AND", "OR", "NOT", "XOR"]
	pris_or_opp = [-1, 1]

	max_depth = 0
	k = 0

	def __init__(self):
		self.tree = self.randomly_generate_tree()

	def randomly_generate_tree(self):
		return self.generate_branch(0);

	def generate_branch(self, depth):
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

	def find_fitness(self, k, iterations, sequence, opponent):
		count = 0
		self.sequence = deepcopy(sequence)

		self.fitness = 0
		while count < iterations:
			decision = self.evaluate()
			self.fitness += fitness_value(decision, opponent[count])
			count += 1

		return self.fitness

	def evaluate(self):
		return self._recursive_evaluate(self.tree)

	def _recursive_evaluate(self, branch):
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
		# If prisoner
		if number > 0:
			return self.sequence[number][0]
		else:
			return self.sequence[-number][1]

	def __str__(self):
		return self.sub_string(self.tree)

	def sub_string(self, branch):

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
		return self.fitness < other.fitness