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
// void BellmanFordC2C::createEmptyGraph() {
//     // Init only 1 dimension of the graph
//     graph = vector<vector<EdgeC2C>>(nodeCount);
// }

// void BellmanFordC2C::addEdge(int from, int to, double cost) {
//     edgeCount++;
//     graph.at(from).push_back(Edge(to, cost));
// }

// E = num edges
// V = num vertices (nodes)
// S = id of starting node
// D = array of size V tracking best distance from S to each node
//     init every element in D to positive infinity
//     D[S] = 0
// CURRENTLY ONLY WORKS IF START NODE IS ZER0
//std::vector<double> bellmanford(int start);