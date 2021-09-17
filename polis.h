#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "stack.h"

// Function initializes the table of variables. Also contains variables with empty names,
// which represent numbers used in calculation.
struct variables* VarInit(void);

// Function deinitializes the table of variables by freeing all the allocated memory.
void VarDeinit(struct variables *pt);

// Function checks if the table of variables is full or not
int VarisFull(struct variables *pt);

// Check if variable new_name is already in the table 
// if so - return it's index, else return -1
long long int inVar(struct variables *pt, char *new_name);

// Function to add a variable new_name with the value new_val to the table of variables
void VarAdd(struct variables *pt, char* new_name, double new_val);

// Check if variable new_name is already in the table 
// if so - return it's index, else return -1
long long int inVar(struct variables *pt, char *new_name);

// Utility function, prints the table of variables
void VarPrint(struct variables *pt);

// Function analyzes the string, which suppose to contain a variable. It gets variable's
// name and value given after '=' symbol, then adds it to the table of variables.
// Checks if the string is correct.
int GetVarFromStr (struct variables *pt, char *str);

// Function analyzes information from the string given in input_str. It may be a stdin or
// a file name. It predicts if the string contains variable (then calls GetVarFromStr())
// or the string contains an expression to calculate (calls generate polis).
// Analizes strings until EOF.
int GetCalcInfo (struct variables *pt, char *input_str);

// This function generates polis from the given string. Polis string contains
// indexes in the table of variables, operands *, /, +, -, separated with '[' before index
// and ']' before an operand (makes it easier to calculate). Then calls function 
// CalculatePolis(). Also checks sintax errors.
int GeneratePolis (char *str, struct variables *table);

// This function calculates polis from the given polis string. It creates a stack of 
// operands and calculates intermediate values by calling CalculateOperation() function.
// Also checks if the string is incorrect (ex. trying to calculate binary operation, but
// the stack is already empty).
double CalculatePolis (char *str, struct variables *table);

// The function returns the result of binary operation '*', '/', '+' or '-' given in the 
// operand. Also checks overflow errors and attempt to divide by zero.
double CalculateOperation (double y, double x, char * operand);

// The function cleans the table of operands from all the operands with empty names, which
// considered to be numbers.
void FlushTable (struct variables *pt);

struct variables
{
	unsigned long int maxsize;	// max capacity of variables
	long long int quantity; //number of variables
	char **name;
	double *value;
};
