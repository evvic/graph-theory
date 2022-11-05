#ifndef FLOYD_WORSHALL
#define FLOYD_WORSHALL

#include <vector>
#include <limits>

// APSP = All Pairs Shortest Path

// N = size of the adjacency matrix
// dp = the memo table that will contain APSP solution (shortest path)
// next = matrix used to reeconstruct shortest path

class FloydWorshall {

private:
    int n;          // number of nodes (vertices)
    bool solved;    // track if it has been solved

    std::vector<std::vector<double>> dp;    // adjacency matrix
    std::vector<std::vector<int>> next;     // matrix to reconstruct the path

    // Constants
    static constexpr int REACHES_NEGATIVE_CYCLE = -1; // used to determine when reached neg cycles

    static constexpr double POSITIVE_INFINITY = std::numeric_limits<double>::max();
    static constexpr double NEGATIVE_INFINITY = std::numeric_limits<double>::min();

    /// PRIVATE FUNCTIONS
    void createEmptyGraph();

    void setup();       // allocate memory for tables

public:
    // Init constructor with adjacency matrix
    // Use addEdge() to add edges to graph
    FloydWorshall(std::vector<std::vector<double>> matrix);

    // Run the solver on a directed graph to find shortest path
    // Returns cost (shortest amount) to travel from start to end
    // If no path, returns "INFINITY" for double data type
    void solve();

    // Runs the APSP and calls solve()
    std::vector<std::vector<double>> getApspMatrix();

    // Reconstructs the shortest path from start to end (inclusive)
    // Returns an empty array if start and end are not connected
    // Returns null if start & end are connected but in a negative cycle
    std::vector<int> reconstructPath(int start, int end);
};

#endif