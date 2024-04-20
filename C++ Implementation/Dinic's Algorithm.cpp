#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>

#define MAX_VERTICES 1000

using namespace std;

// A structure to represent an edge between two vertices
struct Edge {
    int v;            // Vertex v (or "to" vertex)
    int flow;         // flow of data in edge
    int capacity;     // capacity
    int reverse_edge; // To store index of reverse edge in adjacency list
};

// A structure to represent a vertex in the graph
struct Vertex {
    vector<Edge*> edges; // Array of edges
};

// A structure to represent the graph
struct Graph {
    int V; // Number of vertices
    vector<Vertex> vertices; // Array of vertices
};

// Create a new edge
Edge* createEdge(int v, int capacity) {
    Edge* edge = new Edge();
    edge->v = v;
    edge->flow = 0;
    edge->capacity = capacity;
    edge->reverse_edge = -1; // Initially set to -1
    return edge;
}

// Add an edge to the graph
void addEdge(Graph& graph, int u, int v, int capacity) {
    // Forward edge
    Edge* forward_edge = createEdge(v, capacity);
    graph.vertices[u].edges.push_back(forward_edge);

    // Backward edge (reverse edge)
    Edge* backward_edge = createEdge(u, 0); // Capacity of reverse edge is 0
    graph.vertices[v].edges.push_back(backward_edge);

    // Update reverse edge pointers
    forward_edge->reverse_edge = v;
    backward_edge->reverse_edge = u;
}

// BFS to check if there exists a path from source to sink
bool BFS(Graph& graph, int source, int sink, vector<int>& parent) {
    vector<bool> visited(MAX_VERTICES, false);
    visited[source] = true;
    parent[source] = -1;

    // Create a queue for BFS
    queue<int> q;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto edge : graph.vertices[u].edges) {
            int v = edge->v;
            if (!visited[v] && edge->flow < edge->capacity) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return visited[sink];
}

// Implementation of Dinic's Algorithm to find maximum flow
int dinicsMaxFlow(Graph& graph, int source, int sink) {
    int max_flow = 0;
    vector<int> parent(MAX_VERTICES);

    // While there exists a path from source to sink
    while (BFS(graph, source, sink, parent)) {
        int path_flow = INT_MAX;

        // Find bottleneck capacity (minimum residual capacity) of the path
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            for (auto edge : graph.vertices[u].edges) {
                if (edge->v == v) {
                    path_flow = min(path_flow, edge->capacity - edge->flow);
                    break;
                }
            }
        }

        // Update flow along the path
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            for (auto edge : graph.vertices[u].edges) {
                if (edge->v == v) {
                    edge->flow += path_flow;
                    // Update reverse edge
                    for (auto rev_edge : graph.vertices[v].edges) {
                        if (rev_edge->v == u) {
                            rev_edge->flow -= path_flow;
                            break;
                        }
                    }
                    break;
                }
            }
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    return max_flow;
}

// Read graphs from input file
void readGraphsFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    while (!file.eof()) {
        int V, E;
        if (!(file >> V >> E)) {
            break; // Reached end of file
        }

        cout << "Graph with " << V << " vertices and " << E << " edges:" << endl;

        // Create graph
        Graph graph;
        graph.V = V;
        graph.vertices.resize(V);

        // Read edges from the file and add them to the graph
        for (int i = 0; i < E; i++) {
            int u, v, capacity;
            file >> u >> v >> capacity;
            addEdge(graph, u, v, capacity);
        }

        // Find source and sink vertices
        int source = 0; // Source vertex
        int sink = graph.V - 1; // Sink vertex (last vertex)

        // Find and print maximum flow
        cout << "Maximum flow: " << dinicsMaxFlow(graph, source, sink) << endl;
    }

    file.close();
}

int main() {
    const char* filename = "input.txt"; // Input file name
    readGraphsFromFile(filename);
    return 0;
}
