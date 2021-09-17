#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "stack.h"

struct variables* VarInit(void);
void VarDeinit(struct variables *pt);
int VarisFull(struct variables *pt);
long long int inVar(struct variables *pt, char *new_name);
void VarAdd(struct variables *pt, char* new_name, double new_val);
long long int inVar(struct variables *pt, char *new_name);
void VarPrint(struct variables *pt);
int GetVarFromStr (struct variables *pt, char *str);
int GetCalcInfo (struct variables *pt, char *input_str);
int GeneratePolis (char *str, struct variables *table);
double CalculatePolis (char *str, struct variables *table);
double CalculateOperation (double y, double x, char * operand);
void FlushTable (struct variables *pt);

struct variables
{
	unsigned long int maxsize;	// max capacity of variables
	long long int quantity; //number of variables
	char **name;
	double *value;
};

// Function initializes the table of variables. Also contains variables with empty names,
// which represent numbers used in calculation.
struct variables* VarInit(void)
{
	struct variables *pt = (struct variables*)malloc(sizeof(struct variables));
	if (pt == NULL) {
		perror ("Error while creating the table of variables");
		return NULL;
	}

	pt->maxsize = 10;
	pt->quantity = -1;
	pt->name = (char**)malloc(sizeof(char*)*pt->maxsize);
	pt->value = (double*)malloc(sizeof(double)*pt->maxsize);
    if (pt->name == NULL) {
		perror ("Error while creating the table of variables");
		return NULL;
	}
	if (pt->value == NULL) {
		perror ("Error while creating the table of variables");
		return NULL;
	}
	return pt;
}

// Function deinitializes the table of variables by freeing all the allocated memory.
void VarDeinit(struct variables *pt)
{
	while (pt->quantity > -1) {
		free(pt->name[pt->quantity]);
		pt->quantity--;
	}
	free(pt->name);
	free(pt->value);
	free(pt);
	return;
}

// Function checks if the table of variables is full or not
int VarisFull(struct variables *pt)
{
	return pt->quantity == pt->maxsize - 2;
}

// Function to add a variable new_name with the value new_val to the table of variables
void VarAdd(struct variables *pt, char* new_name, double new_val)
{
	long long int k = inVar(pt, new_name);
	
	if ((strcmp("\0\0\0\0\0\0", new_name))&&(k >= 0)&&(k <= pt->quantity)) {
		//consider empty name to be a number
		pt->value[k] = new_val;
		return;
	}
	
	// check if stack is already full, if so - realloc the memory
	if (VarisFull(pt))
	{
		pt->maxsize += 100;
		pt->name = (char**)realloc(pt->name, sizeof(char**) * pt->maxsize);
	    pt->value = (double*)realloc(pt->value, sizeof(double) * pt->maxsize);
	    if (pt->name == NULL) {
		    perror ("Error while reallocating memory for the table of variables");
		    return;
	    }
	    if (pt->value == NULL) {
		    perror ("Error while reallocating memory for the table of variables");
		    return;
	    }
	}
	// add an element and increments the top index
	pt->quantity++;
	pt->name[pt->quantity] = (char*)malloc(sizeof(char)*6); 
	for (int i = 0; i < 6; i++) pt->name[pt->quantity][i] = new_name[i];
	pt->value[pt->quantity] = new_val;
	return;
}

// Check if variable new_name is already in the table 
// if so - return it's index, else return -1
long long int inVar(struct variables *pt, char *new_name)
{
	if (pt->quantity == -1) return -1;
	
	long long int k = 0;
	while (k < pt->quantity + 1) {
		
		if ((pt->name[k][0] == new_name[0])&&(pt->name[k][1] == new_name[1])&&
		       (pt->name[k][3] == new_name[3])&&(pt->name[k][2] == new_name[2])&&
		           (pt->name[k][4] == new_name[4])&&(pt->name[k][5] == new_name[5]))
		               return k;
		k++;
	}
	return -1;
}

// Utility function, prints the table of variables
void VarPrint(struct variables *pt)
{
	for (long long int k = 0; k < pt->quantity + 1; k++) {
		printf ("%lli | %c%c%c%c%c%c | %f\n", k, pt->name[k][0], pt->name[k][1], \
		    pt->name[k][2], pt->name[k][3], pt->name[k][4], pt->name[k][5], pt->value[k]);
	}
	return;
}

// Function analyzes the string, which suppose to contain a variable. It gets variable's
// name and value given after '=' symbol, then adds it to the table of variables.
// Checks if the string is correct.
int GetVarFromStr (struct variables *pt, char *str)
{
	int k, i;
	if (str[0] == '='){
			fprintf (stderr, "Undefined name of a variable\n");
		    return -1;
	}
	
	for (i = 0; i < strlen(str); i++) {
		if ((str[i]=='-')||(str[i]=='+')||(str[i]=='*')||(str[i]=='/')||(str[i]=='.')||(str[i]==')')||(str[i]=='(')) {
			fprintf (stderr, "Symbols '-', '+', '/', '*', '.', '(', ')' are not allowed\n");
		    return -1;
		}
		
		if (i > 6) {
			fprintf (stderr, "Long variable name\n");
		    return -1;
		}
		
		if ((str[i] == ' ')&&(i > 0)) {
			//maybe spaces between var and '='
			k = i; //the number of symbols which contain var's name
			
			while (str[i] == ' ') i++;
			if (str[i] != '=') {
				fprintf (stderr, "Incorrect input\n");
		        return -1;
			}
			i++;
			//maybe spaces between '=' and digit
			while ((str[i] == ' ')&&(str[i] != '\0')) i++; 
			int j = i; //remember the place where number starts
			int sign = 1;
			//maybe - or + before the number
			if ((str[i] == '+')&&(str[i] != '\0')) {
				i++;
				j++; //shift the place where number starts
			}
			if ((str[i] == '-')&&(str[i] != '\0')) {
				i++;
				j++; //shift the place where number starts
				sign = -1;
			}
			while ((str[i] == ' ')&&(str[i] != '\0')) i++; 
			
			char point = 0;
			while (str[i] != '\0') {
				if ((str[i] == '.')&&(point == 0)) {
					point = 1;
					if (((str[i-1] > '9')||(str[i-1] < '0'))&&(i == strlen(str)))
					{
						// one point in the end of the line
						fprintf (stderr, "1. Incorrect input. Only one point in the end of the line\n"); 
				        puts(str);
				        return -1;
					}
					else if ((i < strlen(str) - 1)&&((str[i-1] > '9')||(str[i-1] < '0'))&&((str[i+1] > '9')||(str[i+1] < '0')))
					{
						// one separate
						fprintf (stderr, "2. Incorrect input. Only one point\n"); 
						 puts(str);
				        return -1;
					}
					i++;
				} else if ((str[i] == '.')&&(point == 1)) {
					fprintf (stderr, "Incorrect input. Multiple points\n"); 
					//more then 1 point
				    return -1;
				}
				
				if ((str[i] != '.')&&(str[i] != ' ')&&((str[i] > '9')||(str[i] < '0'))) {
					fprintf (stderr, "Incorrect symbol %c\n", str[i]); //bad symbols
				    return -1;
				}
				i++;
			}
			double temp = strtod(str + j, NULL);
			char tmp[6];
			for (int i1 = 0; i1 < 6; i1++) {
				if (i1 <= k) {
					tmp[i1] = str[i1];
					if (tmp[i1] == ' ') tmp[i1] = '\0';
				} else {
				    tmp[i1] = '\0';
				}
			}
			if (sign*temp < 0) temp = temp*sign;
			VarAdd(pt, tmp, temp);
			return 0;
		}
		
		if (str[i] == '=') {
			k = i - 1;
			i++;
			//maybe spaces between '=' and digit
			while ((str[i] == ' ')&&(str[i] != '\0')) i++; 
			int j = i; //remember the place where number starts
			//maybe - or + before the number
			if (((str[i] == '-')||(str[i] == '+'))&&(str[i] != '\0')) i++;
			
			char point = 0;
			while (str[i] != '\0') {
				if ((str[i] == '.')&&(point == 0)) {
					point = 1;
					i++;
				} else if ((str[i] == '.')&&(point == 1)) {
					fprintf (stderr, "Incorrect input. Multiple points\n"); 
					//more then 1 point
				    return -1;
				}
				if ((str[i] != ' ')&&(str[i] != '.')&&((str[i] > '9')||(str[i] < '0'))) {
					fprintf (stderr, "Incorrect symbol %c\n", str[i]); //bad symbols
				    return -1;
				}
				i++;
			}
			double temp = strtod(str + j, NULL);
			char tmp[6];
			for (int i1 = 0; i1 < 6; i1++) {
				if (i1 <= k) {
					tmp[i1] = str[i1];
				} else {
				    tmp[i1] = '\0';
				}
			}
			VarAdd(pt, tmp, temp);
			return 0;
		}
	}
	return 0;
}

// Function analyzes information from the string given in input_str. It may be a stdin or
// a file name. It predicts if the string contains variable (then calls GetVarFromStr())
// or the string contains an expression to calculate (calls generate polis).
// Analizes strings until EOF.
int GetCalcInfo (struct variables *pt, char *input_str) 
{
    char *str = malloc(100*sizeof(char));
    if (str == NULL) {
        perror("Error while getting information: can not allocate memory.\n");
        free(str);
        return errno;
    }
    char c, flag = 1; //if flag = 0 it is var init, if 1 - string to calculate
    int len = 1;
    
    if (!strcmp(input_str,"stdin")) {
		printf("  Enter variables and expression to calculate in separate strings.\n");
        printf("  Format [name | = | value] initializes a variable name with value.\n");
        printf("  Expression to calculate must contain only float numbers, name of\n");
        printf("variables, symbols '-', '+', '/', '*', '.', '(' or ')'\n");
        
        while ((c = fgetc(stdin)) != EOF) {
			//new string
			while (c == ' ') c = fgetc(stdin); //Searching for the first symbol
			str[len - 1] = c;
            len++;
			while(((c = fgetc(stdin)) != '\n') && (c != EOF)) {
				if (c == '=') flag = 0;
                str[len - 1] = c;
                len++;
			    if (len % 100 == 0) {
                    str = realloc(str, len + 100);
                    if (str == NULL) {
                        perror("Error: can not reallocate memory.\n");
                        free (str);
                        return errno;
                    }
                }
            }
            str[len - 1] = ' ';
            len++;
			if (len % 100 == 0) {
                str = realloc(str, len + 100);
                if (str == NULL) {
                    perror("Error: can not reallocate memory.\n");
                    free (str);
                    return errno;
                }
            }
            str[len - 1] = '\0';
            if (flag == 0) {
				//init a variable
				GetVarFromStr (pt, str);
            } else {
				//calc string function
				GeneratePolis(str ,pt);
            }
			
			//free string
		    if (len > 100) {
                str = realloc(str, 100);
            }
            for (int i = 0; i < 100; i++) {
                str[i] = ' ';
            }
            len = 1;
            flag = 1;
        }
	    free (str);
	    
	} else {
		
		FILE* f = fopen (input_str, "r");
        if (f == NULL) {
			fprintf(stderr, "Error. Can not open the file.\n");
            free(str);
            return errno;
		}
        
        while ((c = fgetc(f)) != EOF) {
			//new string
			while (c == ' ') c = fgetc(f); //Searching for the first symbol
			str[len - 1] = c;
            len++;
			while(((c = fgetc(f)) != '\n') && (c != EOF)) {
				if (c == '=') flag = 0;
                str[len - 1] = c;
                len++;
			    if (len % 100 == 0) {
                    str = realloc(str, len + 100);
                    if (str == NULL) {
                        perror("Error: can not reallocate memory.\n");
                        free (str);
                        return errno;
                    }
                }
            }
            str[len - 1] = ' ';
            len++;
			if (len % 100 == 0) {
                str = realloc(str, len + 100);
                if (str == NULL) {
                    perror("Error: can not reallocate memory.\n");
                    free (str);
                    return errno;
                }
            }
            str[len - 1] = '\0';
                
            if (flag == 0) {
				//init a variable
				GetVarFromStr (pt, str);
            } else {
				//calc string function
				GeneratePolis(str ,pt);
            }
			
			//free string
		    if (len > 100) {
                str = realloc(str, 100);
            }
            for (int i = 0; i < 100; i++) {
                str[i] = ' ';
            }
            len = 1;
            flag = 1;
        }
	    free (str);
		fclose (f);
	}
    return 0;
}

// This function generates polis from the given string. Polis string contains
// indexes in the table of variables, operands *, /, +, -, separated with '[' before index
// and ']' before an operand (makes it easier to calculate). Then calls function 
// CalculatePolis(). Also checks sintax errors.
int GeneratePolis (char *str, struct variables *table)
{
	// After '[' goes a number or a variable
	// After ']' goes operand
    struct stack *pt = StackInit();
    unsigned long long int i = 0, j = 0, length = strlen(str);
    int num_br = 0;
    char *polis = (char*)malloc((length + table->maxsize*10)*sizeof(char));
    if (polis == NULL) {
	    perror ("Error while allocating memory for the polis");
	    free(polis);
	    StackDeinit(pt);
        return errno;
	}
    for (unsigned long long int m = 0; m < length+table->maxsize*10; m++) polis[m] = '\0';
    
    char tmpnum[16], tmpvar[6] = "\0\0\0\0\0\0";
    for (int k = 0; k < 16; k++) tmpnum[k] = '\0';
    int tmp_i = 0, k = 0;
    
    // flag == 1 - just read number or variable. Expect ) or operator.
    // flag == 2 - just read operator (1 symbol). Expect (, ), variable or number.
    // flag == 3 - just read (, expect number, - or +.
    
    char c, flag = 0;
    //maybe the string starts with the '-' or '+', then add 0 to polis
    while ((str[i] == ' ')&&(i < length)) i++;
    
    if ((str[i] == '-')||(str[i] == '+')) {
		VarAdd(table, "\0\0\0\0\0\0", 0);
		char insert_index[20];
        sprintf(insert_index, "%lli", table->quantity);
        polis[j] = '[';
        j++;
        strcat(polis, insert_index);
        j = strlen(polis);
		flag = 1;
	}
    
    while (i < length) {
GeneratePolisPoint2:
		while ((str[i] == ' ')&&(i < length)) i++;
		
		if ((flag == 2)&&(i < length)&&((str[i] == '-')||(str[i] == '+')
		        ||(str[i] == '*')||(str[i] == '/'))) {
			fprintf (stderr, "2 or more operations in a row.\n");
			free(polis);
			StackDeinit(pt);
			return -1;
		}
		
		if ((str[i] <= '9')&&(str[i] >= '0')&&(flag == 1)&&(i < length)) {
			fprintf (stderr, "2 or more numbers in a row.\n");
			free(polis);
			StackDeinit(pt);
			return -1;
		}
		
		if (((str[i] == '*')||(str[i] == '/'))&&(flag == 3)&&(i < length)) {
			fprintf (stderr, "A '*' or '/' after an open bracket.\n");
			free(polis);
			StackDeinit(pt);
			return -1;
		}
		
		if ((str[i] == '(')&&(flag == 1)&&(i < length)) {
			fprintf (stderr, "Missing an operand between number and bracket.\n");
			free(polis);
			StackDeinit(pt);
			return -1;
		}
		
		//push an opening bracket 
		if ((str[i] == '(')&&(i < length)) {
			num_br++;
			push(pt, '(');
			i++;
			flag = 3;
			//maybe '-' after an opening bracket before the operand
			while ((str[i] == ' ')&&(i < length)) i++;
    
            if (str[i] == '-') {
		        VarAdd(table, "\0\0\0\0\0\0", 0);
		        char insert_index[20];
                sprintf(insert_index, "%lli", table->quantity);
                polis[j] = '[';
                j++;
                strcat(polis, insert_index);
                j = strlen(polis);
	            flag = 1;
	        }
	        goto GeneratePolisPoint2;
		}
		
		//push a closing bracket 
		if ((str[i] == ')')&&(i < length)) {
			num_br--;
			if (num_br < 0) {
				fprintf (stderr, "Brackets are disbalanced.\n");
				free(polis);
				StackDeinit(pt);
			    return -1;
			}
			while (((c = peek(pt)) != '(')&&(c != 0)) {
				polis[j] = ']';
				j++;
				polis[j] = pop(pt);
				j++;
			}
			if ((c != '(')&&(isEmpty(pt))) {
				fprintf (stderr, "Brackets are disbalanced.\n");
				free(polis);
				StackDeinit(pt);
			    return -1;
			}
			pop(pt);
			i++;
			flag = 1; //consider construction with brackets to be a number
			goto GeneratePolisPoint2;
		}
		
		// pop an operation with a higher priority
		if (((str[i] == '-')||(str[i] == '+'))&&(flag != 2)&&(i < length)) {
			while (((c = peek(pt)) == '*')||(c == '/')||(c == '+')||(c == '-')) {
				polis[j] = ']';
				j++;
				polis[j] = pop(pt);
				j++;
			}
			push(pt, str[i]);
			i++;
			flag = 2;
			goto GeneratePolisPoint2;
		}
		
		// pop an operation with a higher priority
		if (((str[i] == '/')||(str[i] == '*'))&&(flag != 2)&&(i < length)) {
			while (((c = peek(pt)) == '*')||(c == '/')) {
				polis[j] = ']';
				j++;
				polis[j] = pop(pt);
				j++;
			}
			push(pt, str[i]);
			i++;
			flag = 2;
			goto GeneratePolisPoint2;
		}
		
		//read a float number
		if (((str[i] == '.')||((str[i] <= '9')&&(str[i] >= '0')))&&(i < length)) {
			while ((str[i] <= '9')&&(str[i] >= '0')&&(i < length)) {
				tmpnum[tmp_i] = str[i];
				tmp_i++;
				i++;
			}
			//Met a point. In case of missing point do not enter
			if ((str[i] == '.')&&(i < length)) {
				tmpnum[tmp_i] = str[i];
				tmp_i++;
				//i++;
				if ((i > 0)&&((str[i-1] > '9')||(str[i-1] < '0'))&&(i == length - 2))
				{
					// one point in the end of the line
					fprintf (stderr, "3. Incorrect input. Only one point\n");
					free(polis);
					StackDeinit(pt);
			        return -1;
				}
				else if ((i > 0)&&(i < length - 2)&&((str[i-1] > '9')||(str[i-1] < '0'))&&((str[i+1] > '9')||(str[i+1] < '0')))
				{
					// one separate
					fprintf (stderr, "4. Incorrect input. Only one point\n"); 
					free(polis);
					StackDeinit(pt);
			        return -1;
				}
				i++;
			}
			//Read number after a point. In case of missing point do not enter
			while ((str[i] <= '9')&&(str[i] >= '0')&&(i < length)) {
				tmpnum[tmp_i] = str[i];
				tmp_i++;
				i++;
			}
			if ((str[i] != ' ')&&(str[i] != '+')&&(str[i] != '-')&&(str[i] != '*')
			        &&(str[i] != '/')&&(str[i] != ')')&&(i < length)&&(tmp_i < 6)) {
				//maybe a name of variable
				for (int m = 0; m <= tmp_i; m++) tmpvar[m] = tmpnum[m];
				for (int m = 0; m < 16; m++) tmpnum[m] = '\0';
				k = tmp_i;
				tmp_i = 0;
				goto GeneratePolisPoint1;		
			}
			
			if ((str[i] == '.')&&(i < length)) {				
			    fprintf (stderr, "2 or more points in one number.\n");
			    free(polis);
			    StackDeinit(pt);
			    return -1;
			}
			
		    flag = 1;
		    VarAdd(table, "\0\0\0\0\0\0", strtof(tmpnum, NULL));
		    tmp_i = 0;
		    for (int l = 0; l < 16; l++) tmpnum[l] = '\0';
		    
		    char insert_index[20];
            sprintf(insert_index, "%lli", table->quantity);
            polis[j] = '[';
            j++;
            strcat(polis, insert_index);
            j = strlen(polis);
		    flag = 1;
		    goto GeneratePolisPoint2;
		}
		//if we are here, we see an unusual symbol
		k = 0;
GeneratePolisPoint1:
		while ((str[i] != '*')&&(str[i] != '-')&&(str[i] != '+')&&(str[i] != '/')
		        &&(str[i] != ')')&&(str[i] != '(')&&(str[i] != ' ')&&(str[i] != '\0')
		        &&(k < 6)&&(i < length)){
		    tmpvar[k] = str[i];
		    i++;
		    k++;
		}
		
		if ((k == 6)&&((str[i] != '*')&&(str[i] != '-')&&(str[i] != '+')&&(str[i] != '/')
		        &&(str[i] != ')')&&(str[i] != '(')&&(str[i] != ' ')&&(str[i] != '\0')
		        &&(i < length))) {
			fprintf (stderr, "Unknown long name of a variable.\n");
			free(polis);
			StackDeinit(pt);
			return -1;
		}
		if (k > 0) {
		    for (int d = k+1; d < 6; d++) tmpvar[k] = '\0';
		    long long int my_i = inVar(table, tmpvar);
            if (my_i == -1) {
			    fprintf (stderr, "Unknown name of a variable.\n");
			    free(polis);
			    StackDeinit(pt);
			    return -1;
		    }
		    if (flag == 1) {
				fprintf (stderr, "2 or more operands in a row.\n");
			    free(polis);
			    StackDeinit(pt);
			    return -1;
			}
		    polis[j] = '[';
            j++;
		    char insert_index[20];
            sprintf(insert_index, "%lli", my_i);
            strcat(polis, insert_index);
            j = strlen(polis);
		    flag = 1;
		    for (int m = 0; m < 6; m++) tmpvar[m] = '\0';
	    }
	}
	
	while ((!isEmpty(pt))&&((c = pop(pt)) != '(')) {
		polis[j] = ']';
        j++;
		polis[j] = c;
	    j++;
	}
	polis[j] = '\0';
	if ((!isEmpty(pt))&&((c = pop(pt)) == '('))  {
		fprintf (stderr, "Brackets are disbalanced.\n");
		free(polis);
		StackDeinit(pt);
        return -1;
	}
	if (num_br != 0) {
		fprintf (stderr, "Brackets are disbalanced.\n");
		free(polis);
		StackDeinit(pt);
        return -1;
	}
	polis[j] = '\0';
	if (length > 0) {
		VarAdd(table, "result", CalculatePolis(polis, table));
		printf("%s = %f\n",str, CalculatePolis(polis, table));
		printf("-----------------------------------------\n");
	}
	i = 0;
	FlushTable (table);
	free(polis);
	StackDeinit(pt);
	return 0;
}

// This function calculates polis from the given polis string. It creates a stack of 
// operands and calculates intermediate values by calling CalculateOperation() function.
// Also checks if the string is incorrect (ex. trying to calculate binary operation, but
// the stack is already empty).
double CalculatePolis (char *str, struct variables *table) 
{
	unsigned long long int i = 0, length = strlen(str);
	long int index; //index of a variable in the table of parameters
	char c;
	struct ostack *pt = oStackInit();
	while (i < length) {
		sscanf(str + i, "%c", &c);
		i++;
		if (c == '[') {
			sscanf(str + i, "%li", &index);
			opush(pt, table->value[index]);
			while ((str[i] != ']')&&(str[i] != '[')&&(i < length)) i++;
		} else {
			sscanf(str + i, "%c", &c);
			i++;
			if (oisEmpty(pt)) {
				fprintf(stderr, "Wrong input\n");
				oStackDeinit(pt);
				return 0;
			}
			double x = opop(pt);
			if (oisEmpty(pt)) {
				fprintf(stderr, "Wrong input\n");
				oStackDeinit(pt);
				return 0;
			}
			double y = opop(pt);
			opush(pt, CalculateOperation(y, x, &c));
			if (c == 0) {
				oStackDeinit(pt);
				return 0;
			}
		}
	}
	double res = opop(pt);
	oStackDeinit(pt);
	return res;
}

// The function returns the result of binary operation '*', '/', '+' or '-' given in the 
// operand. Also checks overflow errors and attempt to divide by zero.
double CalculateOperation (double x, double y, char * operand)
{
	long double res;
	switch (*operand) {
	case '+': 
	    res = x + y;
	    if ((res > DBL_MAX)||(res < -DBL_MAX)) {
			*operand = 0;
			fprintf (stderr, "Overflow while counting\n");
			return 0;
		}
		break;
	case '-': 
	    res = x - y;
	    if ((res > DBL_MAX)||(res < -DBL_MAX)) {
			*operand = 0;
			fprintf (stderr, "Overflow while counting\n");
			return 0;
		}
		break;
	case '*': 
	    res = x*y;
	    if ((res > DBL_MAX)||(res < -DBL_MAX)) {
			*operand = 0;
			fprintf (stderr, "Overflow while counting\n");
			return 0;
		}
		break;
	case '/': 
	    if ((y <= DBL_EPSILON)&&(y >= -DBL_EPSILON)) {
		    fprintf (stderr, "Divide by zero\n");
		    *operand = 0;
		    return 0;
	    } else {
		    res = x/y;
	        if ((res > DBL_MAX)||(res < -DBL_MAX)) {
			    *operand = 0;
		        fprintf (stderr, "Overflow while counting\n");
		        return 0;
		    }
		    break;
	    }
	default: break;
	}
	return res;
}

// The function cleans the table of operands from all the operands with empty names, which
// considered to be numbers.
void FlushTable (struct variables *pt)
{
	while ((pt->quantity > -1)&&(!strcmp(pt->name[pt->quantity], "\0\0\0\0\0\0"))) {
		free(pt->name[pt->quantity]);
		pt->quantity--;
	}
	return;
}
