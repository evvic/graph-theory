// main file to start Dijkstras Class

#include "DijkstrasShortestPathDHeap.h"
#include "../../utilities/graphviz.h"
#include <iostream>
// system()
#include <cstdlib>

using namespace std;

int main() {
    cout << "Dijstra's Algorithm implemented with Indexed Priority Queue" << endl;

    // Adjacency List:
    vector<vector<pair<int, double>>> al;

    // Structure of AL:
    vector<pair<int, double>> zero = {{ 1, 5.0 }, { 2, 1.0 }};
    vector<pair<int, double>> one = {{ 2, 2.0 }, { 3, 3.0 }, { 4, 20.0 }};
    vector<pair<int, double>> two = {{ 1, 3.0 }, { 4, 12.0 }};
    vector<pair<int, double>> three = {{ 2, 3.0 }, { 4, 2.0 }, { 5, 6.0 }};
    vector<pair<int, double>> four = {{ 5, 1.0 }};

    // Add all edges to list
    al.push_back({{ 1, 5.0 }, { 2, 1.0 }});
    al.push_back({{ 4, 20.0 }, { 3, 3.0 }, { 2, 2.0 }});
    al.push_back({{ 1, 3.0 }, { 4, 12.0 }});
    al.push_back({{ 2, 3.0 }, { 4, 2.0 }, { 5, 6.0 }});
    al.push_back({{ 5, 1.0 }});
    al.push_back({});

    // Init DijkstrasShortestPathDHeap with num of nodes
    DijkstrasShortestPathDHeap alg(al.size());

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

    cout << "Running Dijkstra's Algorithm..." << endl;
    vector<int> path = alg.reconstructPath(0, 5);

    // Give path
    gviz.setPath(path);
    // Pass file name + directory
    gviz.write("../graphs/gtest");
    gviz.render("../graphs/gtest");

    cout << "Path length: " << path.size() << endl;
    for ( auto node : path) {
        cout << node << " ";
    }

    return 1;
}