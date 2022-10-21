#include <iostream>
#include <vector>
#include "DijkstrasShortestPathDHeap.h"
// Graph_Theory/Data_Structures/IndexedPriorityQueue.h
//#include "../../Data_Structures/IndexedPriorityQueue.h"

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

    // First parameter is key datatype
    // and it should be hashable
    // Second parameter is value datatype comparator
    // function (by default it implements maxheap)
    indexed_priority_queue<int, double> ipq;
    ipq.push(start, 0.0);

    // Maintain an array of min distance to each node
    double dist[n] = { 100.0 }; //CHANGE TO INFINITY / DOUBLE MAX
    dist[start] = 0.0;

    // Maintain array of nodes if visited
    bool visited[n] = { false };

    //prev = new Integer[n]

    while(!ipq.empty()) {
        int nodeID = ipq.top().first;

        visited[nodeID] = true;
        double minVal = ipq.top().second;
        ipq.pop();

        // Ignore if already found a better path
        if (minVal > dist[nodeID]) continue;

        for(Edge edge : graph.at(nodeID)) {

            // Cannot get a shorter path from a previosuly visited node
            if (visited[edge.to]) continue;

            // Relax edge through updating min cost
            double newDist = dist[nodeID] + edge.cost;

            if (newDist < dist[edge.to]) {
                prev[edge.to] = nodeID;
                dist[edge.to] = newDist;

                // Insert cost of node traversal or add new node to queue
                // FIND METHOD OF CHECKING IF NODE ALREADY EXISTS IN QUEUE
                // if (!ipq.) {

                // }
            }

        }
    }







    // temp
    return 0.0;
}
