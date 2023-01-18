#include "../utilities/graphviz.h"
#include "../utilities/Binance/c2c-json.h"
#include "../utilities/Binance/convert.h"
#include "../utilities/Binance/c2c-buy.h"
#include "../utilities/Binance/api-limits.h"
#include "../DFS/circular-arbitrage/dfs-arbitrage.h"
#include "../DFS/circular-arbitrage/threaded-arbitrage.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool isCircular(vector<C2CEdge>& cycle);

int main() {

    // Init DFS class
    // ArbitrageDFS alg(num_v);

    // // Populate adjacency list in Arbitrage graph class with edges vect
    // for (auto edge : edges) {
    //     alg.addEdge(edge);
    // }

    // /* DFS circular arbitrage */
    // vector<C2CEdge> cycles = alg.findCircularArbitrage(std::vector<unsigned short>());
    // // There seems to be some bugs with the path....

    // cout << "Negative cycles: " << endl;
    // for (auto edge : cycles) {
    //     cout << edge << endl;
    // }

    // // ...
    // // Check vector is circular and each edge connects
    // if (!isCircular(cycles)) {
    //     cerr << "Error in circualr trade" << endl;
    // }

    // ...
    // Perform API call to buy
    // Create new class for this method
    // Log any trades or attempts to trade
    // MarketBuyC2C buyer;

    //string resp = buyer.viewWalletContents();

    //cout << resp << endl;

    // MarketBuyC2C::getMappedWallet();

    
    // string resp = buyer.marketBuy("USDTBNB", 0.01);
    // cout << resp << endl;


    ///////////////// convert class
    // Init vect to be populated with all C2C edges
    vector<C2CEdge> co_edges;

    BinanceConvert convert;

    convert.populateEdges(co_edges);
    cout << "edges size: " << co_edges.size() << endl;
    cout << "number of vertices: " << convert.getVerticesCount() << endl;

    // init with number of vertices
    ThreadedArbitrage method(convert.getVerticesCount());

    // Populate edges
    for (auto edge : co_edges) {
        method.addEdge(edge);
    }

    //method.parallelSetEdges(co_edges);

    auto cycles = method.findCircularArbitrage();
    
    for (auto edge : cycles) {
        cout << edge << endl;
    }

}

bool isCircular(vector<C2CEdge>& cycle) {
    // first check size
    if (cycle.size() < 2) return false;

    // conditional if circular
    return (cycle[0].from == cycle[cycle.size() - 1].to)? true : false;
}