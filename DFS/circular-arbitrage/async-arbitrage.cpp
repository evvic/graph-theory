#include "async-arbitrage.h"
#include "../../utilities/edges/c2c-edge.h"
#include "../../utilities/Binance/convert.h"
#include "../../utilities/Binance/c2c-buy.h"
#include "../../utilities/Binance/quote-edge.h"
#include "../../utilities/Binance/api-limits.h"
#include <vector>
#include <limits> // for initializing to positive infinity
#include <iostream>

using namespace std;

// CONSTRUCTOR
AsyncArbitrage::AsyncArbitrage(int _V) : V(_V), adj(_V) {}

// Add a directed edge to->from with the given rate (multiplicative for DFS)
void AsyncArbitrage::addEdge(const C2CEdge e) {
    adj[e.from].push_back(e);
}

// DFS for cicrular arbitrage
// Define a function to search for circular arbitrage opportunities using DFS
void AsyncArbitrage::dfs(const std::vector<std::vector<C2CEdge>>& graph, std::vector<bool>& visited, ProfitPath& tpath, unsigned short current) {
    
    // Return if current node has no edges
    if (graph.at(current).empty()) {
        cout << tpath.path.back().toAsset << " has no edges to explore" << endl;
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

        // If edge leads back to starting node but path length == 2, skip
        if (edge.to == tpath.origin() && tpath.path.size() < 3) {
            visited[edge.to] = true;
            continue;
        }

        // if edge leads back to starting node but path length too long, skip
        if (edge.to == tpath.origin() && tpath.path.size() > LimitTracker::MAX_CIRCULAR_TRADE_SIZE) {
            visited[edge.to] = true;
            continue;
        }


        // If the edge leads back to the starting node, we have found a circular path
        if (edge.to == tpath.origin()) {

            // Add edge completing the circular path
            tpath.path.push_back(edge);

            cout << "Returned to starting node!" << endl;

            // Vect storing information from getting quotes for each conversion
            std::vector<QuoteEdge> tradeCircle;

            // function to iterate through the path to calculate profit (revenue)
            double profit = calculateProfit(tpath, tradeCircle);

            // Condition if the profit is greater than 1.0 (break-even point)
            // TEMP CHNAGED BREAK EVEN TO TEXT EXECUTING TRADES
            const double break_even = 0.98; //1.0
            
            // Execute circular trade
            if (profit > break_even) {
                std::cout << "profit good enough for breakeven" << std::endl;
                executeCircularTrade(tradeCircle);
            }
            
            // Remove edge completing circular path 
            tpath.path.pop_back();

            cout << "Total getQuote API calls made: " << tracker.getNumCalls() << endl << endl << endl;

        }

        // If the edge leads to an unvisited node, explore it recursively
        else if (!visited[edge.to]) {
            cout << "adding edge to path: " << edge << endl;

            // Add the current edge to the path
            tpath.path.push_back(edge);
            dfs(graph, visited, tpath, edge.to);

            cout << "REMOVING node to path: " << tpath.path.back() << endl;

            // Remove the current node from the path
            tpath.path.pop_back();

        }
    }

    // Mark the current node as unvisited
    visited[current] = false;
}



// Define a function to search for the best circular arbitrage opportunity
// traversal_IDs holds all crypto IDs that should be used as the initial point of circular arbitrage
// If traversal_IDs is empty, loop through all vertices
std::vector<C2CEdge> AsyncArbitrage::findCircularArbitrage() {
    // Initialize the visited vector to all false values
    std::vector<bool> visited(adj.size(), false);

    // Initialize the best profit to 0
    best.profit = 0.0;

    // get current wallet balances
    wallet = MarketBuyC2C::getMappedWallet();

    // Loop through all of the nodes in the graph
    for (unsigned short i = 0; i < adj.size() ; i++) {
        // Filter & skip nodes with no funds in wallet
        // For now just tseting with USDT
        if (adj[i].size() <= 0 || adj[i][0].fromAsset != "USDT") {
            std::cout << "Skipping dfs for " << adj[i][0].fromAsset << std::endl;
            continue;
        }


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

        // If the node has not been visited, explore it using DFS
        if (!visited.at(i)) {
            
            visited.at(i) = true;
            dfs(adj, tvisited, tempath, i);
        }

    }
    

    // Check if we found a profitable circular arbitrage opportunity
    if (best.profit > 1.0) {

        cout << "Found profit of " << best.profit << " given the following:" << endl;

        // Create a vector to hold the path of edges
        // Loop through the edges in the best path, adding them to the path vector
        std::vector<C2CEdge> edges(best.path.rbegin(), best.path.rend());

        // Return the path of edges for the best circular arbitrage opportunity
        return edges;
    }
    // If we did not find a profitable circular arbitrage opportunity, return an empty vector

    return std::vector<C2CEdge>();
}

double AsyncArbitrage::calculateProfit(const ProfitPath& tpath, std::vector<QuoteEdge>& tradeCircle) {

    // Init vect to empty
    // Will hold every edge quoting the conversion rate for a circular arbitrage
    tradeCircle = vector<QuoteEdge>(); 

    int weightUID;          // Temp for getting UID weight from API call
    double p = 1.0;         // Calculate the profit for the circular path
    double inverseP = 1.0;  // TODO: calculate the inverse treansaction

    // Init starting amount with amount in wallet
    double tAmount = wallet[tpath.path.at(0).fromAsset];

    // TEMP make it start with 50 cents and peform real trades
    tAmount = 0.50; //////////////////////////////////////////
    //////////////////////////////////////////////////////////

    // Check if loop can fully complete without reaching API call limit
    // given the number of edges (each edge == api call) 
    // If not enough free weight, wait till next minute to have more
    if (!tracker.enough1mFreeWeight(tpath.path.size())) {
        LimitTracker::waitTillNextMinute();
    }

    // If not enough API calls left in the hour, wait till next hour
    if (!tracker.enoughFreeCalls1h(tpath.path.size())) {
        LimitTracker::waitTillNextHour();
    }

    for (auto edge : tpath.path) {
        cout << edge.fromAsset << "->" << edge.toAsset << endl;

        // Start with double the min amount
        if (tAmount < edge.fromAssetMinAmount) {
            cout << "using edge.fromAssetMinAmount = " << edge.fromAssetMinAmount << " *2" << endl;
            tAmount = edge.fromAssetMinAmount * 2;
        }
        
        // Make API call to get quoted rate
        QuoteEdge quote = BinanceConvert::parseSendQuote(edge.fromAsset, edge.toAsset, tAmount, weightUID);

        // TODO: save quoteedges at least quote id
        tradeCircle.push_back(quote);
        
        // Keep track of amount of API calls & their accumulated weight
        tracker.incrememntCalls();    // incrememnt api call count
        tracker.updateUidWeight(weightUID);

        p *= quote.ratio;           // stack profit rate 
        tAmount = quote.toAmount;   // set amount to start with next iteration
    }

    // If the profit is greater than the current best, update the best profit
    if (p > best.profit) {
        cout << "Updating best profit: " << p << endl;

        best.profit = p;
        best.path = tpath.path;

    } else {
        cout << "Profit " << p << " is not better than best " << best.profit << endl;
    }

    // return calculated profit
    return p;
}



// Performs all API calls for each conversion
void AsyncArbitrage::executeCircularTrade(const std::vector<QuoteEdge>& tradeCircle) {
    // asynchronously perform each trade

    // If not enough API calls left in the hour, cannot perform trades
    if (!tracker.enoughFreeCalls1h(tradeCircle.size())) {
        //LimitTracker::waitTillNextHour();
        return;
    }

    // Check if quote edges have quoteId initialized, if null ("")
    // Binance did not give a quoteId therefore conersion cannot be made
    for (QuoteEdge edge : tradeCircle) {
        if (edge.quoteId == "") {
            std::cerr << edge.fromAsset << '-' << edge.toAsset << " has no quoteId" << std::endl;
            return;
        }
    }

    std::cout << "Executing circular trades!" << std::endl;

    int weightUID;          // Temp for getting UID weight from API call
    int decrement = tradeCircle.size();

    for (QuoteEdge edge : tradeCircle) {

        // Check if loop can fully complete without reaching API call limit
        // given the number of edges (each edge == api call) 
        // If not enough free weight, wait till next minute to have more
        if (!tracker.enough1mFreeWeight(decrement--)) {
            LimitTracker::waitTillNextMinute();
        }

        std::string orderStatus = BinanceConvert::parseAcceptQuote(edge, weightUID);

        tracker.updateUidWeight(weightUID);
        
        // If order processed, continue
        if (orderStatus == "SUCCESS") continue;

        // Else, wait (number of)s and check if order has processed
        else if (orderStatus == "PROCESS") {
            double wait_time = 5;
            
            std::cout << "orderStatus : " << orderStatus << std::endl;

            LimitTracker::waitForSeconds(wait_time);

            

        }
        else if (orderStatus == "ERROR") {

            std::cout << "orderStatus : " << orderStatus << std::endl;

        } else {
            std::cerr << "Unknown order status: " << orderStatus << std::endl;
        }

    }
}