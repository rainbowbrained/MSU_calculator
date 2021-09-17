#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "polis.h"

int main (int argc, char **argv) 
{
	if (argc < 2) {
		struct variables *table = VarInit();
		char buf[] = "stdin";
		GetCalcInfo(table, buf);
		if (inVar(table, "result") != -1) {
		    double result = table->value[inVar(table, "result")];
		    printf("Result = %f\n", result);
		}
		VarDeinit(table);
	} else {
		struct variables *table = VarInit();
		GetCalcInfo(table, argv[1]);
		if (inVar(table, "result") != -1) {
		    double result = table->value[inVar(table, "result")];
		    printf("Result = %f\n", result);
		}
		VarDeinit(table);
	}
	return 0;
}

//
