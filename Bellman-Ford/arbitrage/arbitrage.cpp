#include <iostream>
#include <vector>
#include <string>
//#include "bellman-ford.h"
#include "../../utilities/graphviz.h"
//#include "../../utilities/c2c-csv.h"
#include "../../utilities/c2c-json.h"

using namespace std;

int main() {
    RequestC2C jsonReader;

    // json curl request to get rates
    jsonReader.initRates();

    vector<C2CEdge> edges;

    // json curl request to get exchange info
    //jsonReader.populateEdges(edges);

    cout << "Show edges" << endl;
    for (auto edge : edges) {
        cout << edge << endl;
    }

}

// int main() {
//     // name w/ filepath of dir
//     string fname = "binance_trading_pairs.csv";

//     ParseC2C c2c_parser(fname);

//     vector<C2CEdge> edges;

//     c2c_parser.readPairsToEdge(edges);

//     cout << "Show edges" << endl;
//     for (auto edge : edges) {
//         cout << edge << endl;
//     }

// }

