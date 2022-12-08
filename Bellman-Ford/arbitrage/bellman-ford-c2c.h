#ifndef BELLMAN_FORD_C2C
#define BELLMAN_FORD_C2C

#include <vector>
#include "../../utilities/c2c-edge.h"

// E = num edges
// V = num vertices
// S = id of starting node
// D = array of size V tracking best distance from S to each node
//     init every element in D to positive infinity
//     D[S] = 0


class BellmanFordC2C {

private:
    int nodeCount; // number of nodes (vertices)
    int edgeCount; // number of edges (connecting nodes 1 direction)
    //std::vector<double> dist;
    std::vector<int> prev;
    std::vector<std::vector<C2CEdge>> graph;  // AL of C2CEdges (.id member == index)

    void createEmptyGraph();

public:
    // Init solver with graph size & starting node
    // Use addEdge() to add edges to graph
    BellmanFordC2C(int n);

    void addEdge(C2CEdge edge);

    // Run the solver on a directed graph to find shortest path
    // Returns cost (shortest amount) to travel from start to end
    // If no path, returns "INFINITY" for double data type
    std::vector<double> bellmanford(int start);

    // Reconstructs the shortest path from start to end (inclusive)
    // Returns an empty array if start and end are not connected
    std::vector<int> reconstructPath(int start, int end);
};

#endif