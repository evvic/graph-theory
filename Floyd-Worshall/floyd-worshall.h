#ifndef FLOYD_WORSHALL
#define FLOYD_WORSHALL

#include <vector>

// APSP = All Pairs Shortest Path

// N = size of the adjacency matrix
// dp = the memo table that will contain APSP solution (shortest path)
// next = matrix used to reeconstruct shortest path

class FloydWorshall {
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
    bool solved;    // track if it has been solved

    std::vector<std::vector<double>> dp;    // matrix
    std::vector<std::vector<int>> next;     // matrix to reconstruct the path

    const static int REACHES_NEGATIVE_CYCLE = -1; // used to determine when reached neg cycles

    void createEmptyGraph();
    // allocate memory for tables
    void setup();

public:
    // Init constructor with adjacency matrix
    // Use addEdge() to add edges to graph
    FloydWorshall(std::vector<std::vector<double>> matrix);

    //void addEdge(int from, int to, double cost);

    // Run the solver on a directed graph to find shortest path
    // Returns cost (shortest amount) to travel from start to end
    // If no path, returns "INFINITY" for double data type
    std::vector<double> floydworshall();

    // Reconstructs the shortest path from start to end (inclusive)
    // Returns an empty array if start and end are not connected
    //std::vector<int> reconstructPath(int start, int end);
};

#endif