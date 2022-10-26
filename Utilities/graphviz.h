#ifndef GRAPHVIZ
#define GRAPHVIZ

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>


//template <class T1>
class Graphviz {
private:
    int start_node;
    int end_node;

    // Path of nodes from start to end (optional)
    // & Map of path: first = init node, second = destination node
    std::vector<int> path;
    std::map<int, int> path_map;

    // Vect of nodes with their shortest dist to start node
    // & Map of nodes with their dist
    std::vector<double> dist;
    std::map<int, double> dist_map;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // INIT AND IMPLEMENT THE DIST AND DIST_MAP STRUCTURES
    // ALSO THEIR SETTER FUNCTIONS
    // THEN IMPLEMENT FOR WRITING

    // Adjacency list
    std::vector<std::vector<std::pair<int, double>>> al;

    // Writing member function for a
    void writeDigraph(std::ofstream& ofile);
    // Writing member function for prestyling nodes
    void nodePreStyling(std::ofstream& ofile);

    // Take path and mappify it
    void mapPath();

    // Take negCycle path and mappify
    void mapDistFromStart();

public:
    Graphviz();

    // Pass AL to set it locally in class
    void setAdjacencyList(std::vector<std::vector<std::pair<int, double>>> a);

    // Start & End node inclusive
    void setPath(std::vector<int> p);
    // Nodes in negative cycles have value NEGATIVE INFINITY
    void setDistFromStart(std::vector<double> d);

    // Write the Adjacency List to a .gv file properly formatted
    // Include path-directory from to pwd to /graphs in fname
    void write(std::string fname);
    // Render a created .gv file given file name
    // Include path-directory from to pwd to /graphs in fname
    void render(std::string fname);
};

#endif