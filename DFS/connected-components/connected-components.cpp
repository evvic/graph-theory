#include "connected-components.h"
#include <vector>

// Constructor
ConnectedComponents::ConnectedComponents(int s, std::vector<std::vector<std::pair<int, double>>> al) : DFS(s, al){
    // init count to 0
    count = 0;

    // Init vect
    components = std::vector<int> (s);

}

// Method for calculating components making up a graph
std::vector<int> ConnectedComponents::findComponents() {
    for (int i = 0; i < n; i++) {
        if (!visited.at(i)) {
            count++;
            ConnectedComponents::dfs(i);
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
            ConnectedComponents::dfs(neighbor.first);
        }

    }
}
