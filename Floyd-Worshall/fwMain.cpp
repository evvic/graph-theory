#include <iostream>
#include <vector>
#include <limits>
#include "floyd-worshall.h"
#include "../utilities/graphviz.h"

using namespace std;


// FUNCTION DECLARATIONS
vector<vector<double>> createGraph(int n);



int main() {
    //// CONSTRUCTING MATRIX
    int n = 7;
    vector<vector<double>> m = createGraph(n);

    // Add some edge values
    m[0][1] = 2.0;
    m[0][2] = 5.0;
    m[0][6] = 10.0;
    m[1][2] = 2.0;
    m[1][4] = 11.0;
    m[2][6] = 2.0;
    m[6][5] = 11.0;
    m[4][5] = 1.0;
    m[5][4] = -2.0;


    // FLOYD WORSHALL CLASS SHOULD BE COMPLETE
    // FINISH MAIN FUNCTION AND OTHER FUNCTIONS USED HERE
    FloydWorshall solver(m);
    vector<vector<double>> dist = solver.getApspMatrix();

    // Init Graphviz class (for making .gv of AL's)
    Graphviz gviz{};


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "The shortest path from node " << i << " to node " << j << " is " << dist[i][j] << endl;
        }
    }

    gviz.setAdjacencyList(gviz.convertMatrixToList(m));
    // Pass file name + directory
    gviz.write("graphs/fwtest");
    gviz.render("graphs/fwtest");


}

/// @brief Inits a matrix of size nxn
/// @param num n to represent num of nodes
/// @return initialized adjacency matrix
vector<vector<double>> createGraph(int n) {
    // Init all matrix member to positive infinity
    cout << "createGraph" << endl;
    vector<vector<double>> matrix(n, vector<double> (n, numeric_limits<double>::max()));

    cout << "    createGraph matrix size " << matrix.size() << endl;

    // Init all diagnal nodes to 0
    for (int i = 0; i < n; i++) {
        matrix.at(i).at(i) == 0;
    }

    cout << "createGraph end" << endl;

    return matrix;
}