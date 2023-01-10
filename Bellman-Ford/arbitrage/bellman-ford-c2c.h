#ifndef BELLAMAN_FORD_C2C
#define BELLAMAN_FORD_C2C

#include <iostream>
#include <vector>
#include <limits>
#include <stack>
#include "../../utilities/edges/c2c-edge.h"

// a class to represent a graph using an adjacency list
class BellmanFordC2C {
private:
    int V; // number of vertices
    std::vector<std::vector<C2CEdge>> adj; // adjacency list

public:
    BellmanFordC2C(int V);

    // add a directed edge from u to v with the given weight
    void addEdge(C2CEdge e);

    // invoke the Bellman-Ford algorithm to detect negative cycles
    std::vector<C2CEdge> bellmanFord(int source);

    // Negate the reciprocal of the exchange rate. This converts it into a
    // weight that the Bellman-Ford algorithm can use to detect negative cycles.
    double convert_rate_to_additive(double exchange_rate);

    std::vector<C2CEdge> findArbitrage();
    // std::vector<std::vector<C2CEdge>> findArbitrage();

};

#endif