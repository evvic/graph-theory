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

void BFS::setArea(int r, int c) {
    d_rows = r;
    d_cols = c;
}

bool BFS::visitedCell(int r, int c) {
    return visited[r][c];
}

void BFS::setMatrix(vector<vector<char>> m) {
    matrix = m;
}

void BFS::initVisited() {
    copyVisited(vector<vector<bool>>(d_cols, vector<bool> (d_rows, false)));
}

void BFS::copyVisited(vector<vector<bool>> v) {
    visited = v;
}

void BFS::setVisited(pair<int, int> coord) {
    setVisited(coord.first, coord.second);
}

void BFS::setVisited(int r, int c) {
    visited[r][c] = true;
}

int BFS::getRowLength() {
    return d_rows;
}

int BFS::getColLength() {
    return d_cols;
}