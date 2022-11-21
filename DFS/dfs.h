#ifndef DEPTH_FIRST_SEARCH
#define DEPTH_FIRST_SEARCH

#include <vector>

// Depth-First Search Class
//

class DFS {
protected:

    // Variables
    int n;                      // Number of nodes in adjacency list
    std::vector<bool> visited;  // Vect of size n, set to true if node has visited

    // Adjacency List
    std::vector<std::vector<std::pair<int, double>>> al;

    // Setters - Private because everything is set once in the constructor
    void setAdacencyList(std::vector<std::vector<std::pair<int, double>>> al);
    void setNumNodes(int num);

public:

    // Constructor
    DFS(int size, std::vector<std::vector<std::pair<int, double>>> al);

    void dfs(int at);





};

#endif