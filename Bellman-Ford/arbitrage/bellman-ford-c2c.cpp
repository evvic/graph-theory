#include "bellman-ford-c2c.h"
#include "../../utilities/c2c-edge.h"
 #include <vector>
#include <limits> // for initializing to positive infinity
#include <iostream>

using namespace std;

// CONSTRUCTOR
BellmanFordC2C::BellmanFordC2C(int _V) : V(_V), adj(_V) {}

// add a directed edge from u to v with the given weight
void BellmanFordC2C::addEdge(int u, int v, double weight) {
    adj[u].push_back(Edge(u, v, convert_rate_to_additive(weight)));
}

// invoke the Bellman-Ford algorithm to detect negative cycles
std::vector<Edge> BellmanFordC2C::bellmanFord(int source) {
    // initialize distances to infinity
    std::vector<int> distance(V, std::numeric_limits<int>::max());
    distance[source] = 0; // distance from source to source is 0

    // relax edges repeatedly
    for (int i = 0; i < V - 1; i++) {
        for (int u = 0; u < V; u++) {
            for (auto& edge : adj[u]) {
            int v = edge.v;
            int weight = edge.weight;
            distance[v] = std::min(distance[v], distance[u] + weight);
            }
        }
    }

    // check for negative-weight cycles
    std::vector<Edge> negativeCycle;
    for (int u = 0; u < V; u++) {
        for (auto& edge : adj[u]) {
            int v = edge.v;
            int weight = edge.weight;
            if (distance[v] > distance[u] + weight) {
                negativeCycle.push_back(edge);
            }
        }
    }

    return negativeCycle;
}

// Negate the reciprocal of the exchange rate. This converts it into a
// weight that the Bellman-Ford algorithm can use to detect negative cycles.
double BellmanFordC2C::convert_rate_to_additive(double exchange_rate) {
    return -1.0 / exchange_rate;
}