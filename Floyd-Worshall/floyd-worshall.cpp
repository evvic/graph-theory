#include "floyd-worshall.h"
#include <vector>
#include <limits> // for initializing to positive infinity
#include <iostream>

// Vars for checking bounds
const static double POSITIVE_INFINITY = numeric_limits<double>::max();
const static double NEGATIVE_INFINITY = numeric_limits<double>::min();

using namespace std;

// N = size of the adjacency matrix
// dp = the memo table that will contain APSP solution (shortest path)
// next = matrix used to reeconstruct shortest path

FloydWorshall::FloydWorshall(vector<vector<double>> matrix) {
    n = matrix.size();
}

// m = adjacency matrix
vector<double> FloydWorshall::floydworshall() {

}

FloydWorshall::setup() {

}


