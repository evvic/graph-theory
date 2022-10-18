#include <iostream>
#include <vector>
#include "IndexedPriorityQueue.h"

using namespace std;

class DijkstrasShortestPathDHeap {
protected:
    // Edge class to represent a directed edge
    // between 2 nodes with a weighted cost
    class Edge {
    private:
        int to;
        double cost;
    public:
        // Using constructor initializer list
        //Edge(int to, double cost) : to(to), cost(cost) {}
        Edge(int to, double cost) {
            this->to = to;
            this->cost = cost;
        }
    };

private:
    int n;
    int edgeCount;
    std::vector<double> dist;
    std::vector<int> prev;
    std::vector<std::vector<Edge>> graph;

    void createEmptyGraph() {
        graph = vector<vector<Edge>>(n, vector<Edge>(n));
    }

public:
    // Init solver with graph size & starting node
    // Use addEdge() to add edges to graph
    DijkstrasShortestPathDHeap(int n) {
        this->n = n;
        createEmptyGraph();
    }

    void addEdge(int from, int to, double cost) {
        edgeCount++;
        graph.at(from).push_back(Edge(to, cost));
    }

    // Run the solver on a directed graph to find shortest path
    // Returns cost (shortest amount) to travel from start to end
    // If no path, returns "INFINITY" for double data type
    double dijkstra(int start, int end) {

        // Keep an Indexed Priority Queue (ipq) of best node to visit
        int degree = edgeCount / n;


    }

};