#include "graphviz.h"

#include <iostream>
#include <fstream>

using namespace std;

// Constructor
Graphviz::Graphviz(std::vector<std::vector<std::pair<int, double>>> a) {
    cout << "Graphviz constructor" << endl;
    setAdjacencyList(a);
}

Graphviz::Graphviz() {
    cout << "~GRAPHVIZ~" << endl;
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
            ofile << "    " << i << " -> " << node.first << "[label=\"" << node.second << '"' << ']' << endl;
        }
    }
}