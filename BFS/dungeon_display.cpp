#include <iostream>
#include "dungeon_display.h"
#include "2d_BFS.h"

using namespace std;

DungeonDisplay::DungeonDisplay(int r, int c) : BFS(r, c) {
    setDungeonSize(r, c);
    setMatrix(vector<vector<char>>(d_cols, vector<char>(d_rows, ' ')));
}

void DungeonDisplay::setDungeonSize(int r, int c) {
    // extra: add check to make sure it fits in terminal window
    d_rows = r;
    d_cols = c;
}

void DungeonDisplay::setDungeonExit(int r, int c) {
    if(r < 0 || r > d_rows) {
        cout << "Exit row pos is out of bounds." << endl;
        return;
    }
    if(c < 0 || c > d_cols) {
        cout << "Exit col pos is out of bounds." << endl;
        return;
    }
    // check if an exit already existed
    // & if exit aleady exists, remove previous from matrix
    exit = pair<int, int>(r, c);
    // now add new exit coord to matrix
    matrix[r][c] = 'E';
}

void DungeonDisplay::setDungeonStart(int r, int c) {
    if(r < 0 || r > d_rows) {
        cout << "Start row pos is out of bounds." << endl;
        return;
    }
    if(c < 0 || c > d_cols) {
        cout << "Start col pos is out of bounds." << endl;
        return;
    }
    // check if a start already existed
    // & if start aleady exists, remove previous from matrix
    start = pair<int, int>(r, c);
    // now add new start coord to matrix
    matrix[r][c] = 'S';
}



pair<int, int> DungeonDisplay::getExit() {
    return exit;
}

pair<int, int> DungeonDisplay::getStart() {
    return start;
}

bool DungeonDisplay::addWall(int r, int c) {
    // check wall is in bouds
    if(r < 0 || r > d_rows) {
        cout << "Wall row pos is out of bounds." << endl;
        return false;
    }
    if(c < 0 || c > d_cols) {
        cout << "Wall col pos is out of bounds." << endl;
        return false;
    }
    // check if it covers start tile
    if(r == start.first && c == start.second) {
        cout << "Placing wall covers Start." << endl;
        return false;
    }
    // check if it covers start tile
    if(isExit(r, c)) {
        cout << "Placing wall covers Exit." << endl;
        return false;
    }

    // now add new start coord to matrix
    matrix[r][c] = '#';
    return true;
}

bool DungeonDisplay::isExit(int r, int c) {
    if(exit.first == r && exit.second == c) return true;
    else return false;
}

void DungeonDisplay::printDisplay() {
    for(int i = 0; i < d_rows + 2; i++) cout << '#';
    for(int i = 0; i < d_cols; i++) {
        cout << endl << '#';
        for(int j = 0; j < d_rows; j++) {
            cout << matrix[i][j];
        }
        cout << '#';
    }
    cout << endl;
    for(int i = 0; i < d_rows + 2; i++) cout << '#';
    cout << endl;
}