#include <iostream>
#include <vector>
#include <string>
#include "bellman-ford-c2c.h"
#include "../../utilities/graphviz.h"
//#include "../../utilities/c2c-csv.h"
#include "../../utilities/c2c-json.h"

using namespace std;

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

    // Populate adjacency list in BF class with edges vect
    for (auto edge : edges) {
        alg.addEdge(edge.id, edge.to, edge.rate);
    }

    auto neg_cycles = alg.bellmanFord(1);

    cout << "Negative cycles: " << endl;
    for (auto node : neg_cycles) {
        cout << node.u << "->" << node.v << ' ' << node.weight << endl;
    }

}

