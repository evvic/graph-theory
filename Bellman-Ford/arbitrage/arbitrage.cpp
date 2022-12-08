#include <iostream>
#include <vector>
#include <string>
//#include "bellman-ford.h"
#include "../../utilities/graphviz.h"
#include "../../utilities/c2c-csv.h"

using namespace std;

int main() {
    // name w/ filepath of dir
    string fname = "binance_trading_pairs.csv";

    ParseC2C c2c_parser(fname);

    vector<C2CEdge> edges;

    c2c_parser.readPairsToEdge(edges);

    for (auto edge : edges) {
        cout << "edge" << endl;
    }

}