#include <iostream>
#include <vector>
#include <string>
#include "bellman-ford-c2c.h"
#include "../../utilities/graphviz.h"
//#include "../../utilities/c2c-csv.h"
#include "../../utilities/c2c-json.h"
#include "../../DFS/circular-arbitrage/dfs-arbitrage.h"

using namespace std;

void findCircularArbitrage(const std::vector<std::vector<C2CEdge>>& graph);
void dfs(const std::vector<std::vector<C2CEdge>>& graph, std::vector<bool>& visited, std::vector<C2CEdge>& path, double& profit, unsigned short current);

int main() {
    RequestC2C jsonReader;

    // json curl request to get rates
    jsonReader.initRates2();

    vector<C2CEdge> edges;

    // json curl request to get exchange info
    jsonReader.populateEdges2(edges);

    // cout << "Show edges" << endl;
    // for (auto edge : edges) {
    //     cout << edge << endl;
    // }

    cout << "rates map size: " << jsonReader.ratesSize() << endl;
    cout << "edges size: " << edges.size() << endl;

    // Get num of vertices to init BF class
    auto num_v = jsonReader.getVerticesCount();

    // Init BF class
    BellmanFordC2C alg(num_v);

    // Init DFS class
    //ArbitrageDFS alg(num_v);

    // Populate adjacency list in BF class with edges vect
    for (auto edge : edges) {
        alg.addEdge(edge);
    }

    auto neg_cycles = alg.bellmanFord(1);

    //auto neg_cycles2 = alg.findArbitrage();

    // for (int i = 0; i < neg_cycles2.size(); i++) {
    //     cout << "Cycle #" << i << endl;
        
    //     for (auto node : neg_cycles2[i]) {
    //         cout << node.fromAsset << "->" << node.toAsset << ' ' << node.rate << endl;
    //     }
    // }

    for (auto node : neg_cycles) {
        //cout << node.fromAsset << "->" << node.toAsset << ' ' << node.rate << endl;
        cout << node << endl;
    }

    /* DFS circular arbitrage */
    //auto cycles = alg.findCircularArbitrage();

    // cout << "Negative cycles: " << endl;
    // for (auto edge : cycles) {
    //     cout << edge << endl;
    // }



}