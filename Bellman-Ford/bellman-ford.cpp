#include "bellman-ford.h"
#include <vector>
#include <limits> // for initializing to positive infinity

using namespace std;

// CONSTRUCTOR
BellmanFord::BellmanFord(int n) {
    this->n = n;
    createEmptyGraph();
}

// Edge class constructor within BellmanFord
BellmanFord::Edge::Edge(int to, double cost) {
    this->to = to;
    this->cost = cost;
}

// private member function to create empty graph
void BellmanFord::createEmptyGraph() {
    // Init only 1 dimension of the graph
    graph = vector<vector<Edge>>(n);
}

void BellmanFord::addEdge(int from, int to, double cost) {
    edgeCount++;
    graph.at(from).push_back(Edge(to, cost));
}

// E = num edges
// V = num vertices (nodes)
// S = id of starting node
// D = array of size V tracking best distance from S to each node
//     init every element in D to positive infinity
//     D[S] = 0
double BellmanFord::bellmanford(int start, int end) {

    // Maintain an array of min distance to each node
    vector<double> dist(n, numeric_limits<double>::max());
    dist.at(start) = 0.0;

    // Outter loop for BF redundancy checking
    for (int i = 0; i < n - 1; i++) {
        // Compare ALL edges in 2D graph
        for (int j = 0; j < graph.size(); j++) {
            for ( auto edge : graph.at(j)) {
                // Relax edge (update dist vect with shortest path to node)
                if (dist.at(j) + edge.cost < dist.at(edge.to)) {
                    dist.at(edge.to) = dist.at(j) + edge.cost;
                }
            }
        }
    }

    // Repeat to find nodes caught in negative cycle
    
}