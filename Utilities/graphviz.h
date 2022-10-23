#ifndef GRAPHVIZ
#define GRAPHVIZ

#include <vector>
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

    // Adjacency list
    std::vector<std::vector<std::pair<int, double>>> al;

    // Writing member function for a
    void writeDigraph(std::ofstream& ofile);

public:
    Graphviz(std::vector<std::vector<std::pair<int, double>>> a);
    Graphviz();

    void setAdjacencyList(std::vector<std::vector<std::pair<int, double>>> a);

    // Write the Adjacency List to a .gv file properly formatted
    void write(std::string fname);


};

#endif