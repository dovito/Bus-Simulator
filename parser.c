#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#define DELIMITER " \t\n"

int errnum;
int was_error = 0;
// below are defined external variables that will keep values 
// retrieved from inpt file 
extern unsigned int busCapacity;
extern unsigned int boardingTime;
extern float requestRate;
extern float pickupInterval;
extern unsigned int maxDelay;
extern unsigned int noBuses;
extern unsigned int noStops ;
extern int **map;
extern float stopTime;
extern int numberOfExperiments;
// below are arrays that saves corresponding experiment variable values 
extern int *busCapacityExp;
extern int *boardingTimeExp;
extern float *requestRateExp;
extern float *pickupIntervalExp;
extern int *maxDelayExp;
extern int *noBusesExp;
extern float *stopTimeExp;
// below are lengths of corresponding experiment variable arrays
extern int busCapacityExpLength;
extern int boardingTimeExpLength;
extern int requestRateExpLength;
extern int pickupIntervalExpLength;
extern int maxDelayExpLength;
extern int noBusesExpLength;
extern int stopTimeExpLength;
// was_allocated[] tracks which variables are used for experimentation
// and also tracks which arrays were allocated in memory
extern int was_allocated[7];
// below are defined variables that will keep experiment variable array rotation intervals during the experimentation
extern int capacityRot, boardTimeRot, requestRot, pickupRot, delayRot, busesRot, stopsRot, stopTimeRot;
// integer col will be the number of columns that 'map' has. It will be used for defining matrix size
int col = 0;
// myStrToInt() converts string s into an integer 
int myStrToInt(char *s, int l){
	char *string;
	long number;
    number = strtol(s, &string, 10);
    if(*string == '\0'){
    	return number;
    } else {
    	printf("Could not convert to integer \"%s\". Input file line: %d\n", s, l);
    	was_error = 1;
    	return 0;
    }   
}
// myStrToUnsigned() converts string s to an unsigned integer
int myStrToUnsigned(char *s, int l){
	int num = myStrToInt(s, l);
	if(num < 0){
		printf("Could not convert to unsigned integer \"%s\". Input file line: %d\n", s,l);
    	was_error = 1;
    	return 0;
	} else {
		return num;
	}
}
// myStrToFloat() converts strings to a float
float myStrToFloat(char *s, int l){
	char *string;
	float number;
    number = strtof(s, &string);
    if(*string == '\0'){
    	return number;
    } else {
    	printf("Could not convert to float \"%s\". Input file line: %d\n", s,l);
    	was_error = 1;
    	return 0;
    }   
}
// increase_experiment_number() increases the number of experiments
void increase_experiment_number(int i){
	if(numberOfExperiments > 0){
		numberOfExperiments *= i;
	} else {
		numberOfExperiments = i;
	}
}
// get_inputs() contains the loop that scans input file, tokens it, and
// retrieves needed variable values. As input file name it uses passed string
// argument s
void get_inputs(char *s) {
	// define pointer to file and open try to ipen file with its given name s
	FILE *pFile;
	pFile = fopen(s, "r");
	// if it fails to open thefile
	if(!pFile){
		// add .txt extension and try again to open
		strcat(s, ".txt");
		pFile = fopen(s, "r");
		// if it fails again, then following error is raised and program exits
		if(!pFile){
	      perror("Error while opening file"); 
	      printf("Please check if file name is correct and it is in the working directory. \n");
	      exit(1);
		}
	}
// intialize number of experiments
numberOfExperiments = 0;
// define and allocate caharacter array 'buffer' into which scanned line of unput file 
// will be saved. Array has maximum size of 65535
// thus the line cannot have more than 65535 characters per line
char *buffer;
buffer = (char *) malloc(sizeof(char)*65535);
// define character array 'token' into which tokenized string will be copied
char *token;
// previous will have the value of previously seen variable keyword
char previous[10];
memset(previous, '\0', sizeof(previous));
// tmp will be used for saving the first line of map adjacency matrix.
// Maximum size of 'tmp' cannot exceed 65535.
int *tmp;
tmp = (int *) malloc(sizeof(int)*65535);
// variable line tracks which input file line is being currently scanned.
// Ths variable will be used to inform user at which line parsing error occured
int line = 0;
    
	// Read input line by line and save it into the buffer
	while(fgets(buffer, 65535, pFile) != NULL){
		line++;
		// tokenise buffer by defined delimiter and save it into token
	    token = strtok(buffer, DELIMITER);
	    // while loop below tokens buffer until the end of line is reached
	    while( token != NULL ){ 
	    	// if current token is #, then it means that the line is a comment
	    	// and line can be skipped
	    	if(!strcmp(token, "#")){
	    		break;
	    	}
	    	// if busCapacity keyword is being parsed
	    	if(!strcmp(token,"busCapacity")){
	    		token = strtok(NULL, DELIMITER);
	    		// check wheteher it is an experiment
	    		if(!strcmp(token, "experiment")){
	    			// if it is experiment, allocate array that keeps experimentation values
	    			busCapacityExp = (int *) malloc(sizeof(int*)*100);
	    			// set first element of was_allocated[] to 1 so that it would be known
	    			// that busCapacityExp[] was allocated
	    			was_allocated[0] = 1;
	    			// keep parsing and retrieve experiment values until end of line is reached
	    			token = strtok(NULL, DELIMITER);
	    			int k = 0;
	    			while(token != NULL){
	    				busCapacityExp[k] = myStrToUnsigned(token, line);
	    				token = strtok(NULL, DELIMITER);
	    				k++;	
	    			}
	    			// assign to busCapacityExpLength how many values were retrieved
	    			busCapacityExpLength = k;
	    			// increase experiment number
	    			increase_experiment_number(k);
	    			break;
	    		} else {
	    			// else if it is not experimentation
	    			// then experimentation value array was not allocated
	    			was_allocated[0] = 0;
	    			// convert token into unsigned integer and asign it to busCapacity
	    			busCapacity = myStrToUnsigned(token, line);
	    		}
		    	
	    	}
	    	// boardingTime keyword is being parsed
	    	if(!strcmp(token,"boardingTime")){
	    		token = strtok(NULL, DELIMITER);
	    		// experiment keyword is being parsed
	    		if(!strcmp(token, "experiment")){
	    			// retrieve experimentation values and allocate array that saves them
	    			boardingTimeExp = (int *) malloc(sizeof(int*)*100);
	    			was_allocated[1] = 1;
	    			token = strtok(NULL, DELIMITER);
	    			int k = 0;
	    			while(token != NULL){
	    				boardingTimeExp[k] = myStrToUnsigned(token, line);
	    				token = strtok(NULL, DELIMITER);
	    				k++;	
	    			}
	    			boardingTimeExpLength = k;
	    			increase_experiment_number(k);
	    			break;
	    		} else {
	    			was_allocated[1] = 0;
	    			// retrieve boardingTime value
	    			boardingTime = myStrToUnsigned(token, line);
	    		}	

	    	}
	    	// requestRate keyword is being parsed
	    	if(!strcmp(token,"requestRate")){
	    		token = strtok(NULL, DELIMITER);
	    		// experiment keyword is being parsed
	    		if(!strcmp(token, "experiment")){
	    			// retrieve experimentation values and allocate array that saves them
	    			requestRateExp = (float *) malloc(sizeof(float*)*100);
	    			was_allocated[2] = 1;
	    			token = strtok(NULL, DELIMITER);
	    			int k = 0;
	    			while(token != NULL){
	    				requestRateExp[k] = myStrToFloat(token, line);
	    				token = strtok(NULL, DELIMITER);
	    				k++;	
	    			}
	    			requestRateExpLength = k;
	    			increase_experiment_number(k);
	    			break;
	    		} else {
	    			was_allocated[2] = 0;
	    			// retrieve requestRate value
	    			requestRate = myStrToFloat(token, line);
	    		}	
	    	}
	    	// pickupInterval keyword is being parsed
	    	if(!strcmp(token,"pickupInterval")){
	    		token = strtok(NULL, DELIMITER);
	    		// experiment keyword is being parsed
	    		if(!strcmp(token, "experiment")){
	    			// retrieve experimentation values and allocate array that saves them
	    			pickupIntervalExp = (float *) malloc(sizeof(float*)*100);
	    			was_allocated[3] = 1;
	    			token = strtok(NULL, DELIMITER);
	    			int k = 0;
	    			while(token != NULL){
	    				pickupIntervalExp[k] = myStrToFloat(token, line);
	    				token = strtok(NULL, DELIMITER);
	    				k++;	
	    			}
	    			pickupIntervalExpLength = k;
	    			increase_experiment_number(k);
	    			break;
	    		} else {
	    			was_allocated[3] = 0;
	    			// retrieve pickupInterval value
	    			pickupInterval = myStrToFloat(token, line);
	    		}	
	    	}
	    	// maxDelay keyword is being parsed
	    	if(!strcmp(token,"maxDelay")){
	    		token = strtok(NULL, DELIMITER);
	    		// experiment keyword is being parsed
	    		if(!strcmp(token, "experiment")){
	    			// retrieve experimentation values and allocate array that saves them
	    			maxDelayExp = (int *) malloc(sizeof(int*)*100);
	    			was_allocated[4] = 1;
	    			token = strtok(NULL, DELIMITER);
	    			int k = 0;
	    			while(token != NULL){
	    				maxDelayExp[k] = myStrToUnsigned(token, line);
	    				token = strtok(NULL, DELIMITER); 
	    				k++;	
	    			}
	    			maxDelayExpLength = k;
	    			increase_experiment_number(k);
	    			break;
	    		} else {
	    			was_allocated[4] = 0;
	    			// retrieve maxDelay value
	    			maxDelay = myStrToUnsigned(token, line);
	    		}
	    	}
	    	// noBuses keyword is being parsed
	    	if(!strcmp(token,"noBuses")){
	    		// experiment keyword is being parsed
	    		token = strtok(NULL, DELIMITER);
	    		if(!strcmp(token, "experiment")){
	    			// retrieve experimentation values and allocate array that saves them
	    			noBusesExp = (int *) malloc(sizeof(int*)*100);
	    			was_allocated[5] = 1;
	    			token = strtok(NULL, DELIMITER);
	    			int k = 0;
	    			while(token != NULL){
	    				noBusesExp[k] = myStrToUnsigned(token, line);
	    				token = strtok(NULL, DELIMITER);
	    				k++;	
	    			}
	    			noBusesExpLength = k;
	    			increase_experiment_number(k);
	    			break;
	    		} else {
	    			was_allocated[5] = 0;
	    			// retrieve noBuses value
	    			noBuses = myStrToUnsigned(token, line);
	    		}
	    	}
	    	// noStops is being parsed
	    	if(!strcmp(token,"noStops")){
	    		// retrieve noStops value
	    		// experimentation with different number of stops is not allowed
	    		token = strtok(NULL, DELIMITER);
	    		noStops = myStrToUnsigned(token, line);
	    	}
	    	// stopTime keyword is being parsed
	    	if(!strcmp(token,"stopTime")){
	    		token = strtok(NULL, DELIMITER);
	    		// experiment keyword is being parsed
	    		if(!strcmp(token, "experiment")){
	    			stopTimeExp = (float *) malloc(sizeof(float*)*100);
	    			was_allocated[6] = 1;
	    			token = strtok(NULL, DELIMITER);
	    			int k = 0;
	    			while(token != NULL){
	    				stopTimeExp[k] = myStrToFloat(token, line);
	    				token = strtok(NULL, DELIMITER);
	    				k++;	
	    			}
	    			stopTimeExpLength = k;
	    			increase_experiment_number(k);
	    			break;
	    		} else {
	    			was_allocated[6] = 0;
	    			// retrieve experimentation values and allocate array that saves them
	    			stopTime = myStrToFloat(token, line);
	    		}
	    	}
	    	// map keyword is being parsed
	    	// note: experimentation with different maps is not allowed
	    	if(!strcmp(token,"map")){
	    		fgets(buffer, 65535, pFile);
	    		line++;
	    		token = strtok(buffer, DELIMITER);
	    		// tokenise buffer until end of line is reached
	    		while(token != NULL){
	    			// write converted map values into tmp[]
	    			tmp[col] = myStrToInt(token, line);
	    			token = strtok(NULL, DELIMITER);
	    			// increase number of columns
	    			col++;
	    		}

	    		// memory allocation for map variable matrix
	    		map = (int **) malloc(sizeof(int*)*col);
	    		int j;
	    		for(j=0;j<col;j++){
	    			map[j] = (int *) malloc(sizeof(int*)*col);
	    		} 
	    		// copy all values from temporary array into map
	    		for(j=0;j<col;j++){
	    			map[0][j] = tmp[j];
	    		}
	    		// free allocated memory for temporary variable tmp
	    		free(tmp);	
	    		int i;
	    		// keep scanning and parsing the rest of map lines 
	    		// and write retrieved values straight into map variable matrix
	    		for(i=1; i<col;i++){
	    			fgets(buffer, 65535, pFile);
	    			line++;
	    			token = strtok(buffer, DELIMITER);
	    			for(j=0;j<col;j++){
	    				map[i][j] = myStrToInt(token, line);
	    				token = strtok(NULL, DELIMITER);
	    			}	
	    		}
	    	}
	    	token = strtok(NULL, DELIMITER);  
   		}
	}
	// calculate rotation inetrvals for each experimantation variable arrays
	int rotation = numberOfExperiments;
	if(busCapacityExpLength > 0){
        busCapacity = busCapacityExp[0];
        capacityRot = (int) rotation / busCapacityExpLength;
        rotation = capacityRot;
    }
    if(boardingTimeExpLength > 0){
        boardingTime = boardingTimeExp[0];
        boardTimeRot = (int) rotation / boardingTimeExpLength;
        rotation = boardTimeRot;
    }
    if(requestRateExpLength > 0){
        requestRate = requestRateExp[0];
        requestRot = (int) rotation / requestRateExpLength;
        rotation = requestRot;
    }
    if(pickupIntervalExpLength > 0){
        pickupInterval = pickupIntervalExp[0];
        pickupRot = (int) rotation / pickupIntervalExpLength;
        rotation = pickupRot;
    }
    if(maxDelayExpLength > 0){
        maxDelay = maxDelayExp[0];
        delayRot = (int) rotation / maxDelayExpLength;
        rotation = delayRot;
    }
    if(noBusesExpLength > 0){
        noBuses = noBusesExp[0];
        busesRot = (int) rotation / noBusesExpLength;
        rotation = busesRot;
    }
    if(stopTimeExpLength > 0){
        stopTime = stopTimeExp[0];
        stopTimeRot = (int) rotation / stopTimeExpLength;
        rotation = stopTimeRot;
    }	
// close input file
fclose(pFile);
// free memory allocated variables that will not be used anymore
free(buffer);    
free(token);

// check if given number of stops matches with the number of columns and rows in the 
// given map matrix
if(col != (int) noStops){
	// if it does not match, indicate that there was an error
	was_error = 1;
	printf("Number of bus stops does not match with number of columns and rows in the map matrix or wrong keyword\n");
}
// if there any error occured, free memory allocated vriables
// and exit the program
	if(was_error){
		int i;
		for(i = 0; i<noStops;i++){
	        free(map[i]);
	    }
	    free(map);

	    for(i=0; i<7;i++){
	        if(was_allocated[i] == 1){
	            switch(i){
	                case 0 : free(busCapacityExp); break;
	                case 1 : free(boardingTimeExp); break;
	                case 2 : free(requestRateExp); break;
	                case 3 : free(pickupIntervalExp); break;
	                case 4 : free(maxDelayExp); break;
	                case 5 : free(noBusesExp); break;
	                case 6 : free(stopTimeExp); break;
	            }
	        }
	    }
	    printf("Program exited\n");
	    exit(1);
	} else {
		// If program did not exit, check retrieved inputs and raise 
		// corresponding warnings:
		if(busCapacity == 0){
		printf("Warning: busCapacity was given as 0 or incorrect keyword\n");
		}
		if(boardingTime == 0){
			printf("Warning: boardingTime was given as 0 or incorrect keyword\n");
		}
		if(requestRate == 0){
			printf("Warning: requestRate was given as 0 or incorrect keyword\n");
		}
		if(pickupInterval == 0){
			printf("Warning: pickupInterval was given as 0 or incorrect keyword\n");
		}
		if(maxDelay == 0){
			printf("Warning: maxDelay was given as 0 or incorrect keyword\n");
		}
		if(noStops == 0){
			printf("Warning: noStops was given as 0 or incorrect keyword\n");
		}
		if(stopTime == 0){
			printf("Warning: stopTime was given as 0 or incorrect keyword\n");
		}	
	}  
	
}    
