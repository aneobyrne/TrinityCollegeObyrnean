#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "t3.h"

Graph* g = NULL;

Graph* create_graph(int num_stops){ //creates a graph with num_nodes nodes, assuming nodes are stored in alphabetical order (A, B, C..)
    g = (Graph*)malloc(sizeof(Graph)); //allocate memory to graph
    if (!g) {
        printf("Memory allocation failed for graph.\n");
        return NULL;
    }
    g->num_stops = num_stops; //set amount of nodes in the graph
    g->vertices = malloc(num_stops * sizeof(Vertex));
    if (!g->vertices) {
        printf("Memory allocation failed for vertices.\n");
        free(g);
        return NULL;
    }
    for (int i = 0; i < num_stops; i++) { //initialise matrix 10x10
        for (int j = 0; j < num_stops; j++) {
          if (i == j) //if its the same node 
             g->adj_matrix[i][j] = 0; //distance to itself is 0
          else
             g->adj_matrix[i][j] = 100000; //not connected initially (give it a large number)
        }
    }
    return g;
}

void add_edge(Graph *g, int from, int to, int weight){ //adds a undirected edge
    if (!g || !g->adj_matrix) {
        printf("Graph is not initialized.\n");
        return;
    }
    int from_index = -1;
    int to_index = -1; 
    for (int i = 0; i < g->num_stops; i++) {
      if(g->vertices[i].stop_id == from)
        from_index = i;
      if(g->vertices[i].stop_id == to)
        to_index = i;
    }
    
    if(from_index !=-1 && to_index != -1){
    g->adj_matrix[from_index][to_index] = weight;
    g->adj_matrix[to_index][from_index] = weight; //set both ways since there's no specified direction
    g->num_edges++;
    }
}

int load_edges ( char *fname ){ //loads the edges from the CSV file of name fname
   FILE* f;
   char buf[1000];
   int cbuf = 0;
   g->num_edges = 0; //initialize edge count
   f = fopen(fname, "r"); //read specified file

   if (!f) { //check if file exists, if not output message of error
     printf("Unable to open %s\n", fname);
     return 0; 
   }

   fgets(buf, sizeof(buf), f); //read first line (skip header)

   while ( !feof(f) ) { // read until the end of the file
      int from, to, weight; 

      cbuf = next_token(buf, f, MAX_STRING_SIZE);
      from = atoi(buf);
      cbuf = next_token(buf, f, MAX_STRING_SIZE);
      to = atoi(buf);
      cbuf = next_token(buf, f, MAX_STRING_SIZE);
      weight = atoi(buf);

      add_edge(g, from, to, weight);

      if (cbuf == 1) continue;
   }
   fclose(f); //close file
   return 1;
}

int load_vertices ( char *fname ){  //loads the vertices from the CSV file of name fname
   FILE* f;
   char buf[1000];
   int cbuf = 0;
   f = fopen(fname, "r"); //read specified file

   if (!f) { //check if file exists, if not output message of error
     printf("Unable to open %s\n", fname);
     return 0; 
   }

   fgets(buf, sizeof(buf), f); //skip header to not count as line

   int count = 0;
   while (fgets(buf, sizeof(buf), f)) //number of stops
     count++;
   rewind(f); //go back to the beginning of the file 

   g = create_graph(count); //create graph 

   fgets(buf, sizeof(buf), f); //read first line (skip header)
   int i = 0; 
   while ( !feof(f) ) { // read until the end of the file
      cbuf = next_token(buf, f, MAX_STRING_SIZE);
      g->vertices[i].stop_id = atoi(buf);
      cbuf = next_token(buf, f, MAX_STRING_SIZE);
      strcpy(g->vertices[i].name, buf);
      cbuf = next_token(buf, f, MAX_STRING_SIZE);
      g->vertices[i].latitude = atof(buf);
      cbuf = next_token(buf, f, MAX_STRING_SIZE);
      g->vertices[i].longitude = atof(buf);
      i++;
      if (cbuf == 1) continue; 
   }
   fclose(f); //close file
   return 1;
}

void shortest_path(int startNode, int endNode){ // prints the shortest path between startNode and endNode, if there is any
    int permanent[MAX_VERTICES]; //stores permanent nodes (no further change in distances)
    int cost[MAX_VERTICES]; //stores the shortest distance
    int predecessor[MAX_VERTICES]; //keeps track of previous node (shortest distance)
    int num_nodes = g->num_stops;

    for (int i = 0; i < num_nodes; i++) { //initialise arrays
        cost[i] = 100000; //no set distance
        permanent[i] = 0; //no nodes are permanent
        predecessor[i] = -1; //no parents assigned (negative number for not assigned)
    }

    int startIndex = -1;
    for (int i = 0; i < num_nodes; i++) { //find the vertex with the corresponding stop id of starNode
        if (g->vertices[i].stop_id == startNode) {
            startIndex = i;
            break;
        }
    }
    if (startIndex == -1) {
        printf("Start node with stop_id %d not found.\n", startNode);
        return;
    }

    int endIndex = -1;
    for (int i = 0; i < num_nodes; i++) { //find the vertex with the corresponding stop id for endNOde
        if (g->vertices[i].stop_id == endNode) {
            endIndex = i;
            break;
        }
    }
    if (endIndex == -1) {
        printf("End node with stop_id %d not found.\n", endNode);
        return;
    }
    cost[startIndex] = 0; //starting distance is 0

    for (int i = 0; i < num_nodes; i++) { //go thorugh each node in the graph
        int min_cost = 100000; //initialisation for minimum cost and 
        int n = -1; //smallest distance node index not assigned yet

        for (int j = 0; j < num_nodes; j++) { //loop thorugh neighbour nodes
            if (!permanent[j] && cost[j] < min_cost) { //while its not permanent and its distance is smaller than the minimum distance
                min_cost = cost[j]; //set as new smallest distance
                n = j; //node with smallest distance 
            }
        }
        if (n==-1) break; 
        permanent[n] = 1; //make visited node permanent

        for (int n2 = 0; n2 < num_nodes; n2++) { //loop thorugh all nodes
            if (!permanent[n2] && g->adj_matrix[n][n2] != 100000) { //if its not permanent and actually are connected
                int new_cost = cost[n] + g->adj_matrix[n][n2]; //calculates the new distance from origin passin through previous node until new one
                if (new_cost < cost[n2]) { //if the new calculated distance is smaller
                    cost[n2] = new_cost; //shorter path found
                    predecessor[n2] = n; //update node predecessor
                }
            }
        }
    }

    if (cost[endIndex] == 100000) { //if no path connecting
        printf("There is no path from stop %d to stop %d.\n", startNode, endNode);
        return;
    }
    else {
    int travel[MAX_VERTICES]; //the path from the start node to end node
    int travel_length = 0;
    for (int current = endIndex; current != -1; current = predecessor[current]) { //track vertices from end to start
        travel[travel_length++] = current;
    }

    printf("Loaded %d vertices\n", g->num_stops);
    printf("Loaded %d edges\n", g->num_edges);

    for (int i = travel_length - 1; i >= 0; i--) {
        int stop_id = g->vertices[travel[i]].stop_id;
        char *name = g->vertices[travel[i]].name;
        double latitude = g->vertices[travel[i]].latitude;
        double longitude = g->vertices[travel[i]].longitude;

        printf("%d %s %f %f\n", stop_id, name, latitude, longitude);
    }
    }
}

void free_memory ( void ) { // frees any memory that was used
free(g); //free memory in graph
}

int next_token (char *buf, FILE *f, int string_max) { //reads strings of alpha numeric characters from input file. Truncates strings which are too long to string_max-1
   int i = 0;
   char c = fgetc(f);
   bool quotes = false;
   while (c != EOF) {
      if (c == '"')
        quotes = !quotes;
      else if (c == ',' && !quotes)
        break; //stop on comma if not inside quotes
      else if (c == '\n' && !quotes)
        break; //stop on newline if not inside quotes
      else if (i < string_max - 1) {
        buf[i] = c; //store the character in the buffer
        i++;
      }
    c = fgetc(f); //read in next character
   }
   buf[i] = '\0'; //to create a valid string we need to null-terminate
   if (c == ',') return 0;
   if (c == EOF || c == '\n') return 1;
   return 1;
}


/*void print_all_vertices() {
    // Check if the graph is initialized
    if (!g || !g->vertices) {
        printf("Graph is not initialized.\n");
        return;
    }

    printf("List of all vertices (stops):\n");
    for (int i = 0; i < 10; i++) {
        printf("Stop ID: %d\n", g->vertices[i].stop_id);
        printf("Name: %s\n", g->vertices[i].name);
        printf("Latitude: %f\n", g->vertices[i].latitude);
        printf("Longitude: %f\n", g->vertices[i].longitude);
        printf("----------------------------\n");
    }
} */

/*void print_graph() {
    int num_nodes = g->num_stops; // The number of vertices in the graph
    
    // Print all vertices with their connections and the edge weights
    for (int i = 0; i < num_nodes; i++) {
        printf("Vertex %d (%s) is connected to:\n", g->vertices[i].stop_id, g->vertices[i].name);

        // Loop over all other vertices to check for edges
        for (int j = 0; j < num_nodes; j++) {
            // If there is a connection (non-infinite weight)
            if (g->adj_matrix[i][j] != 100000) {
                printf("  - Vertex %d (%s) with weight %d\n", g->vertices[j].stop_id, g->vertices[j].name, g->adj_matrix[i][j]);
            }
        }
        printf("\n"); // Print a newline between vertices for readability
    }
} */
