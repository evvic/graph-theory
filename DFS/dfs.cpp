#include "dfs.h"
#include <vector>

using namespace std;

// CONSTRUCTOR
DFS::DFS(int size, vector<vector<pair<int, double>>> al) {
    // Set all private variables
    setAdacencyList(al);
    setNumNodes(size);

    // Init visited vect to all false values
    visited.assign(size, false);
}

void DFS::dfs(int at) {
    // If already visited, return
    if (visited.at(at)) return;

    visited.at(at) = true;

    // Explore all neigbors of current node
    for (auto neighbor : al.at(at)) {
        dfs(neighbor.first);
    }

}

// SETTERS

void DFS::setAdacencyList(vector<vector<pair<int, double>>> al) {
    this->al = al;
}

void DFS::setNumNodes(int num) {
    this->n = num;
}