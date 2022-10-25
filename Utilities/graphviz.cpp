#include "graphviz.h"

#include <iostream>
#include <fstream>
#include <map>

using namespace std;

// Constructor
Graphviz::Graphviz(std::vector<std::vector<std::pair<int, double>>> a) {
    cout << "Graphviz constructor" << endl;
    setAdjacencyList(a);
}

Graphviz::Graphviz() {
    cout << "~GRAPHVIZ~" << endl;
}

void Graphviz::setPath(vector<int> p) {
    // Path must include start & end node
    path = p;
    mapPath();
}

void Graphviz::mapPath() {
    //first = init node, second = destination node
    for (int i = path.size() - 1; i > 0; i--) {
        path_map.insert(std::make_pair(path.at(i), path.at(i - 1)));
    }
}

void Graphviz::setAdjacencyList(std::vector<std::vector<std::pair<int, double>>> a) {
    al = a;
}

void Graphviz::write(std::string fname) {
    ofstream ofile;

    // If the file type (.gv) is appended, remove it.
    if(fname.substr(fname.length() - 4, fname.length() - 1) == ".gv") {
        fname = fname.substr(0, fname.length() - 4);
    }

    // Include file path directory
    // !!!! ^
    ofile.open (fname + ".gv");

    // Write initial bracket opener
    ofile << "digraph " << fname << " {\n";

    // Write the whole Adjacency List to the file
    writeDigraph(ofile);

    // Write closer bracket
    ofile << endl << "}";

    ofile.close();
}

void Graphviz::writeDigraph(ofstream& ofile) {

    for (int i = 0; i < al.size(); i++) {
        // Write the basic initial for each node
        ofile << endl << "    // node " << i << endl;

        for (auto node : al.at(i)) {

            // CHECK WITH IF STATEMENT USING MAP_PATH
            // IF  map.contains(i) && map.at(i) == node.first:
            //     THEN MAKE STYLE RED
            // Path is in ordewr (reversed)


            //if (path_map.find(i) != path_map.end())
            //if (path_map.count(i) > 0)
                //cout << "    conditions" << i << ": " << (path_map.count(i) > 0) << " && " << path_map.at(i) << " == " << node.first << endl;

            //    cout << "    path_map.count(i): " << path_map.count(i) << ", path_map.at(i): " << path_map.at(i) << ", i: " << i << ", node.first: " << node.first << endl;

            // If conditions met: this edge is part of the shortest path (color it)
            if(path_map.count(i) > 0 && path_map.at(i) == node.first) {
                ofile << "    " << i << " -> " << node.first << "[label=\"" << node.second << "\" color=blue]" << endl;
            }
            // Else it is a regular edge
            else {
                ofile << "    " << i << " -> " << node.first << "[label=\"" << node.second << '"' << ']' << endl;
            }

        }
    }
}