#ifndef DFS_THREADED_ARBITRAGE
#define DFS_THREADED_ARBITRAGE

#include <vector>
#include <limits>
#include <stack>
#include "../../utilities/edges/c2c-edge.h"
#include <tbb/tbb.h>

// Binance Convert API limitations
// sapi/ endpoints (the oens used for the cpnvert endpoints)
// Each endpoint with IP limits has an independent 12000 per minute limit.
// Each endpoint with UID limits has an independent 180000 per minute limit.
// Any endpoint has a hard limit of 1200 per minute
// GET /sapi/v1/convert/exchangeInfo  500 Weight(IP)
// POST /sapi/v1/convert/getQuote     200 Weight(UID)

// Therefore only 900 getQuote calls per minute

class ThreadedArbitrage {
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

    oneapi::tbb::enumerable_thread_specific<std::list<ProfitPath> > localPaths;

    int num_calls;  // counter of getQuote api calls per run of the alg

public:
    // Constructor requires number of vertices
    ThreadedArbitrage(int V);

    // Add a directed edge to->from with the given rate (multiplicative for DFS)
    void addEdge(C2CEdge e);

    void parallelSetEdges(/*const*/ std::vector<C2CEdge>& edges);
    
    // Entry point into recursive dfs() function
    // Returns vector of edges for most profitable circular path
    // Returns empty vector if no profitable path found
    std::vector<C2CEdge> findCircularArbitrage();

};

#endif