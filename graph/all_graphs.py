import matplotlib.pyplot as plt
import numpy as np
import sys
import os
import collections
from collections import OrderedDict

# Files 
files = os.listdir("../logs/")

# number of runs
runs = 0
preivious_line = ''

for file in files:
	keep_track = False
	keep_absolute = False
	ave_dict = {}
	best_dict = {}
	last_eval = []
	absolute_eval = []
	with open("../logs/" + str(file), 'r') as f:
		for line in f:
			if "Run" in line and "Runs" not in line:
				runs = int(line.split()[1])
				keep_track = True
			elif "Absolute Fitness" in line:
				keep_track = False
				last_eval.append(preivious_line)
				keep_absolute = True
			elif keep_absolute == True:
				absolute_eval.append(line)
				keep_absolute = False
			elif keep_track == True:
				preivious_line = line
				eve, ave, best = line.split()
				eve = float(eve)
				ave = float(ave)
				best = float(best)
				if eve in ave_dict:
					ave_dict[eve].append(ave)
					best_dict[eve].append(best)
				else:
					ave_dict[eve] = [ave]
					best_dict[eve] = [best]

	for key, value in ave_dict.items():
		ave_dict[key] = sum(value) / len(value)

	for key, value in best_dict.items():
		best_dict[key] = sum(value) / len(value)

	ave_dict = OrderedDict(sorted(ave_dict.items()))
	best_dict = OrderedDict(sorted(best_dict.items()))

	eval_list = []
	ave_list = []
	best_list = []

	for key, value in ave_dict.items():
		eval_list.append(key)
		ave_list.append(value)

	for key, value in best_dict.items():
		best_list.append(value)

	plt.plot(eval_list, ave_list, label="Average")
	plt.plot(eval_list, best_list, label="Best")

	# Add information to graphs
	plt.xlabel('Eval')
	plt.ylabel('Fitness')
	plt.legend()#loc='lower right')

	# Save figure
	plt.savefig('./graphs/' + str(file) + '.pdf')

	#plt.show()

	# Clear graph memory
	plt.clf()

	### Add last eval points ###
	with open("./points/relative/" + str(file), 'w') as f:
		for line in last_eval:
			f.write(line.split()[-1] + '\n')

	### Add last absolute eval points ###
	with open("./points/absolute/" + str(file), 'w') as f:
		for line in absolute_eval:
			f.write(line.split()[-1] + '\n')

############# ABSOLUTE #############
# Files 
files = os.listdir("../absolute_fitness_log/")

# number of runs
runs = 0

for file in files:
	ave_dict = {}
	best_dict = {}
	last_eval = []
	with open("../absolute_fitness_log/" + str(file), 'r') as f:
		for line in f:
			if "Run" in line or line == '\n':
				pass
			elif "Last Eval:" in line:
				last_eval.append(line.split()[-1])
			else:
				keep_track = False
				eve, ave, best = line.split()
				eve = float(eve)
				ave = float(ave)
				best = float(best)
				if eve in ave_dict:
					ave_dict[eve].append(ave)
					best_dict[eve].append(best)
				else:
					ave_dict[eve] = [ave]
					best_dict[eve] = [best]

	for key, value in ave_dict.items():
		ave_dict[key] = sum(value) / len(value)

	for key, value in best_dict.items():
		best_dict[key] = sum(value) / len(value)

	ave_dict = OrderedDict(sorted(ave_dict.items()))
	best_dict = OrderedDict(sorted(best_dict.items()))

	eval_list = []
	ave_list = []
	best_list = []

	for key, value in ave_dict.items():
		eval_list.append(key)
		ave_list.append(value)

	for key, value in best_dict.items():
		best_list.append(value)

	plt.plot(eval_list, ave_list, label="Average")
	plt.plot(eval_list, best_list, label="Best")

	# Add information to graphs
	plt.xlabel('Eval')
	plt.ylabel('Fitness')
	plt.legend()#loc='lower right')

	# Save figure
	plt.savefig('./absolute/' + str(file) + '.pdf')

	#plt.show()

	# Clear graph memory
	plt.clf()