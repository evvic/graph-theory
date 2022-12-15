#include "bellman-ford-c2c.h"
#include "../../utilities/graphviz.h"
#include "../../utilities/c2c-json.h"
#include "../../DFS/circular-arbitrage/dfs-arbitrage.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {

    // Init vect to be populated with all C2C edges
    vector<C2CEdge> edges;

    // Class for making all API calls to get current C2C pairs info
    RequestC2C jsonReader;
    jsonReader.populateEdges(edges);

    // Get num of vertices to init Arbitrage graph class
    unsigned short num_v = jsonReader.getVerticesCount();

    // Check data
    cout << "rates map size: " << jsonReader.ratesSize() << endl;
    cout << "edges size: " << edges.size() << endl;

    // Init BF class
    // BellmanFordC2C alg(num_v);

    // Example using bellamnford alg to return edges in negative cycle
    // auto neg_cycles = alg.bellmanFord(1);

    // Another BF example with a different implementation
    // auto neg_cycles2 = alg.findArbitrage();

    // Init DFS class
    ArbitrageDFS alg(num_v);

    // Populate adjacency list in Arbitrage graph class with edges vect
    for (auto edge : edges) {
        alg.addEdge(edge);
    }

    /* DFS circular arbitrage */
    vector<C2CEdge> cycles = alg.findCircularArbitrage();

    cout << "Negative cycles: " << endl;
    for (auto edge : cycles) {
        cout << edge << endl;
    }

    // ...
    // Check vector is circular and each edge connects


    // ...
    // Perform API call to buy
    // Create new class for this method
    // Log any trades or attempts to trade

}