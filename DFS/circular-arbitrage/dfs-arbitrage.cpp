#include "dfs-arbitrage.h"
#include "../../utilities/c2c-edge.h"
#include <vector>
#include <limits> // for initializing to positive infinity
#include <iostream>

using namespace std;

// CONSTRUCTOR
ArbitrageDFS::ArbitrageDFS(int _V) : V(_V), adj(_V) {}

// Add a directed edge to->from with the given rate (multiplicative for DFS)
void ArbitrageDFS::addEdge(C2CEdge e) {
    // weight is calculated in the C2CEdge constructor
    adj[e.from].push_back(C2CEdge(e.from, e.to, e.rate, e.fromAsset, e.toAsset));
}

// DFS for cicrular arbitrage
// Define a function to search for circular arbitrage opportunities using DFS
void ArbitrageDFS::dfs(const std::vector<std::vector<C2CEdge>>& graph, std::vector<bool>& visited, ProfitPath& tpath, unsigned short current) {
    
    // Return if current node has no edges
    if (graph.at(current).empty()) {
        //cout << tpath.path.top().toAsset << " no edges to explore" << endl;
        return;
    }
    
    // Return if already visited
    if (visited[current]) {
        return;
    }
    
    // Mark the current node as visited
    visited[current] = true;

    // Loop through all of the edges connected to the current node
    for (const C2CEdge& edge : graph[current]) {
        //cout << "Exploring edge " << edge << endl;

        // If the edge leads back to the starting node, we have found a circular path
        if (edge.to == tpath.origin()) {
            // Calculate the profit for the circular path
            double p = 1.0;

            //cout << "adding edge to complete path: " << edge << endl;
            // Add edge completing the circular path
            tpath.path.push(edge);

            //cout << "Returned to startign node!" << endl;

            // Stack is not iterable, make a copy to iterate through
            stack<C2CEdge> path_copy(tpath.path);
            while (!path_copy.empty()) {
                p *= path_copy.top().rate;
                //cout << path_copy.top().toAsset << "<-" << path_copy.top().fromAsset << ' ';
                path_copy.pop();
            }

            // If the profit is greater than the current best, update the best profit
            if (p > tpath.profit) {
                //cout << "Profit is better with path.." << p << endl;

                tpath.profit = p;
                best.profit = p;
                best.path = tpath.path;
                // think of a way to store the profit path
            }
            //else cout << "Profit " << p << " is not better with path.." << endl;

            // Remove edge completing circular path (unnessary)
            //cout << "REMOVING node to complete path: " << tpath.path.top() << endl; 
            tpath.path.pop();
        }

        // If the edge leads to an unvisited node, explore it recursively
        else if (!visited[edge.to]) {
            //cout << "adding edge to path: " << edge << endl;

            // Add the current edge to the path
            tpath.path.push(edge);
            dfs(graph, visited, tpath, edge.to);

            //cout << "REMOVING node to path: " << tpath.path.top() << endl;

            // Remove the current node from the path
            tpath.path.pop();

        }

        
    }
    
    //cout << "REMOVING node to path: " << tpath.path.top() << endl;

    // Remove the current node from the path
    //tpath.path.pop();

    // Mark the current node as unvisited
    visited[current] = false;
}

// Define a function to search for the best circular arbitrage opportunity
// traversal_IDs holds all crypto IDs that should be used as the initial point of circular arbitrage
// If traversal_IDs is empty, loop through all vertices
std::vector<C2CEdge> ArbitrageDFS::findCircularArbitrage(std::vector<unsigned short> traversal_IDs) {
    // Initialize the visited vector to all false values
    std::vector<bool> visited(adj.size(), false);

    // Initialize the best profit to 0
    best.profit = 0.0;

    // Only loop through IDs 
    // Choose the currency IDs that the wallet already has funds for

    // If traversal_IDs is empty, loop through all vertices

    // Loop through all of the nodes in the graph
    for (unsigned short i = 0; i < adj.size() ; i++) {
        // Create a temp ProfitPath struct to hold path of traversal
        // Include origin of start of the DFS (where it should return to be circular)
        ProfitPath tempath(i);

        // DEBUG
        if(adj[i].size() > 0) {
            //cout << "Exploring vertex " << adj[i][0].fromAsset << ", has " << adj[i].size() << " edges" << endl;
        }
        else {
            cout << "Adjacency list for ID " << i << "is empty" << endl;
        }

        // Create a temp visited vect<> for exploring all optional paths to get back to start
        std::vector<bool> tvisited(adj.size(), false);
        // Set visited[i] = true???

        // If the node has not been visited, explore it using DFS
        if (!visited.at(i)) {
            visited.at(i) = true;
            dfs(adj, tvisited, tempath, i);

        }

        if (tempath.profit > best.profit) {
            // copy tembest data into best if true
            // already being done in DFS(?)
        }
    }
    

    // Check if we found a profitable circular arbitrage opportunity
    if (best.profit > 1.0) {

        cout << "Found profit of " << best.profit << " given the following:" << endl;

        // Create a vector to hold the path of edges
        std::vector<C2CEdge> edges;

        // Loop through the edges in the best path, adding them to the path vector
        stack<C2CEdge> temp(best.path);
        while (!temp.empty()) {

            cout << temp.top() <<endl;


            edges.push_back(temp.top());
            // Rotate vect to get edges in correct order
            std::rotate(edges.rbegin(), edges.rbegin() + 1, edges.rend());

            temp.pop();
        }

        // Return the path of edges for the best circular arbitrage opportunity
        return edges;
    }
    // If we did not find a profitable circular arbitrage opportunity, return an empty vector

    return std::vector<C2CEdge>();
}