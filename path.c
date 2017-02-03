#include <stdio.h>
#include <stdlib.h>
#include "path.h"
#define INFINITY 999999

int enable_printing = 0;

// reverse path backwards without using extra memory
void reverse(int path[], int length){
    int j = length - 1;
    int i = 0;
    // temporary variable that will keep value that needs to be swapped
    int temp;
    while (i < j) {
      temp = path[i];
      path[i] = path[j];
      path[j] = temp;
      i++; 
      j--;         
   }
}

// function that finds shortest path from source stop to target bus stop and returns the length of that path
int dijkstra(int** G,int source, int target, int noVertices){
//printf("Searching for a path from %d to %d\n",source, target );
//count gives the number of nodes seen so far
int count,mindistance,nextnode,i,j;   
// visited[] tracks which nodes were already visited
int *visited = (int *) malloc(sizeof(int)*noVertices);
// distance[]
int *distance = (int *) malloc(sizeof(int)*noVertices);
//pred[] stores the predecessor of each node
int *pred = (int *) malloc(sizeof(int)*noVertices);
// cost[i][j] is a cost of going from vertex i to j.
// If there is no edge between vertices i and j then cost[i][j] is infinity.
int (*cost)[noVertices];
cost = (int (*)[noVertices]) malloc(sizeof(int*)*noVertices*noVertices);

//create the cost matrix
    for(i=0;i<noVertices;i++){
       
        for(j=0; j < noVertices; j++){
           
            if(G[i][j] <= 0){
                cost[i][j] = INFINITY;
           
            } else{
                cost[i][j] = G[i][j];
            }
        }
    }            
   
//initialize pred[],distance[] and visited[]
    for(i=0; i < noVertices; i++){
           
        distance[i] = cost[source][i];
               
        pred[i] = source;
               
        visited[i] = 0;
    }
   
    distance[source] = 0;
       
    visited[source] = 1;
       
    count = 1;
   
    while(count < noVertices - 1){

        mindistance = INFINITY;
       
//nextnode gives the node at minimum distance
       
        for(i = 0; i < noVertices; i++)
                   
        if(distance[i] < mindistance && !visited[i]){
            mindistance = distance[i];
            nextnode = i;
        }
       
        //check if a better path exists through nextnode
        visited[nextnode] = 1;
           
        for(i=0; i < noVertices; i++){
                   
            if(!visited[i])
                           
            if(mindistance + cost[nextnode][i] < distance[i]){
                distance[i] = mindistance + cost[nextnode][i];
                pred[i] = nextnode;
            }
        }       
        count++;
    }
   
              
    int dist = distance[target];    
    free(visited);
    free(distance);
    free(pred);
    free(cost);
    return dist;
}

int get_shortest_path(int pathToReturn[], int** G,int source, int target, int noVertices){
    
int localPath[noVertices];
int len;
//printf("Searching for a path from %d to %d\n",source, target );
//count gives the number of nodes seen so far
int count,mindistance,nextnode,i,j;   
// visited[] tracks which nodes were already visited
int *visited = (int *) malloc(sizeof(int)*noVertices);
// distance[]
int *distance = (int *) malloc(sizeof(int)*noVertices);
//pred[] stores the predecessor of each node
int *pred = (int *) malloc(sizeof(int)*noVertices);
// cost[i][j] is a cost of going from vertex i to j.
// If there is no edge between vertices i and j then cost[i][j] is infinity.
int **cost;
cost = (int **) malloc(sizeof(int*)*noVertices);
    for(i=0; i < noVertices; i++){
        cost[i] = (int *) malloc(sizeof(int*)*noVertices);
    }

//create the cost matrix
    for(i=0;i<noVertices;i++){
       
        for(j=0; j < noVertices; j++){
           
            if(G[i][j] <= 0){
                cost[i][j] = INFINITY;
           
            } else{
                cost[i][j] = G[i][j];
            }
        }
    }            
   
//initialize pred[],distance[] and visited[]
    for(i=0; i < noVertices; i++){
           
        distance[i] = cost[source][i];
               
        pred[i] = source;
               
        visited[i] = 0;
    }
   
    distance[source] = 0;
       
    visited[source] = 1;
       
    count = 1;
   
    while(count < noVertices - 1){

        mindistance = INFINITY;
       
//nextnode gives the node at minimum distance
       
        for(i = 0; i < noVertices; i++)
                   
        if(distance[i] < mindistance && !visited[i]){
            mindistance = distance[i];
            nextnode = i;
        }
       
//check if a better path exists through nextnode
        visited[nextnode] = 1;
           
        for(i=0; i < noVertices; i++){
                   
            if(!visited[i])
                           
            if(mindistance + cost[nextnode][i] < distance[i]){
                distance[i] = mindistance + cost[nextnode][i];
                pred[i] = nextnode;
            }
        }       
        count++;
        
    }
    //printf("Distance from %d to the target %d is: %d\n",source, target, distance[target]);
    if(distance[target] >= INFINITY){
        free(visited);
        free(distance);
        free(pred);
        for(i=0; i < noVertices; i++){
            free(cost[i]);
        }
        free(cost);
        return -1;
    }

    int current_node, node;
    int k = 1;
    node = target;
    localPath[0] = node;
    while(node != source){
        //printf("current node: %d\n", current_node);
        current_node = pred[node];
        localPath[k] = current_node;
        node = current_node;
        k++;
    }
    len = k;
    reverse(localPath, len);
    for(i = 0; i<len;i++){
        pathToReturn[i] = localPath[i];
    }
    free(visited);
    free(distance);
    free(pred);
    for(i=0; i < noVertices; i++){
        free(cost[i]);
    }
    free(cost);
    return len;
}
