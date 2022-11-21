#ifndef CONNECTED_COMPONENTS
#define CONNECTED_COMPONENTS

#include "../dfs.h"

// Extends the Depth-First Search class

class ConnectedComponents : public DFS {
private:
    int count;                      // Count of components
    std::vector<int> components;    // Vect of num nodes to assign component ID

    // Overwrite original DFS algorithm with adding components tracking
    void dfs(int at);

public:

    // Returns the vect of node's IDs
    std::vector<int> findComponents();
};

#endif