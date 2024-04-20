#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 1000

// A structure to represent an edge between two vertices
struct Edge {
    int v;            // Vertex v (or "to" vertex)
    int flow;         // flow of data in edge
    int capacity;     // capacity
    int reverse_edge; // To store index of reverse edge in adjacency list
};

// A structure to represent a vertex in the graph
struct Vertex {
    struct Edge* edges[MAX_VERTICES]; // Array of edges
};

// A structure to represent the graph
struct Graph {
    int V; // Number of vertices
    struct Vertex vertices[MAX_VERTICES]; // Array of vertices
};

// Create a new edge
struct Edge* createEdge(int v, int capacity) {
    struct Edge* edge = (struct Edge*)malloc(sizeof(struct Edge));
    edge->v = v;
    edge->flow = 0;
    edge->capacity = capacity;
    edge->reverse_edge = -1; // Initially set to -1
    return edge;
}

// Add an edge to the graph
void addEdge(struct Graph* graph, int u, int v, int capacity) {
    // Forward edge
    struct Edge* forward_edge = createEdge(v, capacity);
    graph->vertices[u].edges[v] = forward_edge;

    // Backward edge (reverse edge)
    struct Edge* backward_edge = createEdge(u, 0); // Capacity of reverse edge is 0
    graph->vertices[v].edges[u] = backward_edge;

    // Update reverse edge pointers
    forward_edge->reverse_edge = v;
    backward_edge->reverse_edge = u;
}

// BFS to check if there exists a path from source to sink
bool BFS(struct Graph* graph, int source, int sink, int parent[]) {
    bool visited[MAX_VERTICES] = {false};
    visited[source] = true;
    parent[source] = -1;

    // Create a queue for BFS
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;
    queue[rear++] = source;

    while (front != rear) {
        int u = queue[front++];
        for (int v = 0; v < graph->V; v++) {
            struct Edge* edge = graph->vertices[u].edges[v];
            if (!visited[v] && edge && edge->flow < edge->capacity) {
                queue[rear++] = v;
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return visited[sink];
}

// Implementation of Dinic's Algorithm to find maximum flow
int dinicsMaxFlow(struct Graph* graph, int source, int sink) {
    int max_flow = 0;
    int parent[MAX_VERTICES];

    // While there exists a path from source to sink
    while (BFS(graph, source, sink, parent)) {
        int path_flow = INT_MAX;

        // Find bottleneck capacity (minimum residual capacity) of the path
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            struct Edge* edge = graph->vertices[u].edges[v];
            path_flow = (path_flow < edge->capacity - edge->flow) ? path_flow : (edge->capacity - edge->flow);
        }

        // Update flow along the path
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            struct Edge* edge = graph->vertices[u].edges[v];
            edge->flow += path_flow;
            // Update reverse edge
            graph->vertices[v].edges[u]->flow -= path_flow;
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    return max_flow;
}

// Read graphs from input file
void readGraphsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    while (!feof(file)) {
        int V, E;
        if (fscanf(file, "%d %d", &V, &E) != 2) {
            break; // Reached end of file
        }

        printf("Graph with %d vertices and %d edges:\n", V, E);

        // Create graph
        struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
        graph->V = V;

        // Initialize all edges to NULL
        for (int i = 0; i < MAX_VERTICES; i++) {
            for (int j = 0; j < MAX_VERTICES; j++) {
                graph->vertices[i].edges[j] = NULL;
            }
        }

        // Read edges from the file and add them to the graph
        for (int i = 0; i < E; i++) {
            int u, v, capacity;
            fscanf(file, "%d %d %d", &u, &v, &capacity);
            addEdge(graph, u, v, capacity);
        }

        // Find source and sink vertices
        int source = 0; // Source vertex
        int sink = graph->V - 1; // Sink vertex (last vertex)

        // Find and print maximum flow
        printf("Maximum flow: %d\n", dinicsMaxFlow(graph, source, sink));
    }

    fclose(file);
}

int main() {
    const char* filename = "input.txt"; // Input file name
    readGraphsFromFile(filename);
    return 0;
}
