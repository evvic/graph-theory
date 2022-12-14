#ifndef DIJKSTRAS_SHORTEST_PATH_D_HEAP
#define DIJKSTRAS_SHORTEST_PATH_D_HEAP

#include <vector>
// Graph_Theory/Data_Structures/IndexedPriorityQueue.h
#include "../../utilities/IndexedPriorityQueue.h"

class DijkstrasShortestPathDHeap {
protected:
    // Edge class to represent a directed edge
    // between 2 nodes with a weighted cost
    class Edge {
    public:
        int to;
        double cost;
        // Using constructor initializer list
        //Edge(int to, double cost) : to(to), cost(cost) {}
        Edge(int to, double cost);
        Edge();
    };

private:
    int n;
    int edgeCount;
    //std::vector<double> dist;
    std::vector<int> prev;
    std::vector<std::vector<Edge>> graph;

    void createEmptyGraph();

public:
    // Init solver with graph size & starting node
    // Use addEdge() to add edges to graph
    DijkstrasShortestPathDHeap(int n);

    void addEdge(int from, int to, double cost);

    // Run the solver on a directed graph to find shortest path
    // Returns cost (shortest amount) to travel from start to end
    // If no path, returns "INFINITY" for double data type
    double dijkstra(int start, int end);

    // Reconstructs the shortest path from start to end (inclusive)
    // Returns an empty array if start and end are not connected
    vector<int> reconstructPath(int start, int end);

    void display(indexed_priority_queue<int, double> IPQ);

};

#endif