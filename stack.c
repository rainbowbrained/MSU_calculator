#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

// Data structure for stack with operators
struct stack
{
	unsigned long int maxsize;	// define max capacity of stack
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

// Utility function to initialize stack with operators
struct stack* StackInit(void)
{
	struct stack *pt = (struct stack*)malloc(sizeof(struct stack));
	if (pt == NULL) {
		perror ("Error while creating the stack");
		return NULL;
	}

	pt->maxsize = 3;
	pt->top = -1;
	pt->items = (char*)malloc(sizeof(char) * pt->maxsize);
    if (pt->items == NULL) {
		perror ("Error while creating the stack");
		return NULL;
	}
	return pt;
}

// Utility function to deinitialize stack with operators
void StackDeinit(struct stack *pt)
{
	free(pt->items);
	free(pt);
	return;
}

// Utility function to return the size of the stack with operators
int size(struct stack *pt)
{
	return pt->top + 1;
}

// Utility function to check if the stack with operators is empty or not
int isEmpty(struct stack *pt)
{
	return pt->top == -1;
}

// Utility function to check if the stack with operators is full or not
int isFull(struct stack *pt)
{
	return pt->top == pt->maxsize - 1;
}

// Utility function to add an element x in the stack with operators
void push(struct stack *pt, char x)
{
	// check if stack is already full, if so - realloc the memory
	if (isFull(pt))
	{
		pt->maxsize += 100;
	    pt->items = (char*)realloc(pt->items, sizeof(char) * pt->maxsize);
	    if (pt->items == NULL) {
		    perror ("Error while reallocating memory for stack items");
		    return;
	    }
	}
	
	// add an element and increments the top index
	pt->items[++pt->top] = x;
}

// Utility function to return top element in a stack with operators
char peek(struct stack *pt)
{
	// check for empty stack
	if (!isEmpty(pt))
		return pt->items[pt->top];
	else
		return 0;
}

// Utility function to pop top element from the stack with operators
char pop(struct stack *pt)
{
	// check for stack underflow
	if (isEmpty(pt))
	{
		return 0;
	}
	// decrement stack size by 1 and return the popped element
	return pt->items[pt->top--];
}

//-------------------------------------------------------------------------
// Same functions for stak with operands
// Utility function to initialize stack with operands
struct ostack* oStackInit(void)
{
	struct ostack *pt = (struct ostack*)malloc(sizeof(struct ostack));
	if (pt == NULL) {
		perror ("Error while creating the stack");
		return NULL;
	}

	pt->maxsize = 3;
	pt->top = -1;
	pt->items = (double*)malloc(sizeof(double) * pt->maxsize);
    if (pt->items == NULL) {
		perror ("Error while creating the stack");
		return NULL;
	}
	return pt;
}

// Utility function to deinitialize stack with operands
void oStackDeinit(struct ostack *pt)
{
	free(pt->items);
	free(pt);
	return;
}

// Utility function to return the size of the stack with operands
int osize(struct ostack *pt)
{
	return pt->top + 1;
}

// Utility function to check if the stack with operands is empty or not
int oisEmpty(struct ostack *pt)
{
	return pt->top == -1;
}

// Utility function to check if the stack with operands is full or not
int oisFull(struct ostack *pt)
{
	return pt->top == pt->maxsize - 1;
}

// Utility function to add an element x in the stack with operands
void opush(struct ostack *pt, double x)
{
	// check if stack is already full, if so - realloc the memory
	if (oisFull(pt))
	{
		pt->maxsize += 100;
	    pt->items = (double*)realloc(pt->items, sizeof(double) * pt->maxsize);
	    if (pt->items == NULL) {
		    perror ("Error while reallocating memory for stack items");
		    return;
	    }
	}
	
	// add an element and increments the top index
	pt->items[++pt->top] = x;
}

// Utility function to return top element in a stack with operands
double opeek(struct ostack *pt)
{
	// check for empty stack
	if (!oisEmpty(pt))
		return pt->items[pt->top];
	else
		return 0;
}

// Utility function to pop top element from the stack with operands
double opop(struct ostack *pt)
{
	// check for stack underflow
	if (oisEmpty(pt))
	{
		fprintf(stderr, "The stack is empty\n");
		return 0;
	}
	// decrement stack size by 1 and return the popped element
	return pt->items[pt->top--];
}
