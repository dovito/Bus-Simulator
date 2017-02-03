#ifndef PATH
#define PATH

void reverse(int* path, int length);

int dijkstra(int** G,int source, int target, int noVertices);

int get_shortest_path(int* pathToReturn, int** cost, int source, int target, int no_stops);

#endif