#include <iostream>
#include <vector>
#include "bellman-ford.h"

using namespace std;

int main() {
    cout << "Bellman-Ford's Algorithm." << endl;

    // Adjacency List:
    vector<vector<pair<int, double>>> al;

    // Add all edges to list
    al.push_back({{ 1, 5.0 }, { 2, 1.0 }});
    al.push_back({{ 4, 20.0 }, { 3, 3.0 }, { 2, 2.0 }});
    al.push_back({{ 1, 3.0 }, { 4, 12.0 }});
    al.push_back({{ 2, 3.0 }, { 4, 2.0 }, { 5, 6.0 }});
    al.push_back({{ 5, 1.0 }});
    al.push_back({});

    // Init Bellman-Ford's with num of nodes
    BellmanFord alg(al.size());

    // Iterate through adjacency list to add all edges to class
    for (int i = 0; i < al.size(); i++) {
        for (auto edge : al.at(i)) {
            cout << "addEdge(" << i << ',' << edge.first << ',' << edge.second << ")  ";
            alg.addEdge(i, edge.first, edge.second);
        }
        cout << endl;
    }

    alg.bellmanford(0, 5);



    return 0;
}