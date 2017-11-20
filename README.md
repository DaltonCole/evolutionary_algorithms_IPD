# EC Assignment 2b

## General Info

* Name: Dalton Cole
* Email: drcgy5@mst.edu
* Assignment: COMP SCI 5401 FS2017 Assignment 2b

## Description

For this assignment, the task is to implement the Prisoner's Dilemma problem using genetic programming search.

This is done by randomly creating a tree for each evaluation. The tree consists of AND, OR, XOR, and NOT operators. Each leaf of the tree is a string of "P" or "O" followed by a number. P represents a prisoner, O represents the opponent. The number is the number of the decision to look back at. For example, if the number if 2, we would look back at two decisions ago. Internally, if a number if positive, then it is a prisoner, negative if opponent.

## How to run
```
./run.sh
```

* It is assumed that any configurations that need to be done are done in *default.cfg*, which follows the JSON file format

## File Descriptions

### driver.cpp

This is the face of the program

### helper.*

This contains helper functions for the main program. Any change in a prisoner that involves more than one prisoner is done in this file.

### prisoner.*

This is the prisoner class. It represents what a prisoner does.

### config.*

This is the class that handles configutation. It sets up default parameters and reads in user defined values when initalized.

### default.cfg

The configuration file used to feed into the program. There are the following options (**bold** represents default, or null (i.e., use null for default value)):
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
* Population_Size:
	* ** 5 **
	* Any int
* Children_Count:
	* ** 5 **
	* Any int
* Parent_Selection:
	* ** Fitness_Proportional_Selection **
	* Over_Selection
* Survival_Selection:
	* ** Truncation **
	* K_Tournament_No_Replacement
* Parsimony_Pressure_Penalty_Coefficent
	* ** 0.5 **
	* Any float
* Termination_Convergence_Criterion
	* ** 10000 **
	* Any int

### logs/

The default file path for log files.

### solutions/

The default file path for solution files.

### graphs/

Automatically generated graph files, such as the fitness plot and tree graph.

### Python/

Contains assignment 2a