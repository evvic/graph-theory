#include "connected-components.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
    cout << "Connected Components Test" << endl;


    // Adjacency List:
    vector<vector<pair<int, double>>> al;

    al.push_back({{ 1, 0.0 }}); // 0
    al.push_back({{ 2, 0.0 }}); // 1
    al.push_back({{ 0, 0.0 }}); // 2
    al.push_back({{ 4, 0.0 }}); // 3
    al.push_back({{ 5, 0.0 }}); // 4
    al.push_back({{ 4, 0.0 }}); // 5
    al.push_back({{ 7, 0.0 }}); // 6
    al.push_back({});           // 7

    // Construct DFS, connected components class
    ConnectedComponents thing(8, al);

    vector<int> components = thing.findComponents();

    cout << "Num components " << components.size() << endl;

    for (int i = 0; i < components.size(); i++) {
        cout << "Node " << i << " in component " << components.at(i) << endl;
    }




}