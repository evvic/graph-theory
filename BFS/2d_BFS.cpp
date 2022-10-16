#include <iostream>
#include "2d_BFS.h"

using namespace std;

BFS::BFS(int r, int c) {
    setArea(r, c);
    // init matrix with all ' '
    setMatrix(vector<vector<char>>(d_cols, vector<char>(d_rows, ' ')));
    // init have visited matrix all false
    initVisited();
}

void BFS::setMatrix(vector<vector<char>> m) {
    matrix = m;
}

void BFS::initVisited() {
    setVisited(vector<vector<bool>>(d_cols, vector<bool> (d_rows, false)));
}

void BFS::setVisited(vector<vector<bool>> v) {
    visited = v;
}