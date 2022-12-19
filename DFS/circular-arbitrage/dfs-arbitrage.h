#ifndef DFS_ARBITRAGE
#define DFS_ARBITRAGE

#include <vector>
#include <limits>
#include <stack>
#include "../../utilities/c2c-edge.h"

class ArbitrageDFS {
protected:
    // struct to hold (max) profit & path of edges to get it
    struct ProfitPath {
    private:
        unsigned short _id;         // ID of start node

    public:
        double profit;              // Current best profit
        std::stack<C2CEdge> path;   // Current path for best profit
        
        // Start node should be immutable after initalization
        // Profit should be initialized to 0.0
        ProfitPath(unsigned short i = 0) : _id(i), profit(0.0) {};
        unsigned short origin() { return this->_id; }  
    };

    // DFS for findiong circular arbitrage
    void dfs(const std::vector<std::vector<C2CEdge>>& graph, std::vector<bool>& visited, ProfitPath& tpath, unsigned short current);

private:
    int V;                                  // Number of vertices
    ProfitPath best;                        // For tracking best profit with its path
    std::vector<std::vector<C2CEdge>> adj;  // Adjacency list

public:
    // Constructor requires number of vertices
    ArbitrageDFS(int V);

    // Add a directed edge to->from with the given rate (multiplicative for DFS)
    void addEdge(C2CEdge e);
    
    // Entry point into recursive dfs() function
    // Returns vector of edges for most profitable circular path
    // Returns empty vector if no profitable path found
    std::vector<C2CEdge> findCircularArbitrage(std::vector<unsigned short> traversal_IDs);

};

#endif