# MSU_calculator
The project implements calculator using  recursive descent and Reverse Polish notation

 CONCEPT 
 =====================

  In calculations is used a structure "variables", which contain name and value of each
variable. THis structure is also used for numbers in calculations to simplify the
computing process (see "CalculatePOlis" in POLIS.H FUNCTIONS below).
  
    struct variables
    {
      unsigned long int maxsize; // max capacity of variables
      long long int quantity;    // current number of variables
      char **name;               // array of names 
      double *value;             // array of values
    };
  
  Before starting the calculation the user must create a table of variables by calling
VarInit() function. Also the result of the last calculation will be passed to the user
through this table in the variable "result". After the calculation user must call 
VarDeinit() function to prevent all the memory leaks and destroy the table of variables.

  The programm gets the name of the input stream through the second arguement (the first 
is the name of a program) and passes this name to the GetCalcInfo() function.

  This function analyzes text from the given input stream (maybe stdin or the file name)
linewise. Each line may content the initialization of 1 variable (the format is 
[name|'='|value]) or an expression to calculate (all other formats). Then the function 
passes each line to an apropriate function: GetVarFromStr() or GeneratePolis() until
the stream is over.

  If the line contains a variable, the function GetVarFromStr() gets needed information
from the string and adds a new variable to the table of variables by calling VarAdd().
For additional information see the POLIS.H FUNCTIONS below.

  If the line containes calculation, the function GeneratePolis() generates a polis,
passes a string to the CalculatePolis() and adds a variable "result" with the value
which equals the result of the calculation by calling VarAdd(). Akso the function
prints the result of an expression to the stdout.

  All the functions are user-friendly as much as possible. The user gets informed about 
all the syntax errors, attempts to divide on zero, overflows while counting, wrong 
initialization of variables and other mistakes without exiting the program. Functions 
process various complex expressions with many spaces and infinetely long strings:

    INPUT STRING                     | INTERPRETATION
    1) 1var    =   -  1              | Variable "1var" equals -1
    2)     !_sa3:     =  +  2        | Variable "!_sa3:" equals 2
    3) x=-.45   1234                 | Variable "x" equals -0.45
    4) y1z =     4.                  | Variable "y1z" equals 4
    5) .2   * (-4   +10.)            | Expression 0.2*(0-4+10) to calculate
    6) ( (  -   1))  / (x +321y  )   | Expression (0-1)/(x+321y) to calculate

#### Cases of errors in the string with variable:
  
  1) Expression before '=' symbol is empty or contains only spaces => "Undefined name of 
a variable\n";
  
  2) Expression before '=' symbol containes '-', '+', '/', '*', '.', '(' or ')' symbol;
  
  3) Expression contains multiple '=' symbols between the variable's name and value;
  
  4) Expression contains multiple sign symbols '+' or '-' between the '=' symbol and 
numeric value, which supposes to be variable's value;
  
  5) Expression after '=' which which supposes to be variable's value contains multiple
'.' symbols;
  
  6) Expression after '=' which which supposes to be variable's value contains symbols
other then digits, sign symbols '+' or '-' and '.'.
  
  7) The name of a variable containes more than 6 symbols;
  
  8) The name of a variable contains symvols separated by spaces.


#### Cases of errors in the string with calculation:
  
  1) 2 or more operands in a row;
  
  2) 2 or more numbers in a row;
  
  3) A '*' or '/' symbol after an open bracket;
  
  4) Disbalanced brackets;
  
  5) 2 or more points in one number;
  
  6) The string containes name of a variable which is missing in the table of variables.
  
#### IMPORTANT NOTES:
  
  - If an overflow or an attempt of dividing by zero occurs while the calculation the
  program prints the diagnostic about it, but the result (0) may still be printed but 
  there is no guarantee that it is correct.
  
  - The main.c programm prints the gets the numeric result only of a final calculation.
  Results of previous calculations are only printed in stdout. If a user wants to have
  an access to the result of calculation, he must stop the calculating process after
  getting the result of needed expression.
  
  - The archive contains .txt files with tests and answers. The tests.txt contains
  strings which are processed correctly, tests_ans.txt containes answers to this tests,
  which were generated manually. The mistakes.txt contains strings which are considered 
  to be incorrect, mistakes_ans.txt containes outputs, which should be printed by the 
  programm, which were generated manually too.


 STACK.H FUNCTIONS 
 =====================

  This library containes all the function needed for operating with stacks in generating 
and calculating polis, which include different kinds of stacks: for operands and 
operations. All stacks may be infinitely long.
  
  Library <stack.h> uses auxiliary libraries:
1) <stdio.h>
2) <errno.h>
3) <stdlib.h>

  Data structure for stack with operators:
  
    struct stack
    {
      unsigned long int maxsize;  // max capacity of stack
      long long int top;          // the order number of item on the top
      char *items;                // an array of chars  
    };

  Data structure for stack with operands
  
    struct ostack
    {
      unsigned long int maxsize;  // max capacity of stack
      long long int top;          // the order number of item on the top
      double *items;              // an array of float numbers
    };

  Notation:

[1] struct stack* StackInit(void);
[2] void StackDeinit(struct stack *pt);
[3] long long int size(struct stack *pt);
[4] int isEmpty(struct stack *pt);
[5] int isFull(struct stack *pt);
[6] void push(struct stack *pt, char x);
[7] char peek(struct stack *pt);
[8] char pop(struct stack *pt);
[9] struct ostack* oStackInit(void);
[10] void oStackDeinit(struct ostack *pt);
[11] long long int osize(struct ostack *pt);
[12] int oisEmpty(struct ostack *pt);
[13] int oisFull(struct ostack *pt);
[14] void opush(struct ostack *pt, double x);
[15] double opeek(struct ostack *pt);
[16] double opop(struct ostack *pt);

_____________________________________________________________________

#### [1]  StackInit

SYNOPSIS
    
    struct stack* StackInit(void);

DESCRIPTION
    
    Initializes the stack with operators and checks if memory was allocated correctly.

ERRORS
    
    An error might occur while the allocation of the stack. In this case function returns
    NULL Pointer.

RETURN VALUE
    
    Returns a pointer to the stack with operands
_____________________________________________________________________

#### [2]  StackDeinit

SYNOPSIS
    void StackDeinit(struct stack *pt);

DESCRIPTION
    Deinitializes the stack with operators and frees all the allocated memory
_____________________________________________________________________

[3]  size

SYNOPSIS
    
    long long int size(struct stack *pt);

DESCRIPTION
    
    Function returns the size of the stack with operators.

RETURN VALUE
    
    Returns a size of the stack with operators.
 _____________________________________________________________________
 
#### [4]  isEmpty

SYNOPSIS
    
    int isEmpty(struct stack *pt)

DESCRIPTION
    
    Function checks if the stack with operators is empty or not.

RETURN VALUE
    
    Returns 1 if the stack is empty or 0 if not.
_____________________________________________________________________

#### [5]  isFull

SYNOPSIS
    
    int isFull(struct stack *pt)

DESCRIPTION
    
    Function checks if the stack with operators is full or not.

RETURN VALUE
    
    Returns 1 if the stack is full or 0 if not.    
_____________________________________________________________________

#### [6]  push

SYNOPSIS
    
    void push(struct stack *pt, char x)

DESCRIPTION
    
    Function adds an element x to the stack with operators on which points pt. Also 
    increases the number of elements.

ERRORS
    
    An error might occur while the reallocation of the stack. In this case function quits.
_____________________________________________________________________

#### [7]  peek

SYNOPSIS
    
    peek(struct stack *pt)

DESCRIPTION
    
    Function returns top element in a stack with operators on which points pt. If the 
    stack is empty returns 0.
_____________________________________________________________________

#### [8]  pop

SYNOPSIS
    
    char pop(struct stack *pt)

DESCRIPTION
    
    Function returns top element in a stack with operators on which points pt, pops it
    from the stack and decreases the total number of elements. If the stack is empty returns 0.
_____________________________________________________________________

#### [9]  oStackInit

SYNOPSIS
    
    struct stack* oStackInit(void);

DESCRIPTION
    
    Same as StackInit but for the stack with operands.
_____________________________________________________________________

#### [10]  oStackDeinit

SYNOPSIS
    
    void oStackDeinit(struct ostack *pt);

DESCRIPTION
    
    Same as StackDenit but for the stack with operands.
_____________________________________________________________________

#### [11]  osize

SYNOPSIS
    
    long long int osize(struct ostack *pt);

DESCRIPTION
    
    Same as size but for the stack with operands.
_____________________________________________________________________

#### [12]  oisEmpty

SYNOPSIS
    
    int oisEmpty(struct ostack *pt)

DESCRIPTION
    
    Same as oisEmpty but for the stack with operands.
_____________________________________________________________________

#### [13]  oisFull

SYNOPSIS
    
    int oisFull(struct ostack *pt)

DESCRIPTION
    
    Same as isFull but for the stack with operands.   
_____________________________________________________________________

#### [14]  opush

SYNOPSIS
    
    void opush(struct ostack *pt, double x)

DESCRIPTION
    
    Same as push but for the stack with operands.   
_____________________________________________________________________

#### [15]  opeek

SYNOPSIS
   
   double opeek(struct ostack *pt)

DESCRIPTION
   
   Same as opeek but for the stack with operands.  
_____________________________________________________________________

#### [16]  opop

SYNOPSIS
    
    double opop(struct ostack *pt)

DESCRIPTION
    
    Same as opop but for the stack with operands.     
_____________________________________________________________________
***


 POLIS.H FUNCTIONS 
 =====================

  This library containes all the function needed for reading variables and expression to
calculate, controlls of input, computations and getting the result. 

  Library <polis.h> uses auxiliary libraries:
1) <stdio.h>
2) <errno.h>
3) <stdlib.h>
4) <string.h>
5) <float.h>
6) "stack.h"

  Notation:

[1] struct variables* VarInit(void);
[2] void VarDeinit(struct variables *pt)
[3] int VarisFull(struct variables *pt)
[4] void VarAdd(struct variables *pt, char* new_name, double new_val);
[5] long long int inVar(struct variables *pt, char *new_name)
[6] void VarPrint(struct variables *pt)
[7] int GetVarFromStr (struct variables *pt, char *str)
[8] int GetCalcInfo (struct variables *pt, char *input_str) 
[9] int GeneratePolis (char *str, struct variables *table)
[10] double CalculatePolis (char *str, struct variables *table) 
[11] double CalculateOperation (double y, double x, char * operand);
[12] void FlushTable (struct variables *pt)

_____________________________________________________________________

#### [1]  VarInit

SYNOPSIS
   
    struct variables* VarInit(void)

DESCRIPTION
    
    Function initializes the table of variables and checks if memory was allocated correctly.

ERRORS
    
    An error might occur while the allocation of the table. In this case function returns
    NULL Pointer.

RETURN VALUE
    
    Returns a pointer to the table of variables.
_____________________________________________________________________

#### [2]  VarDeinit

SYNOPSIS
   
   void VarDeinit(struct variables *pt)

DESCRIPTION
    
    Function deinitializes the table of variables by freeing all the allocated memory.
    Same as StackDeinit but for the table of variables.  
_____________________________________________________________________

#### [3]  VarisFull

SYNOPSIS
   
   int VarisFull(struct variables *pt)

DESCRIPTION
    
    Function checks if the table of variables is full or not (current variable has an 
    order number equal to the maxsize). Same as isFull but for the table of variables.
_____________________________________________________________________

#### [4]  VarAdd

SYNOPSIS
   
    void VarAdd(struct variables *pt, char* new_name, double new_val)

DESCRIPTION
    
    Function adds a variable new_name with the value new_val to the table of variables.
    If the new_name equals '\0\0\0\0\0\0' it means that function will add a value with
    an empty name in the table. This is necessary as GeneratePolise() and 
    CalculatePolis() function operate with indexes in the table of variable instead of
    real values.
    
    Before the function adds a variable it checks if a variable already exists in the 
    table. If so, the function swaps previous value with a new one and quits. Else the
    function extend the table of variables (reallocating the memory if needed) and 
    increases the number of variables.
    
ERRORS
    
    An error might occur while the reallocation of the table. In this case function 
    prints the signal about an error in the stderr and quits.
_____________________________________________________________________

#### [5]  inVar

SYNOPSIS
   
    long long int inVar(struct variables *pt, char *new_name)

DESCRIPTION
    
    Check if variable new_name is already in the table by searching throuht names of 
    variables. 
    
RETURN VALUE
    
    If so - return it's index, else return -1.
_____________________________________________________________________

#### [6]  VarPrint

SYNOPSIS
   
    void VarPrint(struct variables *pt)

DESCRIPTION
    
    Utility function, prints the table of variables in the stdout stream.
_____________________________________________________________________

#### [7]  GetVarFromStr

SYNOPSIS
   
    int GetVarFromStr (struct variables *pt, char *str)

DESCRIPTION
    
    Function analyzes the string elementwise, which suppose to contain a variable. It 
    gets variable's name and value given after '=' symbol, then adds it to the table of 
    variables by calling VarAdd(). Checks if the string is correct.
    
ERRORS
    
    An error may occur if a string has an incorrect format. In this case function prints
    in the stderr the notification about mistake and returns -1.
    
    Cases of errors in the string with variable:
    - Expression before '=' symbol is empty or contains only spaces => "Undefined name of
    a variable\n";
    - Expression before '=' symbol containes '-', '+', '/', '*', '.', '(' or ')' symbol;
    - Expression contains multiple '=' symbols between the variable's name and value;
    - Expression contains multiple sign symbols '+' or '-' between the '=' symbol and 
    numeric value, which supposes to be variable's value;
    - Expression after '=' which which supposes to be variable's value contains multiple
    '.' symbols;
    - Expression after '=' which which supposes to be variable's value contains symbols
    other then digits, sign symbols '+' or '-' and '.'.
    - The name of a variable containes more than 6 symbols;
    - The name of a variable contains symvols separated by spaces.
    
RETURN VALUE
    
    In case of getting a correct string and calling VarAdd() the function return 0, in 
    case of an error return -1.
_____________________________________________________________________

#### [8]  GetCalcInfo

SYNOPSIS
   
    int GetCalcInfo (struct variables *pt, char *input_str) 

DESCRIPTION
    
    Function analyzes information from the string given in input_str. It may be a stdin or
    a file name. It predicts if the string contains variable (then calls GetVarFromStr())
    or the string contains an expression to calculate (calls generate polis).
    Analizes strings until the end of stream or the end of file.
    
    If input_str equals "stdin" the function prints invitation to input the data.
    
    The function can get infinetly long strings though it has a dynamicly increasing 
    buffer.
    
ERRORS
    
    An error may occur while allocating or reallocating memory for the buffer. In this 
    case function prints in the stderr the notification about mistake and returns errno.

    An error may occur if the file, which name is in the input_str, can not be opened. In
    this case function prints in the stderr notification about mistake and returns errno.
    
RETURN VALUE
    
    In case of getting a correct string and calling VarAdd() the function returns 0, in 
    case of an error returns errno.
_____________________________________________________________________

#### [9]  GeneratePolis

SYNOPSIS
   
    int GeneratePolis (char *str, struct variables *table)

DESCRIPTION
    
    This function generates polis from the given string by analyzing the string 
    elementwise. IT operates according to the Dijkstra's alorithm. If the input 
    string was correct, the function calls CalculatePolis() and adds a variable 
    "result" in the table of variables with the final value of a given expression.
    
    If the function gets a name of a variable, it inserts in the polis the variable's 
    index in the table of variables. If it gets a number, it adds a number in the end
    of a table of variables by calling VarAdd() and inserts in the polis the number's 
    index in the table of variables. 
    
    To simplify calculations in the CalculatePolis(), function inserts '[' symbol 
    before each index and a ']' symbol before each operation. 
    
ERRORS
    
    An error may occur while allocating or reallocating memory for the buffer. In this 
    case function prints in the stderr the notification about mistake and returns errno.

    An error may occur if the string contains the following mistakes:
    - 2 or more operands in a row;
    - 2 or more numbers in a row;
    - A '*' or '/' symbol after an open bracket;
    - Disbalanced brackets;
    - 2 or more points in one number;
    - The string containes name of a variable which is missing in the table of variables.
    In all this cases function prints in the stderr the diagnostic and returns -1.
    
RETURN VALUE
    
    Returns 0 in case of correct work, errno ot -1 in case of an error.    
_____________________________________________________________________

#### [10]  CalculatePolis

SYNOPSIS
   
    double CalculatePolis (char *str, struct variables *table) 

DESCRIPTION
    
    The function calculates expression str in a polish inverse format by using stack
    with operands (see stack.h), getting values from the table of variables by searching
    by indexes, uses CalculateOperation() to make intermediate calculations and returns
    the calculated value. 
    
ERRORS
    
    An error might occur if the stack with operands is empty (or contains 1 number), but 
    the string contains operations. This means the string was incorrect. The function
    prints a diagnostic in the stderr and returns 0.
    
RETURN VALUE
    
    Returns the calculated value or 0 in case of an error.
_____________________________________________________________________

#### [11]  CalculateOperation

SYNOPSIS
   
   double CalculateOperation (double y, double x, char * operand);

DESCRIPTION
    
    The function receives two operands in FIFO order. This was made because this function
    is called in CalculatePolis like CalculateOperation(opop(pt), opop(pt), char), so the
    first popped element is considered to be 'y', and the second is 'x' due to the 
    stack's work specification. 
    
    Then the function calculates and returns the result of binary operation: x operand y.
    
ERRORS
    
    An error might occur if the calculated value does not fit in the double type. In this
    case the function diagnoses an error and returns 0.
    
    An error might occur if the operand equals '/' and y is lower then DBL_EPSILON (very 
    close to the 0 and considered to be 0). In this case function outputs diagnostic 
    about an attempt to divide by zero and returns 0.
    
RETURN VALUE
    
    Returns calculated value or 0 in case of an error.
_____________________________________________________________________

#### [12]  FlushTable

SYNOPSIS
   
    void FlushTable (struct variables *pt)

DESCRIPTION
    
    The function cleans the table of variables from variables with empty name which are 
    considered to be numeric values. It is made for freeing resources after various 
    calculations, as expression may contain a huge amount of numbers.
    
