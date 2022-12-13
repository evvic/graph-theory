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
    jsonReader.initRates2();

    

    vector<C2CEdge> edges;

    // json curl request to get exchange info
    jsonReader.populateEdges2(edges);

    cout << "Show edges" << endl;
    for (auto edge : edges) {
        cout << edge << endl;
    }

    cout << "rates map size: " << jsonReader.ratesSize() << endl;
    cout << "edges size: " << edges.size() << endl;

}

