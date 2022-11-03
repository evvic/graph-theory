#include <iostream>
#include <vector>
#include <limits>
#include "floyd-worshall.h"
//#include "../Utilities/graphviz.h"

using namespace std;


// FUNCTION DECLARATIONS
vector<vector<double>> createGraph(int n);



int main() {
    //// CONSTRUCTING MATRIX
    int n = 7;
    vector<vector<double>> m = createGraph(n);

    // Add some edge values
    m[0][1] = 2;
    m[0][2] = 5;
    m[0][6] = 10;
    m[1][2] = 2;
    m[1][4] = 11;
    m[2][6] = 2;
    m[6][5] = 11;
    m[4][5] = 1;
    m[5][4] = -2;


    // FLOYD WORSHALL CLASS SHOULD BE COMPLETE
    // FINISH MAIN FUNCTION AND OTHER FUNCTIONS USED HERE
    FloydWorshall solver(m);
    vector<vector<double>> dist = solver.getApspMatrix();


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "The shortest path from node " << i << " to node " << j << " is " << dist[i][j] << endl;
        }
    }


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