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
    dist = {};
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

void Graphviz::mapDistFromStart() {
    //
}

// Any "node" set to negative infinity is in a negative cycle
void Graphviz::setDistFromStart(std::vector<double> d) {
    dist = d;
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

    // Prestyle any individual nodes
    nodePreStyling(ofile);

    // Write the whole Adjacency List to the file
    writeDigraph(ofile);

    // Write closer bracket
    ofile << endl << "}";

    ofile.close();

    cout << "Created digraph " << file_name << " in " << (fname + ".gv") << endl;
}

void Graphviz::nodePreStyling(std::ofstream& ofile) {

    ofile << "\n    layout=\"neato\";";
    ofile << "\n    overlap=\"false\";";
    ofile << "\n    sep=\"+20\";";
    ofile << "\n    splines=true;" << endl;

    ofile << "\n    // Prestyling of nodes";
    ofile << "\n    //// Red = node in negative cycle" << endl;

    // Loop through each node and compare
    // Add any special stylings when necessary
    for (int i = 0; i < al.size(); i++) {
        string extras = "";

        // If start/end node of path, add outline
        if (i == start_node || i == end_node) {
            extras += "color=orange ";
        }

        // Check for NEGATIVE INFINITY (represents negative cycle)
        if (dist.size() > i && dist.at(i) == NEGATIVE_INFINITY) {
            // Make node red
            extras += "style=filled, fillcolor=red ";
        }

        // Apply stylings to node when necessary
        if (extras.length() > 0) {
            ofile << "\n    " << i << (" [" + extras + "]");
        }

    }

    ofile << endl;
}

// Directional graph
void Graphviz::writeDigraph(ofstream& ofile) {

    for (int i = 0; i < al.size(); i++) {
        // Write the basic initial for each node
        ofile << endl << "    // node " << i << endl;

        for (auto node : al.at(i)) {

            // String for appending additional styling when necessary
            string extras = "";

            // Include a min length for the edge (arrow)
            extras += " minlen=2";

            // CHECK WITH IF STATEMENT USING MAP_PATH
            // IF  map.contains(i) && map.at(i) == node.first:
            //     THEN MAKE STYLE RED
            // Path is in ordewr (reversed)

            // If conditions met: this edge is part of the shortest path (color it)
            if(path_map.count(i) > 0 && path_map.at(i) == node.first) {
                extras += " color=blue";
            }

            // If this edge is in an infinite cycle (both connecting nodes are neg inf), color it
            if (dist.size() > i && dist.at(i) == NEGATIVE_INFINITY ) {
                if(dist.size() > node.first && dist.at(node.first) == NEGATIVE_INFINITY)
                    extras += " color=red";
            }

            // Write line for edge to file
            ofile << "    " << i << " -> " << node.first << "[label=\"" << node.second << '"' << extras << ']' << endl;

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

vector<vector<pair<int, double>>> Graphviz::convertMatrixToList(vector<vector<double>> m) {
    // adjList[i][j].second is the distance from i to j
    // adjMatrix[i][j] is the distance frim i to j

    // Init 1st dimension of list to num of nodes
    vector<vector<pair<int, double>>> al(m.size(), vector<pair<int, double>>());

    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m.at(i).size(); j++) {
            // If value [i][j] == NEGATIVE_INFINITY: EDGE IN NEGATIVE CYCLE
            // If value [i][j] == POSITIVE_INFINITY: nodes do not connect
            if (m.at(i).at(j) == POSITIVE_INFINITY) continue;

            al.at(i).push_back(make_pair(j, m.at(i).at(j)));
        }
    }

    for (int i = 0; i < al.size(); i++) {
        for (auto el : al.at(i)) {
            cout << i << " -> " << el.first <<  ": " << el.second << endl;
        }
    }

    return al;
}

vector<vector<double>> Graphviz::convertListToMatrix(vector<vector<pair<int, double>>> al) {
    // adjList[i][j].second is the distance from i to j
    // adjMatrix[i][j] is the distance frim i to j

    // Init both dimensions of matrix size to num of nodes (POSITIVE_INFIITY default)
    vector<vector<double>> m(al.size(), vector<double> (al.size(), numeric_limits<double>::max()));

    for (int i = 0; i < al.size(); i++) {
        for (auto element : al.at(i)) {
            // If value [i][j] == NEGATIVE_INFINITY: EDGE IN NEGATIVE CYCLE

            m.at(i).at(element.first) = element.second;
        }
    }

    return m;
}
