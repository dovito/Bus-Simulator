#ifndef OUTPUT
#define OUTPUT

void request_message(char buff[], int req_time, int bus_stop, int dest_bus_stop, int pick_time, int accomm_time);
void decline_message(char buff[], int req_time, int bus_stop, int dest_bus_stop, int pick_time);
void departure_message(char buff[], int event_time, int bus, int stop);
void arrival_message(char buff[], int event_time, int bus, int stop);
void boarding_message(char buff[], int event_time, int bus, int stop);
void disembark_message(char buff[], int event_time, int bus, int stop);
void occupancy_message(char buff[], int event_time, int bus, int stop);
void convert_time(char str[], int time_in_seconds);
	
#endif