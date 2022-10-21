#include <iostream>
#include <vector>
// Graph_Theory/Data_Structures/IndexedPriorityQueue.h
#include "DijkstrasShortestPathDHeap.h"

using namespace std;

// Edge class constructor within DijkstrasShortestPathDHeap
DijkstrasShortestPathDHeap::Edge::Edge(int to, double cost) {
    this->to = to;
    this->cost = cost;
}

DijkstrasShortestPathDHeap::Edge::Edge() { this->to = 0; this->cost = 0.0; }

// private member function to create empty graph
void DijkstrasShortestPathDHeap::createEmptyGraph() {
    graph = vector<vector<Edge>>(n, vector<Edge>(n));
}

DijkstrasShortestPathDHeap::DijkstrasShortestPathDHeap(int n) {
    this->n = n;
    createEmptyGraph();
}

void DijkstrasShortestPathDHeap::addEdge(int from, int to, double cost) {
    edgeCount++;
    graph.at(from).push_back(Edge(to, cost));
}

double DijkstrasShortestPathDHeap::dijkstra(int start, int end) {

    // Keep an Indexed Priority Queue (ipq) of best node to visit
    int degree = edgeCount / n;

    // temp
    return 0.0;
}
