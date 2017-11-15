# EC Assignment 2a

## General Info

* Name: Dalton Cole
* Email: drcgy5@mst.edu
* Assignment: COMP SCI 5401 FS2017 Assignment 2a

## Description

For this assignment, the task is to implement the Prisoner's Dilemma problem using genetic programming and random search.

This is done by randomly creating a tree for each evaluation. The tree consists of AND, OR, XOR, and NOT operators. Each leaf of the tree is a string of "P" or "O" followed by a number. P represents a prisoner, O represents the opponent. The number is the number of the decision to look back at. For example, if the number if 2, we would look back at two decisions ago. Internally, if a number if positive, then it is a prisoner, negative if opponent.

Fitness is determined by subtracting the fitness of the tree (average number of years the prisoner was able to get out of prison) by the tic-for-tac method. Thus, if the fitness is negative, then tic-for-tac performed better than random search.

## How to run
```
./run.sh
```

* It is assumed that any configurations that need to be done are done in *default.cfg*, which follows the JSON file format

## File Descriptions

### driver.py

The face of the program

### graph.py

This file is used by the driver to automatically generate the fitness vs evals plot and generate a tree shape of the solution.

### helper.py

Contains helper functions used by the driver

### progressBar.py

Contains the logic for the progress bar

### prisoner.py

Contains the prisoner GP class

### config.json

The configuration file used to feed into the program. There are the following options (**bold** represents default, or null):
* Random Seed:
	* ** Time in seconds since Epoch **
	* Any int
* l:
	* ** 5 **
	* Any int
* k:
	* ** 5 **
	* Any int
* d:
	* ** 5 **
	* Any int
* Runs:
	* ** 1 **
	* Any int
* Log File Path:
	* ** /logs/<Random Seed> **
	* Any path
* Solution File Path:
	* ** /solutions/<Random Seed> **
	* Any path
* Fitness Evaluations:
	* ** 10000 **
	* Any int

### logs/

The default file path for log files.

### solutions/

The default file path for solution files.

### graphs/

Automatically generated graph files, such as the fitness plot and tree graph.

### scripts/

Contains useful scripts