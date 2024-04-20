from typing import List, Tuple
from collections import deque
import sys

MAX_VERTICES = 1000

class Edge:
    def __init__(self, v: int, capacity: int):
        self.v = v
        self.flow = 0
        self.capacity = capacity
        self.reverse_edge = -1

class Vertex:
    def __init__(self):
        self.edges = []

class Graph:
    def __init__(self, V: int):
        self.V = V
        self.vertices = [Vertex() for _ in range(V)]

    def add_edge(self, u: int, v: int, capacity: int):
        forward_edge = Edge(v, capacity)
        self.vertices[u].edges.append(forward_edge)

        backward_edge = Edge(u, 0)
        self.vertices[v].edges.append(backward_edge)

        forward_edge.reverse_edge = v
        backward_edge.reverse_edge = u

def BFS(graph: Graph, source: int, sink: int, parent: List[int]) -> bool:
    visited = [False] * MAX_VERTICES
    visited[source] = True
    parent[source] = -1

    q = deque([source])
    while q:
        u = q.popleft()
        for edge in graph.vertices[u].edges:
            v = edge.v
            if not visited[v] and edge.flow < edge.capacity:
                q.append(v)
                parent[v] = u
                visited[v] = True
    return visited[sink]

def dinicsMaxFlow(graph: Graph, source: int, sink: int) -> int:
    max_flow = 0
    parent = [-1] * MAX_VERTICES

    while BFS(graph, source, sink, parent):
        path_flow = sys.maxsize

        v = sink
        while v != source:
            u = parent[v]
            for edge in graph.vertices[u].edges:
                if edge.v == v:
                    path_flow = min(path_flow, edge.capacity - edge.flow)
                    break
            v = parent[v]

        v = sink
        while v != source:
            u = parent[v]
            for edge in graph.vertices[u].edges:
                if edge.v == v:
                    edge.flow += path_flow
                    for rev_edge in graph.vertices[v].edges:
                        if rev_edge.v == u:
                            rev_edge.flow -= path_flow
                            break
                    break
            v = parent[v]

        max_flow += path_flow

    return max_flow

def read_graphs_from_file(filename: str) -> List[Tuple[int, int, List[Tuple[int, int, int]]]]:
    graphs = []
    with open(filename, 'r') as file:
        while True:
            line = file.readline()
            if not line:
                break
            V, E = map(int, line.split())
            edges = []
            for _ in range(E):
                u, v, capacity = map(int, file.readline().split())
                edges.append((u, v, capacity))
            graphs.append((V, E, edges))
    return graphs

def main():
    filename = "input.txt"  # Input file name
    graphs_info = read_graphs_from_file(filename)

    for V, E, edges in graphs_info:
        print(f"Graph with {V} vertices and {E} edges:")
        graph = Graph(V)
        for u, v, capacity in edges:
            graph.add_edge(u, v, capacity)
        source = 0  # Source vertex
        sink = V - 1  # Sink vertex (last vertex)
        print("Maximum flow:", dinicsMaxFlow(graph, source, sink))

if __name__ == "__main__":
    main()
