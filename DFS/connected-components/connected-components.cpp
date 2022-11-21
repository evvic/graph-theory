#include "connected-components.h"
#include <vector>

// Method for calculating components making up a graph
std::vector<int> ConnectedComponents::findComponents() {
    for (int i = 0; i < n; i++) {
        if (!visited.at(i)) {
            count++;
            dfs(i);
        }
    }
    return components;
}

// Overwrite original DFS algorithm with adding components tracking
void ConnectedComponents::dfs(int at) {

    visited.at(at) = true;

    // count is the ID of the connected components
    components.at(at) = count;

    // Explore all neigbors of current node
    for (auto neighbor : al.at(at)) {
        // If not visited, continue into depth
        if (!visited.at(neighbor.first)) {
            dfs(neighbor.first);
        }

    }
}
