#ifndef BELLMAN_FORD
#define BELLMAN_FORD

#include <vector>

// E = num edges
// V = num vertices
// S = id of starting node
// D = array of size V tracking best distance from S to each node
//     init every element in D to positive infinity
//     D[S] = 0

class BellmanFord {
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
    };

private:
    // number of nodes (vertices)
    int n;
    int edgeCount;
    //std::vector<double> dist;
    std::vector<int> prev;
    std::vector<std::vector<Edge>> graph;

    void createEmptyGraph();

public:
    // Init solver with graph size & starting node
    // Use addEdge() to add edges to graph
    BellmanFord(int n);

    void addEdge(int from, int to, double cost);

    // Run the solver on a directed graph to find shortest path
    // Returns cost (shortest amount) to travel from start to end
    // If no path, returns "INFINITY" for double data type
    double bellmanford(int start, int end);

    // Reconstructs the shortest path from start to end (inclusive)
    // Returns an empty array if start and end are not connected
    std::vector<int> reconstructPath(int start, int end);
};

#endif