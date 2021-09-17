#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

// Data structure for stack with operators
/*
struct stack
{
	unsigned long int maxsize;	// max capacity of stack
	long long int top;		
	char *items;
};
// Data structure for stack with operands
struct ostack
{
	unsigned long int maxsize;	// define max capacity of stack
	long long int top;		
	double *items;
};
*/
// Functions initializes stack with operators and checks if memory was allocated correctly
// returns a pointer to the stack with operands
struct stack* StackInit(void);

// Function frees all the memory which was allocated for the stack with operators
void StackDeinit(struct stack *pt);

// Function returns the size of the stack with operators (the number of the top element)
long long int size(struct stack *pt);

// Function checks if the stack with operators is empty (top index == -1) or not
// return 1 if it is full or 0 if not
int isEmpty(struct stack *pt);

// Function checks if the stack with operators is full (top index == maxsize) or not
// return 1 if it is full or 0 if not
int isFull(struct stack *pt);

// Function adds an element x in the stack with operators and increments the top index
void push(struct stack *pt, char x);

// Functions return top element in a stack with operators or 0 if stack is empty
char peek(struct stack *pt);

// Function pops top element from the stack with operators and
// decrements stack size by 1 and return the popped element
char pop(struct stack *pt);

//---------------------------------------------------------------------------------------
// Same functions for the stack with operands
// Functions initializes stack and checks if memory was allocated correctly
// returns a pointer to the stack with operands
struct ostack* oStackInit(void);

// Function frees all the memory which was allocated for the stack with operands
void oStackDeinit(struct ostack *pt);

// Function returns the size of the stack with operands (the number of the top element)
long long int osize(struct ostack *pt);

// Function checks if the stack with operands is empty (top index == -1) or not
// return 1 if it is full or 0 if not
int oisEmpty(struct ostack *pt);

// Function checks if the stack with operands is full (top index == maxsize) or not
// return 1 if it is full or 0 if not
int oisFull(struct ostack *pt);

// Function adds an element x in the stack with operands and increments the top index
void opush(struct ostack *pt, double x);

// Functions return top element in a stack with operands or 0 if stack is empty
double opeek(struct ostack *pt);

// Function pops top element from the stack with operands and
// decrement stack size by 1 and return the popped element
double opop(struct ostack *pt);
