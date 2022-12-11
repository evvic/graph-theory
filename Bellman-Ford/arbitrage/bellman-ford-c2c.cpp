#include "bellman-ford-c2c.h"
#include "../../utilities/c2c-edge.h"
#include <vector>
#include <limits> // for initializing to positive infinity
#include <iostream>

using namespace std;

// CONSTRUCTOR
BellmanFordC2C::BellmanFordC2C(int n) {
    this->nodeCount = n;
    //createEmptyGraph();
}

// private member function to create empty graph
void BellmanFordC2C::createEmptyGraph() {
    // Init only 1 dimension of the graph
    graph = vector<vector<C2CEdge>>(nodeCount);
}

void BellmanFordC2C::addEdge(C2CEdge edge) {
    edgeCount++;
    graph.at(edge.id).push_back(edge);
}

// E = num edges
// V = num vertices (nodes)
// S = id of starting node
// D = array of size V tracking best distance from S to each node
//     init every element in D to positive infinity
//     D[S] = 0
// CURRENTLY ONLY WORKS IF START NODE IS ZER0
vector<double> BellmanFordC2C::bellmanford(int start) {

    // Maintain an array of min distance to each node
    vector<double> dist(nodeCount, numeric_limits<double>::max());
    dist.at(start) = 0.0;

    // Outter loop for BF redundancy checking
    for (int i = 0; i < nodeCount - 1; i++) {
        // Compare ALL edges in 2D graph
        for (int j = 0; j < graph.size(); j++) {
            for ( auto edge : graph.at(j)) {
                // Relax edge (update dist vect with shortest path to node)
                if (dist.at(j) + edge.rate < dist.at(edge.to)) {
                    dist.at(edge.to) = dist.at(j) + edge.rate;
                }
            }
        }
    }

    // Repeat to find nodes caught in negative cycle
    for (int i = 0; i < nodeCount - 1; i++) {
        // Compare ALL edges in 2D graph
        for (int j = 0; j < graph.size(); j++) {
            for ( auto edge : graph.at(j)) {
                // Catch any negative cycles
                if (dist.at(j) + edge.rate < dist.at(edge.to)) {
                    dist.at(edge.to) = numeric_limits<double>::min();
                }
            }
        }
    }

    cout << "Cheapest distances from start node " << start << endl;
    for (auto num : dist) {
        cout << num << endl;
    }

    // check bounds, if not having end node return infinity
    return dist;

}