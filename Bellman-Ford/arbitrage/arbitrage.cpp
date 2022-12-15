#include <iostream>
#include <vector>
#include <string>
#include "bellman-ford-c2c.h"
#include "../../utilities/graphviz.h"
#include "../../utilities/c2c-json.h"
#include "../../DFS/circular-arbitrage/dfs-arbitrage.h"

using namespace std;

void findCircularArbitrage(const std::vector<std::vector<C2CEdge>>& graph);
void dfs(const std::vector<std::vector<C2CEdge>>& graph, std::vector<bool>& visited, std::vector<C2CEdge>& path, double& profit, unsigned short current);

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