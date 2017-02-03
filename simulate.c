#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include "output.h"
#include "path.h"
#include "simulate.h"
#define infinity 999999

unsigned int busCapacity;
unsigned int boardingTime;
float requestRate;
float pickupInterval;
unsigned int maxDelay;
unsigned int noBuses;
unsigned int noStops;
int **map;
float stopTime;

int numberOfExperiments;

int *busCapacityExp;
int busCapacityExpLength;
int *boardingTimeExp;
int boardingTimeExpLength;
float *requestRateExp;
int requestRateExpLength; 
float *pickupIntervalExp;
int pickupIntervalExpLength;
int *maxDelayExp;
int maxDelayExpLength;
int *noBusesExp;
int noBusesExpLength;
float *stopTimeExp;
int stopTimeExpLength;
int was_allocated[7];
int capacityRot, boardTimeRot, requestRot, pickupRot, delayRot, busesRot, stopTimeRot;
char buffer[201];

// define Bus structure for a bus that holds data about the bus
    struct Bus {
        // number of passengers currently on board
        int passengersOnBoard;
        // if bus arranges to pick up a apassenger, seat is reserved for him
        int reservedSeats[6500]; 
        // bus stops from which bus picking up passengers (planned route)
        int route[6500]; 
        // number of stops that bus will visit
        int routeLength;
        // bus stop at which bus currently is
        int currentStop;
        // the bus stop that is last to visit   
        int lastScheduledStopIndex;
        // leaving time (in seconds) to a respective bus stop in a route 
        int departureTime[6500];
        // leaving time (in seconds) to a respective bus stop in a route
        int arrivalTime[6500];
        // arrived tracks whether bus arrived to the bus stop to pick or disembark passenger
        int arrived;
        // tripDuration[] tracks actual trip duration to the bus stop parralel in the bus route
        int tripDuration[6500];
        // theoreticalDuration[] tracks shortest trip duration to the bus stop parralel in the bus route
        int theoreticalDuration[6500];
        // destination bus stops that every passanger goes to
        int passengersToDisembark[6500];
        // waiting[] tracks for how many passengers bus will be waiting at the bus stop parralel to the stops
        // int the bus route
        int waiting[6500];
        // tracks travel of the whole route (only when bus is moving - excepts waiting, boarding and disembarking at the stop)
        int routeDuration;
        //  tracks the total number of passengers transported between each stop in the bus route
        int passengersTransported;
    };
// rotateIntArray() rotates given integer array 
// e.g. [1,2,3] after rotation looks like [2,3,1]    
void rotateIntArray(int *arr, int len){
    int i;
    int temp = arr[0];
    if (len > 1){
        for(i=1; i< len;i++){
            arr[i-1] = arr[i];
        } 
        arr[len-1] = temp;
    }
}  
// rotateFloatArray() rotates given array of floats
// e.g.[10.0,15.0,20.0] after rotation is [15.0,20.0,10.0]  
void rotateFloatArray(float *arr, int len){
    int i;
    float temp = arr[0];
    if(len > 1){
        for(i=1; i< len;i++){
            arr[i-1] = arr[i];
        } 
        arr[len-1] = temp;
    }
}
// control_simulation() function controls experiments or if there are no experiments
// to be performed then it performs one-off simulation
void control_simulation(){
    // time variable t that will be passed to simulate() function
    time_t t = time (NULL);
    // integers i and j are used for loops
    int i,j;
    // for loop below runs all possible combinations of experiment variable values 
    for(j = 0; j< numberOfExperiments; j++){
            if(busCapacityExpLength > 0 && (j+1)%capacityRot == 0 && j > 0){
                rotateIntArray(busCapacityExp, busCapacityExpLength);
                busCapacity = busCapacityExp[0];
            }
            if(boardingTimeExpLength > 0 && (j+1)%boardTimeRot == 0 && j > 0){
                rotateIntArray(boardingTimeExp, boardingTimeExpLength);
                boardingTime = boardingTimeExp[0];
                
            }
            if(requestRateExpLength > 0 && (j+1)%requestRot == 0 && j > 0){
                rotateFloatArray(requestRateExp, requestRateExpLength);
                requestRate = requestRateExp[0];
            }
            if(pickupIntervalExpLength > 0 && (j+1)%pickupRot == 0 && j > 0){
                rotateFloatArray(pickupIntervalExp, pickupIntervalExpLength);
                pickupInterval = pickupIntervalExp[0];
            }
            if(maxDelayExpLength > 0 && (j+1)%delayRot == 0 && j > 0){
                rotateIntArray(maxDelayExp, maxDelayExpLength);
                maxDelay = maxDelayExp[0];
            }
            if(noBusesExpLength > 0 && (j+1)%busesRot == 0 && j > 0){
                rotateIntArray(noBusesExp,noBusesExpLength);
                noBuses = noBusesExp[0];
            }
            if(stopTimeExpLength > 0 && (j+1)%stopTimeRot == 0 && j > 0){
                rotateFloatArray(stopTimeExp, stopTimeExpLength);
                stopTime = stopTimeExp[0];
            } 

        printf("Experiment #%d: ", j+1);
        for(i=0; i<7;i++){
            if(was_allocated[i] == 1){
                switch(i){
                    case 0 : printf("busCapacity %d ", busCapacity); break;
                    case 1 : printf("boardingTime %d ", boardingTime); break;
                    case 2 : printf("requestRate %f ", requestRate); break;
                    case 3 : printf("pickupInterval %f ", pickupInterval); break;
                    case 4 : printf("maxDelay %d ", maxDelay); break;
                    case 5 : printf("noBuses %d ", noBuses); break;
                    case 6 : printf("stopTime %f ", stopTime); break;
                }
            }
        }
        simulate(t);    
    } 
    // if there are no experiments then simply start one-off simulation
    if(numberOfExperiments == 0){
        simulate(t);
    }
    // free memory allocated variables
    //******************************************************************
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
    // ******************************************************************
}

void simulate(time_t tm){
// ---------------- start simulation -------------------- 

    FILE *pf = fopen("Output.txt", "w");
   
    // if it is an experiment, then disable event printing, otherwise print the events
    int printing_enabled = 0;
    if(numberOfExperiments == 0){
        printing_enabled = 1;
    }

    // initialize local variables used for simulation calculations:
    // number of completed trips
    int completed_trips = 0;
    // total duration of all completed trips
    int journey_duration = 0;
    // theoretical duration (without any delays) of completed trips
    int theoretical_duration = 0;
    // summation of all differences between shortest and actual trips 
    int deviationSum = 0;
    // number of total requests
    int numberOfRequests = 0;
    // number of missed requests
    int numberOfMissedRequests = 0;
    // declare and initialize current time
    int current_time = 0;
    // declare and initialize pickup time
    int pickup_time = 0;
    // declare and initialize request time
    int request_time = 0;
    // declare and initialize pickup rime interval
    int pickup_time_int = 0;
    
    // declare integers i and j that will be used for loops
    int i,j;
    // dinamically allocate memory for structure Bus - create an array of structures
    struct Bus *bus[noBuses];
    for(i=0; i<noBuses; i++){
        bus[i] = malloc(sizeof(struct Bus));
    }
    // initialize structure Bus data for every bus
    for (i = 0; i < noBuses; i++){
        bus[i] -> passengersOnBoard = 0;
        bus[i] -> routeLength = 0;
        bus[i] -> currentStop = 0;
        bus[i] -> lastScheduledStopIndex = 0;
        bus[i] -> arrived = 1;
        bus[i] -> routeDuration = 0;
        bus[i] -> passengersTransported = 0;
            for(j = 0; j < 6500; j++){
                bus[i] -> reservedSeats[j] = 0;
                bus[i] -> route[j] = -1;
                bus[i] -> departureTime[j] = 0;
                bus[i] -> arrivalTime[j] = 0;
                bus[i] -> tripDuration[j] = 0;
                bus[i] -> theoreticalDuration[j] = 0;
                bus[i] -> passengersToDisembark[j] = 0;
                bus[i] -> waiting[j] = 0;
                if(j == 0){
                    // all buses start from garage (stop #0)
                    bus[i] -> route[j] = 0; 
                }
            }    
    }
    // is_valid_subroute() checks if path is a subroute of  current bus route from last scheduled 
    // bus stop (it can be merged) and if it is it returns positive index of bus route where subpath begins
    // otherwise it returns -1; e.g. bus path [0,4,1,0,5] and path to merge is [1,0,5]; then it returns merge 
    // index 2
    int is_valid_subroute(struct Bus *bus[], int busNumber, int pickupTime, int pathToCheck[], int pathLength){
        struct Bus *b = bus[busNumber];
        // it can only merge from last scheduled stop - no changes are allowed before sheduled stop because all pickup times
        // are scheduled already
        int from = b -> lastScheduledStopIndex;
        int to = b -> routeLength+1;
        int t = 0;
        int k = from;
        // define and initialize if there was a match and intialize intersection index 
        int match = 0;
        int intersection_index = -1;
        while(1){
                begin:
                // if bus route or path reached its checking boundaries, then stop searching
                if( k == to||t == pathLength){
                    break;
                }
                if(b -> route[k] == pathToCheck[t]){
                    // if bus route element matches path element and there was a match before, 
                    // check the rest of elements
                    if(match == 1){
                        t++;
                        k++;
                        goto begin;
                    }
                    // if bus route element matches path element but there was no match before, 
                    // do the following:
                    if(match != 1){
                        // update that there is match
                        match = 1;
                        // update intersection index to current route element that is being checked
                        intersection_index = k;
                        // check the rest of path and route elemnets if they also match
                        t++;
                        k++;
                        goto begin;
                    }
                }
                if(b -> route[k] != pathToCheck[t]){
                    // if route and path elements do not match but there was a match before:
                    if(match == 1){
                        // reset intersection index to -1
                        intersection_index = -1;
                        // update that there is no match
                        match = 0;
                        // check next route element
                        k++; 
                        // scan path from the beggining
                        t = 0;
                        goto begin;
                    }
                    // if route and path elements do not match and was no match before:
                    if(match != 1){
                        // check next bus route elemnt
                        k++;
                        goto begin;
                    } 
                }    
        }
        if(intersection_index != -1){
            // if merge is possible but it takes customer to wait longer than maxDelay or it arrives quicker than desired pickup time, then 
            // merge is not possible
            if(b -> arrivalTime[intersection_index] - pickupTime > (int) maxDelay*60 || b -> arrivalTime[intersection_index] < pickupTime){
                intersection_index = -1;
            }
        }
        return intersection_index;
    }     

    // function add route adds path to the current bus route; If path is subarray of bus route, then only variables of how 
    // many passengers needs to be picked up and disembarked are increased, otherwise route is extended
    void add_route(struct Bus *bus[], int busNumber, int mergeIndex, int lastPickupTime, int pathToAdd[], int pathLength){
        struct Bus *b = bus[busNumber];
        int k = 0;
        int d, limit;
        // limit is the index up to which bus route needs to be changed
        limit = mergeIndex + pathLength;
        // if the limit is greater than current bus route length, route needs to be extended 
        if(limit > (b-> routeLength + 1)){
            for(d = mergeIndex; d < limit; d++){
                // part of path that extends route is copied to a bus route
                b -> route[d] = pathToAdd[k];
                k++;
            }
        }
        // route length is changed
        b -> routeLength = limit-1; 
        // number of passengers to be picked up from desired bus stop is increased
        b -> reservedSeats[mergeIndex]++;
        // number of passengers to be disembarked at the destination stop is increased
        b -> passengersToDisembark[mergeIndex+pathLength-1]++;
        // check how many passengers's trip duration will be affected by pickingup a passenger on the way and update 
        // their actual trip duration
        for(i = mergeIndex+1; i < mergeIndex+pathLength-1; i++){
            if(b -> passengersToDisembark[i] > 0){
                b -> tripDuration[i] += b -> passengersToDisembark[i]*boardingTime;
            }
        }
        // check how many passengers's trip duration will be affected by pickingup and disembarking a passenger on the way
        // and update their actual trip duration
        for(i = mergeIndex+pathLength-1; i < b -> routeLength; i++){
            if(b -> passengersToDisembark[i] > 0){
                b -> tripDuration[i] += b -> passengersToDisembark[i]*2*boardingTime;
            }
        }

    }
    // refresh_bus_timing() recalculates arrival and departure timing for each stop in the bus route
    // also it updates last scheduled bus stop
    void refresh_bus_timing(struct Bus *bus[], int busNumber){
        int c;
        struct Bus *b = bus[busNumber];
        // it checks only from last puckup stop, because before that stop no changes can be made
        for(c = b->lastScheduledStopIndex; c <= b->routeLength; c++){
            if (c == 0 && b-> arrivalTime[0] > 0){
                // update only departure time
                b -> departureTime[c] = b->arrivalTime[c]+ (b->reservedSeats[c] + b->passengersToDisembark[c] + b->waiting[c])*boardingTime;
            // else if not checking first element of the route 
            } else if (c > 0 ){ 
                // update arrival time by checking when bus leaves previous bus stop and add time to get to the bus stop                                                    
                b -> arrivalTime[c] = b -> departureTime[c-1] + dijkstra(map, b->route[c-1], b->route[c], noStops) * 60;
                // update departure time by checking when bus arrives at the bus stop and add boarding time for every person that
                // disembarks and boards at this bus stop
                b -> departureTime[c] = b->arrivalTime[c] + (b->reservedSeats[c] + b->passengersToDisembark[c] + b->waiting[c])*boardingTime;
                // if bus is scheduled to pick some customer at this bus stop, then its index in the bus route 
                // becomes index of last scheduled bus stop and no changes can be made before this index
                if(b->reservedSeats[c] > 0){
                    b -> lastScheduledStopIndex = c;
                }
            } else {
                // otherwise if there was waiting, already scheduled arrival and departure time do not change 
                // only last scheduled stop index is updated
                if(b->reservedSeats[c] > 0){
                    b -> lastScheduledStopIndex = c;
                }
            }
        }
    }
    // scan_capacity() checks and returns how many passengers will be on board at the end of sheduled route
    int scan_capacity(struct Bus *bus[], int busNumber){
        struct Bus *b = bus[busNumber];
        int capacity = 0;
        for(i = 0; i<= b->routeLength; i++){
            capacity += b -> reservedSeats[i] + b -> waiting[i] - b -> passengersToDisembark[i];
        }
        return capacity;
    }
    // function accommoadate_passenger() returns 1 if passenger will be picked up by some bus, otherwise 0 is returned 
    int accommodate_passenger(struct Bus *bus[], int busNumber, int pickupStop, int  disembarkStop, int currentTime, int pickupTime){
        struct Bus *b = bus[busNumber];
        int d, distance, sourceStopIndex, targetStopIndex;
        int l = b -> routeLength;
        // for every bus that has no departure time at the beggining, assign cuurrent time as departure time from garage
        if(b->departureTime[l] == 0){
            b -> departureTime[0] = currentTime;
        }
        // if bus route already reaches the limit, the bus can't accommodate passenger    
        if(l >= 6500){
            return 0;
        }
        // define and initialize path[] and its length
        int path[noStops], path_length;
        // retrieve path and ist length from pickup stop to passengers destination bus stop
        path_length = get_shortest_path(path, map, pickupStop, disembarkStop, noStops);
        if(path_length < 0){
            return 0;
        }
        // check if possible to merge path and current bus route
        int mergeIndx = is_valid_subroute(bus, busNumber,pickupTime, path, path_length);
        if(mergeIndx != -1){
             // if merege is possible, merge route and the path
            add_route(bus, busNumber, mergeIndx, pickupTime, path, path_length);
            // recalculate bus timing
            refresh_bus_timing(bus, busNumber);
            // pickup bus top will have sourceStopIndex in the bus route which is equal to merge index
            sourceStopIndex = mergeIndx;
            // destination bus stop will have targetStopIndex in the bus route which is equal to the index at which merging ends
            targetStopIndex = mergeIndx+path_length-1;
            // calculate and update actual trip duration
            b -> tripDuration[targetStopIndex] += b->arrivalTime[targetStopIndex] - b->departureTime[sourceStopIndex];
            // update shortest trip duration at the index of target bus stop in the route by calculating shortest distance from pickup 
            // bus top to the destination bus stop
            b -> theoreticalDuration[targetStopIndex] += dijkstra(map, pickupStop, disembarkStop, noStops)*60; 
            return 1;
        } 
        // if can't merge path and the current bus route then try to append path 
        // search for a path from last stop in the route to the pickup stop
        distance = dijkstra(map, b->route[l], pickupStop, noStops);
        if(distance >= infinity){
            // if no path was found then bus can't pickup passenger
            return 0;
        } 
        // if path was found and time gap from last bus stop in the route to pickup stop
        // is less then given maximum delay and bus capacity won't be exceeded,
        // then bus accommodates passenger
        if(((b -> departureTime[l] + distance * 60) - pickupTime) <= (int) (maxDelay * 60) && 
            scan_capacity(bus, busNumber) < (int) busCapacity &&
            ((b->departureTime[l]) + distance * 60) >= pickupTime){
            for(d = 0; d < path_length; d++){
                l++;
                // when adding first stop to a route, a seat is reserved as passenger will be picked up at that stop
                if(d == 0){  
                    sourceStopIndex = l; 
                    b -> reservedSeats[l]++;
                } 
                // when adding last stop from the path, number of passengers disembarking at that stop is increased   
                if(d == path_length-1){
                    targetStopIndex = l;
                    b -> passengersToDisembark[l]++;
                }
                // path is copied to a bus route
                b -> route[l] = path[d];
                // route length is increased
                b -> routeLength++;
                
            }
            // recalculate arrival and departure times for each bus stop in bus route
            refresh_bus_timing(bus, busNumber);
            // actual trip duration to destination bus stop is calculated and updated(time for boarding and disembarking is excluded)
            b -> tripDuration[targetStopIndex] += (b->arrivalTime[targetStopIndex] - b-> departureTime[sourceStopIndex]);
            // shortest trip duration to destination bus stop is calculated and updated
            b -> theoreticalDuration[targetStopIndex] += dijkstra(map, pickupStop, disembarkStop, noStops)*60; 
            return 1;
        } 
        // if bus is standing at the bus stop or bus needs to wait at the bus time in order to pickup a passenger
        if(scan_capacity(bus, busNumber) < (int) busCapacity && b -> departureTime[l] +  distance * 60 < pickupTime){
            // calculates at what time bus needs to leave so that it wouldn't arrive too early
            b -> departureTime[l] = pickupTime - distance * 60;
            for(d = 0; d < path_length; d++){
                l++;
                // when adding first stop to a route, a seat is reserved as passenger will be picked at that stop
                if(d == 0){
                    sourceStopIndex = l;
                    b -> lastScheduledStopIndex = l;
                    b -> waiting[l]++;
                }
                // when adding last stop from the path, number of passengers disembarking at that stop is increased    
                if(d == path_length-1){
                    targetStopIndex = l;
                    b -> passengersToDisembark[l]++;
                }
                // path is copied to a bus route
                b -> route[l] = path[d];
                // route length is increased
                b -> routeLength++;
            }
            // recalculate arrival and departure times for each bus stop in bus route
            refresh_bus_timing(bus, busNumber);
            // actual trip duration to destination bus stop is calculated and updated(time for boarding and disembarking is excluded)
            b -> tripDuration[targetStopIndex] += (b->arrivalTime[targetStopIndex] - b-> departureTime[sourceStopIndex]);
            // shortest trip duration to destination bus stop is calculated and updated
            b -> theoreticalDuration[targetStopIndex] += dijkstra(map, pickupStop, disembarkStop, noStops)*60; 
            return 1; 
        } 
        // if none of conditions were not satisfied, then it is not possible to pickup passenger thus 0 is returned
        return 0;
    }
    // function disembark() updates data of the bus when disembarking the passenger
    void disembark(struct Bus *bus[], int busNumber){
        struct Bus *b = bus[busNumber];
        // if there are passengers to disembark, disembark message is yield
        if(b->passengersToDisembark[0] > 0){
            if(printing_enabled){
                disembark_message(buffer, b->arrivalTime[0], busNumber+1, b->route[0]);
                printf("%s", buffer);     
            }
            // number of completed trips is increased
            completed_trips++;
            // number of passengers on board is reduced
            b -> passengersOnBoard--;  
            if(printing_enabled){
                // occupancy change message is yield
                occupancy_message(buffer, b->arrivalTime[0], busNumber+1, b->passengersOnBoard);
                printf("%s", buffer);    
            }
            // number of passengers that left to disembark is decreased
            b -> passengersToDisembark[0]--;
        }   
    }
    // function board() updates data of the bus when boarding the passenger
    void board(struct Bus *bus[], int busNumber){
        struct Bus *b = bus[busNumber];
        // if there are passenger that needs to boarded, boarding message is yield
        if(b -> reservedSeats[0] > 0){
            if(printing_enabled){
                boarding_message(buffer, b -> arrivalTime[0], busNumber+1, b -> route[0]);
                printf("%s", buffer);                
            }
            // number of passengers on board is increased
            b -> passengersOnBoard++;
            if(printing_enabled){
                // occupancy change message is yield
                occupancy_message(buffer, b -> arrivalTime[0], busNumber+1, b->passengersOnBoard);
                printf("%s", buffer);
            }
            // number of passengers that left to board is decreased
            b -> reservedSeats[0]--;
        } 
    }
    // fnction shift_route() shifts all bus data arrays to the left after passing by or leaving bus stop
    void shift_route(struct Bus *bus[], int busNumber){
        struct Bus *b = bus[busNumber];
        int g;
        int limit = b->routeLength;
        // total journey duration is increased by the actual duration
        journey_duration += b->tripDuration[0];
        theoretical_duration += b->theoreticalDuration[0];
        deviationSum += b->tripDuration[0] - b->theoreticalDuration[0];
        
        // if bus route is greater than 0
        if(b -> routeLength > 0){
            b->passengersTransported += b->passengersOnBoard;
            b->routeDuration += b->arrivalTime[1] - b->departureTime[0]; 
            // push all values of arrays to the left (overwrite with values on the right)
            for(g = 1; g <= b -> routeLength; g++){
                b -> route[g-1] = b -> route[g];
                b -> arrivalTime[g-1] = b -> arrivalTime[g];
                b -> departureTime[g-1] = b -> departureTime[g];
                b -> reservedSeats[g-1] = b -> reservedSeats[g];
                b -> passengersToDisembark[g-1] = b -> passengersToDisembark[g];
                b -> tripDuration[g-1] = b -> tripDuration[g];
                b -> theoreticalDuration[g-1] = b -> theoreticalDuration[g];
                b -> waiting[g-1] = b -> waiting[g];
            } 
            // update all values that are the last in the arrays to the initial values
            b -> reservedSeats[limit] = 0;
            b -> passengersToDisembark[limit] = 0;
            b -> arrivalTime[limit] = 0;
            b -> departureTime[limit] = 0;
            b -> tripDuration[limit] = 0;
            b -> theoreticalDuration[limit] = 0;
            b -> currentStop = b -> route[0];
            b -> waiting[limit] = 0;
            if(b -> lastScheduledStopIndex > 0){
                b -> lastScheduledStopIndex--;   
            }
            // reduce bus route length
            b -> routeLength--;
        }
    }
    // function find_next_event() checks through all buses if there exists a bus with an event time less than current time
    // if such bus is found, then that bus number is returned, otherwise -1 is returned
    int find_next_event(struct Bus *bus[], int currentTime){
        // set local minimum to infinity
        int local_minimum = infinity; 
        int bus_number = -1;
        for(i = 0; i < noBuses; i++){
            // check if there is any bus that departs from the bus stop
            if( bus[i] -> routeLength > 0 &&
                bus[i] -> departureTime[0] <= currentTime && 
                bus[i] -> departureTime[0] < local_minimum){
                // set local minimum to the departure time
                local_minimum = bus[i] -> departureTime[0];
                // update bus number that will be returned to the currently checked bus number
                bus_number = i;
            }
            // check if there is any bus that arrives at the bus stop
            if( bus[i] -> routeLength > 0 &&
                bus[i] -> arrivalTime[0] > 0 &&
                bus[i] -> arrivalTime[0] <= currentTime && 
                bus[i] -> arrivalTime[0] <= bus[i] -> departureTime[0] &&
                bus[i] -> arrivalTime[0] <= local_minimum){
                // set local minimum to the departure time
                local_minimum = bus[i] -> arrivalTime[0];
                // update bus number that will be returned to the currently checked bus number
                bus_number = i;
            }
        }
        return bus_number;
    }

    // find_optimal_bus() tries to find the best bus to pickup a passenger, if no bus is found decline message is triggered
    void find_optimal_bus(int numOfBuses[], int len, int pickupStop, int disembarkStop, int currentTime , int pickupTime){
        int optimal_bus, m;
        // set local distance mninimum to infinity
        int local_minimum = infinity;
        // define shortest distance
        int shortest_distannce;
        // new array that will have numbers of busses except already checked ones
        int newNumOfBuses[len];
        // index at which bus number needs to be removed from bus number array
        int index;
        int found = 0;

        while(1){
            for(m=0; m<len; m++){
                // retrieve current stop for a bus 
                int current_stop = bus[numOfBuses[m]]->currentStop;
                // calculate shortest distance from bus current stop to pickup stop
                shortest_distannce = dijkstra(map, current_stop, pickupStop, noStops);
                // if the distance is less then local minimum
                if(shortest_distannce < local_minimum){
                    // set local minimum to the value of shortest distance
                    local_minimum = shortest_distannce;
                    // set optimal bus number to currently checked bus number
                    optimal_bus = numOfBuses[m];  
                }
            }
            // if no closest bus was ever found
            if(local_minimum >= infinity){
                // decline message is triggered
                if(printing_enabled){
                    decline_message(buffer, currentTime, pickupStop, disembarkStop, pickupTime);
                    printf("%s",buffer);
                }
                // number of missed request is increased
                numberOfMissedRequests = numberOfMissedRequests + 1;
                break;     
            }
            // if bus is capabale to accomodate passenger then message of picking up costomer is triggered
            if(accommodate_passenger(bus, optimal_bus, pickupStop, disembarkStop, currentTime, pickupTime) == 1){
                // get the index of previously scheduled bus stop
                int index = bus[optimal_bus] -> lastScheduledStopIndex;
                if(printing_enabled){
                    // if bus will be waiting at the bus stop
                    if(bus[optimal_bus]->waiting[index]>0){
                        // then pickup time is calculated
                        int temp_time = bus[optimal_bus]->departureTime[index] - bus[optimal_bus] -> waiting[index] * boardingTime;
                        // request message is yield
                        request_message(buffer, currentTime, pickupStop, disembarkStop, pickupTime, temp_time);
                        printf("%s", buffer);
                    } else {
                        // otherwise pickup time is set to the arrival time at the pickup stop 
                        request_message(buffer, currentTime, pickupStop, disembarkStop, pickupTime, bus[optimal_bus] -> arrivalTime[index]);
                        printf("%s", buffer);
                    }
                }
                break;
            // if bus cannot pickup customer, its number is removed from buses list and another bus is being searched for pickingup a passenger    
            } else {
                // if length of the list is 1 and no optimal bus previosuly was found, then there are no buses
                // that could pick a passenger and decline message is triggered
                if(len == 1){
                    if(printing_enabled){
                        decline_message(buffer, currentTime, pickupStop, disembarkStop, pickupTime);
                        printf("%s",buffer);
                    }
                    // number of missed requests is increased
                    numberOfMissedRequests = numberOfMissedRequests + 1;
                    break; 
                }
                // otherwise remove already searched bus from the list of busses
                for(m = 0; m<len; m++){
                    if(numOfBuses[m] == optimal_bus){
                        found = 1;
                        index = m;
                        break;
                    }
                    newNumOfBuses[m] = numOfBuses[m];
                }
                if(found){
                    for(m = index; m<len-1;m++){
                        newNumOfBuses[m] = numOfBuses[m+1];
                    }
                }
                // make a recursive call with a smaller list of buses number  
                find_optimal_bus(newNumOfBuses, len-1, pickupStop, disembarkStop, currentTime, pickupTime);
            }
            break;
        }
    }

    // Calculate request mean (in seconds)
    float request_mean = requestRate/(60*60);

    // generate an array that holds bus numbers
    int numOfBuses[noBuses];
    for(i = 0; i<noBuses;i++){
        numOfBuses[i] = i;
    }
    // seed the random number 
    srand((unsigned) tm);
    // ---- start simulation control:
    while(1) {
        // Get random request time (in seconds)
        request_time = (int) (-log((float)rand()/(float)RAND_MAX)/request_mean);        
        // Get random desired pick-up time interval      
        pickup_time_int = (int) (-log((float)rand()/(float)RAND_MAX)*pickupInterval)*60;
        pickup_time = (int) request_time + pickup_time_int;
        // Get random bus stop number at which request is placed
        int pickup_bus_stop;
        while(1) {
            pickup_bus_stop = (int) (rand() % noStops-1)+1;
            if (pickup_bus_stop != 0)
                break;
        }      
        // Get random bus stop number to which passenger desires to travel
        int destination_bus_stop;
        while(1) {
            destination_bus_stop = (int) (rand() % noStops-1)+1;
            if (destination_bus_stop != pickup_bus_stop && destination_bus_stop != 0){
                break;
            }
        }
        // update request time
        request_time += current_time;
        // update current time
        current_time = request_time;  
        // update pickup time
        pickup_time += request_time;

        // if request or pickup time exceeds given simulation duration time, stop the simulation
        if ( (current_time > stopTime*60*60)){
            break;
        }
        // increase number of requests
        numberOfRequests++;

        // MAIN EVENT CONTROLLER:
        // ***********************************************************************************************
        // check if exists bus with an event that should happen before request is placed
        int bus_num;
        search_again:
        bus_num = find_next_event(bus, current_time);
        if(bus_num != -1){
            struct Bus *b = bus[bus_num];
            // check if it is arrival event
            if(b->arrivalTime[0] <= b->departureTime[0] &&
                b->arrivalTime[0] <= current_time){
                // if bus has to disembark or board passengers at the bus stop an it hasn't arrived there yet, then 
                // yield arrival event
                if((b -> reservedSeats[0] > 0 || b -> passengersToDisembark[0] > 0 || b -> waiting[0]) && b -> arrived == 0){
                    if(printing_enabled){
                        arrival_message(buffer, b->arrivalTime[0], bus_num+1, b->route[0]);
                        printf("%s", buffer);
                    }
                    // update that bus has arrived
                    b -> arrived = 1;
                    // update crrent stop 
                    b -> currentStop = b->route[0]; 
                    // increase arrival time which will be used for disembarking/boarding passengers
                    b -> arrivalTime[0] += boardingTime;
                    // search if there are more events that need to be triggered
                    goto search_again;
                } else
                // if there is a passenger to disembark and bus already arrived to the stop, yield disembark message
                if(b -> passengersToDisembark[0] > 0 && b -> arrived == 1){
                    disembark(bus, bus_num);
                    if(b -> passengersToDisembark[0] > 0 || b-> reservedSeats[0]> 0){
                        b -> arrivalTime[0] += boardingTime;
                    }
                    // search if there are more events that needs to be triggered 
                    goto search_again;
                } else 
                // if there is a passenger to bard on and the bus had already arrived, trigger boarding message
                if(b -> reservedSeats[0] > 0 && b -> arrived == 1){
                    board(bus, bus_num);
                    if(b -> reservedSeats[0] > 0){
                        b -> arrivalTime[0] += boardingTime;
                    }
                    // search if there are more events that needs to be triggered
                    goto search_again;
                } else 
                // if there are no more passengers to board/disembark but bus hasn't left the stop yet and it is waiting for a passenger
                if(b-> passengersToDisembark[0] == 0 && b->reservedSeats[0] == 0 && b -> waiting[0] > 0 && b -> arrived == 1){
                    // then calculate and update at what tim eit will be boarded
                    if(b -> arrivalTime[0] < b -> departureTime[0] - b -> waiting[0] * boardingTime){
                        b -> arrivalTime[0] = b -> departureTime[0] - b -> waiting[0] * boardingTime;
                    }
                    // update reserved seat number to the number of passengers that bus is waiting for
                    b -> reservedSeats[0] = b -> waiting[0];
                    // reset number of passengers taht bus is waiting for
                    b -> waiting[0] = 0;
                    goto search_again;
                } else 
                // if bus is only passing by the bus stop
                if( b-> arrivalTime[0] == b-> departureTime[0] && b-> passengersToDisembark[0] == 0 && b->reservedSeats[0] == 0 && b -> waiting[0] == 0 &&
                    b-> arrived == 0){
                    // then shift the whole route
                    shift_route(bus, bus_num);
                    goto search_again;
                } else 
                // if bus is waiting at the stop
                if(b-> arrivalTime[0] < b-> departureTime[0] && b-> passengersToDisembark[0] == 0 && b->reservedSeats[0] == 0 && b -> waiting[0] == 0 &&
                    b-> arrived == 1) {
                    // recalculate arrival time so that function find_next_event() could find other bus events
                    b -> arrivalTime[0] = b -> departureTime[0];
                    goto search_again;
                }
                
            }
            // check if it is departure event and there aro no more passengers to board or disembark
            // and bus has already arrived to the bus stop
            if(b->departureTime[0] >= b->arrivalTime[0] && b->departureTime[0] <= current_time &&
                b -> reservedSeats[0] == 0 && b-> passengersToDisembark[0] == 0 &&
                b->arrived == 1){
                // initiate for the next event that bus has not arrived 
                b -> arrived = 0;
                // print departure message
                if(printing_enabled){
                    departure_message(buffer, b->departureTime[0], bus_num+1, b->route[0]);
                    printf("%s", buffer);
                }
                // shift the route
                shift_route(bus, bus_num);
                // search if there are more events that needs to be triggered
                goto search_again;
            }
        } 

        find_optimal_bus(numOfBuses, noBuses, pickup_bus_stop, destination_bus_stop, current_time, pickup_time);   
        // ******************************************************************************************************
    }

    fclose(pf);
    float avgEfficiency = 0;
    int numOfBusesThatMoved = 0;
    FILE *eff = fopen("eff", "a+");
    FILE *missedReq = fopen("missedReq", "a+");
    FILE *dur = fopen("dur", "a+");
    FILE *dev = fopen("dev", "a+");
    // calculate average trip efficiency and check how many buses moved during the simulation
    for(i = 0; i < noBuses; i++){
        if(bus[i] -> arrivalTime[0] > 0){
            avgEfficiency +=  (bus[i] -> passengersTransported / (bus[i]->routeDuration / 60.0));
            numOfBusesThatMoved++;
        }
    }

    if(numOfBusesThatMoved > 0){
        avgEfficiency = avgEfficiency / numOfBusesThatMoved*1.0;
    } else {
        avgEfficiency = 0;
    }

    printf("\n---\n");
    if(completed_trips>0){
        printf("average trip duration %02d:%02d\n", (journey_duration/completed_trips)/60,  (journey_duration/completed_trips)%60);
    } else {
        printf("average trip duration 00:00\n");
    }
    printf("trip efficiency %5f\n", avgEfficiency);
    printf("percentage of missed requests %.2f\n", (numberOfMissedRequests*100/numberOfRequests*1.0));
    printf("average passenger waiting time %.0f seconds\n", 0.0);
    if(completed_trips>0){
        printf("average trip deviation %5f\n", (deviationSum/completed_trips*1.0)/60.0);
    } else {
        printf("average trip deviation 0.00\n");
    }
    printf("---\n");
    fprintf(eff, "%d %5f", busCapacity, avgEfficiency);
    fprintf(eff, "\n");
    fprintf(missedReq, "%d %5f", busCapacity, (numberOfMissedRequests*100/numberOfRequests*1.0));
    fprintf(missedReq, "\n");
    fprintf(dev, "%d %5f", busCapacity, (deviationSum/completed_trips*1.0)/60.0);
    fprintf(dev, "\n");
    fprintf(dur, "%d %d", busCapacity, journey_duration/completed_trips);
    fprintf(dur, "\n");
    for(i=0; i<noBuses; i++){
        free(bus[i]);
    }
    fclose(eff);
    fclose(missedReq);
    fclose(dev);
    fclose(dur);
}