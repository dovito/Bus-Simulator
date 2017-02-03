#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parser.h"
#include "simulate.h"

    
int main(int argc, char **argv){

	if(argc < 2){
		printf("No parameters were passed\n");
		exit(1);
	} 
	if(argc > 2){
		printf("Too many arguments\n");
		exit(1);
	} 
	// pas command line parameter into get_inputs() function
	// and read input file in
    get_inputs(argv[1]);
    // start simulation
    control_simulation();

    return 0;
    
}
