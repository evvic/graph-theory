#include "bellman-ford-c2c.h"
#include "../../utilities/c2c-edge.h"
#include <vector>
#include <limits> // for initializing to positive infinity
#include <iostream>

using namespace std;

// CONSTRUCTOR
BellmanFordC2C::BellmanFordC2C(int _V) : V(_V), adj(_V) {}

// add a directed edge from u to v with the given weight
void BellmanFordC2C::addEdge(C2CEdge e) {
    // weight is calculated in the C2CEdge constructor
    adj[e.from].push_back(C2CEdge(e.from, e.to, e.rate, e.fromAsset, e.toAsset));
}

//u = from, v = to

// invoke the Bellman-Ford algorithm to detect negative cycles
std::vector<C2CEdge> BellmanFordC2C::bellmanFord(int source) {
    // initialize distances to infinity
    std::vector<int> distance(V, std::numeric_limits<int>::max());
    distance[source] = 0; // distance from source to source is 0

    // relax edges repeatedly
    for (int i = 0; i < V - 1; i++) {
        for (int u = 0; u < V; u++) {
            for (auto& edge : adj[u]) {
            int v = edge.to;
            int weight = edge.weight;
            distance[v] = std::min(distance[v], distance[u] + weight);
            }
        }
    }

    // check for negative-weight cycles
    std::vector<C2CEdge> negativeCycle;
    for (int u = 0; u < V; u++) {
        for (auto& edge : adj[u]) {
            int v = edge.to;
            int weight = edge.weight;
            if (distance[v] > distance[u] + weight) {
                negativeCycle.emplace_back(edge);
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

//////
// function that takes in adjacency list and finds currency conversion arbitrage
// function that takes in adjacency list and finds currency conversion arbitrage
vector<C2CEdge> BellmanFordC2C::findArbitrage() {
    vector<C2CEdge> negativeCycle; // vector to store vertices/edges involved in negative cycle  

       // loop through all currencies in adjacency list
    for (int i = 0; i < adj.size(); i++) {
        // initialize distances to be infinite
        vector<double> distances(adj.size(), numeric_limits<double>::max());
        distances[i] = 0; // set distance of starting currency to be 0
        
        // perform bellman-ford algorithm to find negative weight cycle
        for (int j = 0; j < adj.size(); j++) {
            for (int k = 0; k < adj.size(); k++) {
                for (int l = 0; l < adj[k].size(); l++) {
                    int neighbor = adj[k][l].to;
                    double weight = adj[k][l].weight;
                    if (distances[k] != numeric_limits<double>::max() && distances[neighbor] > distances[k] + weight) {
                        distances[neighbor] = distances[k] + weight;
                        // if negative weight cycle is found, add vertices/edges to negativeCycle vector
                        if (j == adj.size() - 1) {
                            negativeCycle.emplace_back(adj[k][l]);
                        }
                    }
                }
            }
        }
    }

    return negativeCycle;
}

// vector<vector<C2CEdge>> BellmanFordC2C::findArbitrage() {
//     vector<vector<C2CEdge>> negativeCycle; // vector to store negative weight cycles

//     // loop through all currencies in adjacency list
//     for (int i = 0; i < adj.size(); i++) {
//         // initialize distances to be infinite
//         vector<double> distances(adj.size(), numeric_limits<double>::max());
//         distances[i] = 0; // set distance of starting currency to be 0

//         // perform bellman-ford algorithm to find negative weight cycle
//         for (int j = 0; j < adj.size(); j++) {
//             for (int k = 0; k < adj.size(); k++) {
//                 for (int l = 0; l < adj[k].size(); l++) {
//                     int neighbor = adj[k][l].to;
//                     double weight = adj[k][l].weight;
//                     if (distances[k] != numeric_limits<double>::max() && distances[neighbor] > distances[k] + weight) {
//                         distances[neighbor] = distances[k] + weight;
//                         // if negative weight cycle is found, add vertices/edges to negativeCycle vector
//                         if (j == adj.size() - 1) {
//                             vector<C2CEdge> cycle;
//                             cycle.emplace_back(adj[k][neighbor]);
//                             negativeCycle.push_back(cycle);
//                         }
//                     }
//                 }
//             }
//         }
//     }
//     return negativeCycle;
// }