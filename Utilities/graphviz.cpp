#include "graphviz.h"

#include <iostream>
#include <fstream>
#include <map>

using namespace std;

// Constructor

Graphviz::Graphviz() {
    cout << "~GRAPHVIZ~" << endl;
    path = {};
    path_map = {};
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

    // Extract just the name of file from file path
    //int found = fname.find_last_of('/');
    //cout << " found: " << found << endl;
    string file_name = fname.substr(fname.find_last_of('/')+1);

    // Include file path directory
    // !!!! ^
    ofile.open (fname + ".gv");

    // Write initial bracket opener
    ofile << "digraph " << file_name << " {\n";

    // Write the whole Adjacency List to the file
    writeDigraph(ofile);

    // Write closer bracket
    ofile << endl << "}";

    ofile.close();

    cout << "Created digraph " << file_name << " in " << (fname + ".gv") << endl;
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

// Render a created .gv file given file name
// Include path-directory from to pwd to /graphs in fname
void Graphviz::render(std::string fname) {
    string cmd = "dot -Tpng " +  fname + ".gv -o " + fname + ".png";

    pclose(popen(cmd.c_str(), "w"));

    cout << "Rendered graph to " << fname << ".png" << endl;
}