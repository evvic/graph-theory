#ifndef DFS_ASYNC_ARBITRAGE
#define DFS_ASYNC_ARBITRAGE

#include "../../utilities/edges/c2c-edge.h"
#include "../../utilities/Binance/api-limits.h"
#include "../../utilities/Binance/quote-edge.h"
#include <vector>
#include <limits>
#include <stack>
#include <map>

// AsyncArbitrage performs each API call getting the quoted convert rate sequentially
// Although this means running through the entire adjacency list finding circualr 
// arbitrage is slower, the true bottleneck (limit) is the Binance API call limit
// put on for the number of API calls allowed every minute, hour, and day.

// Binance Convert API has no trade fees (included in conversion rate)
// therefore no additional math required for calculating potential profit
// Slippage is also not necessary to account for because the quoted rate is locked in
// for the given amount of time (can be adjusted in API call parameter)

class AsyncArbitrage {
protected:
    // struct to hold (max) profit & path of edges to get it
    struct ProfitPath {
    private:
        unsigned short _id;         // ID of start node

    public:
        double profit;              // Current best profit
        std::vector<C2CEdge> path;   // Current path for best profit
        
        // Start node should be immutable after initalization
        // Profit should be initialized to 0.0
        ProfitPath(unsigned short i = 0) : _id(i), profit(0.0) {};
        unsigned short origin() { return this->_id; }  
    };

    // DFS for finding circular arbitrage
    void dfs(const std::vector<std::vector<C2CEdge>>& graph, std::vector<bool>& visited, ProfitPath& tpath, unsigned short current);

private:
    int V;                                  // Number of vertices
    std::vector<std::vector<C2CEdge>> adj;  // Adjacency list

    ProfitPath best;                        // For tracking best profit with its path
    LimitTracker tracker;                   // Class to track when API Limit is reached
    std::map<std::string, double> wallet;   // map of coins and their current balance

    // Returns the calcualated profit given the circular path
    // Pass tradeCircle by ref to return the circular trade of the quoted rates and amounts
    double calculateProfit(const ProfitPath& tpath, std::vector<QuoteEdge>& tradeCircle);

    // Performs all API calls for each conversion
    void executeCircularTrade(const std::vector<QuoteEdge>& tradeCircle);

public:
    // Constructor requires number of vertices
    AsyncArbitrage(int V);

    // Add a directed edge to->from with the given rate (multiplicative for DFS)
    void addEdge(C2CEdge e);
    
    // Entry point into recursive dfs() function
    // Returns vector of edges for most profitable circular path
    // Returns empty vector if no profitable path found
    std::vector<C2CEdge> findCircularArbitrage();

};

#endif