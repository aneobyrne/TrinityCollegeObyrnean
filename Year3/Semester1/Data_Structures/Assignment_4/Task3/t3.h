#ifndef T3_H_
#define T3_H_
#define MAX_STRING_SIZE 100
#define MAX_VERTICES 8000

/*typedef struct Edge { //edges
    int vertex;
    int weight; //graph edge weights (distance)
    struct Edge* next; //next node in list
} Edge; */

typedef struct Vertex {
    int stop_id;
    char name[100];
    double latitude;
    double longitude;
} Vertex;

typedef struct Graph {
    int num_stops;
    int num_edges;
    Vertex* vertices; //array of vertices
    int adj_matrix[MAX_VERTICES][MAX_VERTICES]; //adjacency matrix
} Graph;

Graph* create_graph(int num_stops);
void add_edge(Graph *g, int from, int to, int weight);
int load_edges ( char *fname ); //loads the edges from the CSV file of name fname
int load_vertices ( char *fname );  //loads the vertices from the CSV file of name fname
void shortest_path(int startNode, int endNode); // prints the shortest path between startNode and endNode, if there is any
void free_memory ( void ) ; // frees any memory that was used
int next_token (char *buf, FILE *f, int string_max);

void print_all_vertices() ;

#endif
