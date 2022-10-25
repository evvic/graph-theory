#include <iostream>
#include <vector>
#include "bellman-ford.h"
#include "../Utilities/graphviz.h"

using namespace std;

int main() {
    cout << "Bellman-Ford's Algorithm." << endl;

    // Adjacency List:
    vector<vector<pair<int, double>>> al;

    // Add all edges to list
    // al.push_back({{ 1, 5.0 }, { 2, 1.0 }});
    // al.push_back({{ 4, 20.0 }, { 3, 3.0 }, { 2, 2.0 }});
    // al.push_back({{ 1, 3.0 }, { 4, 12.0 }});
    // al.push_back({{ 2, 3.0 }, { 4, 2.0 }, { 5, 6.0 }});
    // al.push_back({{ 5, 1.0 }});
    // al.push_back({});

    // Add all NEW edges to list
    al.push_back({{1, 5}});
    al.push_back({{2, 20}, { 6, 60 }, { 5, 30 }});
    al.push_back({{3, 10}, { 4, 75 }});
    al.push_back({{2, -15}});
    al.push_back({{9, 100}});
    al.push_back({{6, 5}, { 8, 50 }, { 4, 25 }});
    al.push_back({{7, -50}});
    al.push_back({{8, -10}});
    al.push_back({});
    al.push_back({});

    // Init Bellman-Ford's with num of nodes
    BellmanFord alg(al.size());

    // Init Graphviz class (for making .gv of AL's)
    Graphviz gviz{};

    // Set AL and write it to gtest.gv
    gviz.setAdjacencyList(al);

    // Iterate through adjacency list to add all edges to class
    for (int i = 0; i < al.size(); i++) {
        for (auto edge : al.at(i)) {
            cout << "addEdge(" << i << ',' << edge.first << ',' << edge.second << ")  ";
            alg.addEdge(i, edge.first, edge.second);
        }
        cout << endl;
    }

    alg.bellmanford(0, 9);

    // Pass file name + directory
    gviz.write("graphs/gtest");
    gviz.render("graphs/gtest");



    return 0;
}