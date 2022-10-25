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
    std::vector<int> path;
    // Map of path: first = init node, second = destination node
    std::map<int, int> path_map;

    // Adjacency list
    std::vector<std::vector<std::pair<int, double>>> al;

    // Writing member function for a
    void writeDigraph(std::ofstream& ofile);

    // Take path and mappify it
    void mapPath();

public:
    Graphviz(std::vector<std::vector<std::pair<int, double>>> a);
    Graphviz();

    // Start & End node inclusive
    void setPath(std::vector<int> p);
    // Pass AL to set it locally in class
    void setAdjacencyList(std::vector<std::vector<std::pair<int, double>>> a);

    // Write the Adjacency List to a .gv file properly formatted
    // Include path-directory from to pwd to /graphs
    void write(std::string fname);
    // Render a created .gv file given file name
    // Include path-directory from to pwd to /graphs
    void render(std::string fname);

};

#endif