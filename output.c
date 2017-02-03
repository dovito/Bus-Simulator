#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "output.h"


int to_seconds(int tm){
    return tm % 60;
}

int to_minutes (int tm) {
    return (tm / 60) % 60; 
}

int to_hours (int tm) {
    return (tm / 3600) % 24; 
}
        
int to_days (int tm) {
    return tm / 86400;
}

void request_message(char buff[], int req_time, int bus_stop, int dest_bus_stop, int pick_time, int accomm_time){
	char temp1[15];
	char temp2[15];
	char temp3[15];
	convert_time(temp1, req_time);
	convert_time(temp2, pick_time);
	convert_time(temp3, accomm_time);
	snprintf(buff, 200, "%s -> new request placed from stop %d to stop %d for departure at %s scheduled for %s\n", 
				temp1, bus_stop, dest_bus_stop, temp2, temp3);
}	

void decline_message(char buff[], int req_time, int bus_stop, int dest_bus_stop, int pick_time){
	char temp1[15];
	char temp2[15];
	convert_time(temp1, req_time);
	convert_time(temp2, pick_time);
	snprintf(buff, 200, "%s -> new request placed at stop %d to stop %d for departure at %s cannot be accommodated\n",
				temp1, bus_stop, dest_bus_stop, temp2);
}

void departure_message(char buff[], int event_time, int bus, int stop){
	char temp[15];
	convert_time(temp, event_time);
	snprintf(buff, 200, "%s -> minibus %d left stop %d\n",temp, bus, stop);
}

void arrival_message(char buff[], int event_time, int bus, int stop){
	char temp[15];
	convert_time(temp, event_time);
	snprintf(buff, 200, "%s -> minibus %d arrived at stop %d\n",temp, bus, stop);
}

void boarding_message(char buff[], int event_time, int bus, int stop){
	char temp[15];
	convert_time(temp, event_time);
	snprintf(buff, 200, "%s -> minibus %d boarded passenger at stop %d\n",temp, bus, stop);
}

void disembark_message(char buff[], int event_time, int bus, int stop){
	char temp[15];
	convert_time(temp, event_time);
	snprintf(buff, 200, "%s -> minibus %d disembarked passenger at stop %d\n",temp, bus, stop);
}

void occupancy_message(char buff[], int event_time, int bus, int occupancy){
	char temp[15];
	convert_time(temp, event_time);
	snprintf(buff, 200, "%s -> minibus %d occupancy became %d\n",temp, bus, occupancy);
}

void convert_time(char str[], int time_in_seconds){
	snprintf(str, 15, "%02d:%02d:%02d:%02d", to_days(time_in_seconds), to_hours(time_in_seconds),
				to_minutes(time_in_seconds),to_seconds(time_in_seconds));
}
