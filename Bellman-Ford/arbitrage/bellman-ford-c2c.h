#include <iostream>
#include <vector>
#include <limits>

// a simple class to represent a weighted edge in the graph
class Edge {
public:
    int u, v;
    int weight;

    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
};

// a class to represent a graph using an adjacency list
class BellmanFordC2C {
public:
    int V; // number of vertices
    std::vector<std::vector<Edge>> adj; // adjacency list

    BellmanFordC2C(int V);

    // add a directed edge from u to v with the given weight
    void addEdge(int u, int v, int weight);

    // invoke the Bellman-Ford algorithm to detect negative cycles
    std::vector<int> bellmanFord(int source);

    // Negate the reciprocal of the exchange rate. This converts it into a
    // weight that the Bellman-Ford algorithm can use to detect negative cycles.
    double convert_rate_to_additive(double exchange_rate);
};