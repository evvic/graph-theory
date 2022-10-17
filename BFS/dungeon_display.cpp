#include <iostream>
#include <time.h>
#include "dungeon_display.h"

using namespace std;

DungeonDisplay::DungeonDisplay(int r, int c) : BFS(r, c) {
    //setDungeonSize(r, c);
    //setMatrix(vector<vector<char>>(d_cols, vector<char>(d_rows, ' ')));
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





void DungeonDisplay::addPath(vector<pair<int, int>> path) {
    for(auto step : path) {
        if (step == exit) continue;
        if (step == start) continue;
        addStep(step);
    }
}

void DungeonDisplay::addStep(pair<int, int> wall) {
    addStep(wall.first, wall.second);
}

bool DungeonDisplay::addStep(int r, int c) {
    // check wall is in bouds
    if(r < 0 || r > d_rows) {
        cout << "Step row pos is out of bounds." << endl;
        return false;
    }
    if(c < 0 || c > d_cols) {
        cout << "Step col pos is out of bounds." << endl;
        return false;
    }
    // check if it covers start tile
    if(r == start.first && c == start.second) {
        cout << "Placing step covers Start." << endl;
        return false;
    }
    // check if it covers start tile
    if(isExit(r, c)) {
        cout << "Placing step covers Exit." << endl;
        return false;
    }

    // now add new start coord to matrix
    matrix[r][c] = '*';
    return true;
}

void DungeonDisplay::addWall(pair<int, int> wall) {
    addWall(wall.first, wall.second);
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


void DungeonDisplay::printDisplay() {
    for(int i = 0; i < d_cols + 2; i++) cout << '#';
    for(int i = 0; i < d_rows; i++) {
        cout << endl << '#';
        for(int j = 0; j < d_cols; j++) {
            cout << matrix[i][j];
        }
        cout << '#';
    }
    cout << endl;
    for(int i = 0; i < d_cols + 2; i++) cout << '#';
    cout << endl;
}

vector<pair<int, int>> DungeonDisplay::reconstruct_path() {
    // Reconstruct path going backwards from E
    vector<pair<int, int>> path;

    cout << "Reconstructing path..." << endl;

    // init curr node to last vector element
    node* curr = find_node(prev.back()->data);
    // if reached end, init to exit node
    if(reached_end) {
        curr = find_node(exit);
    }


    //cout << "Exit node starting point: (" << curr->data.first << ',' << curr->data.second << ')' << endl;

    while (curr->parent != NULL) {
        //cout << "curr node: " << curr->data.first << ", " << curr->data.second << endl;
        path.push_back(curr->data);
        curr = curr->parent;
    }

    return path;
}

void DungeonDisplay::generateWalls(int percent) {
    if(percent < 0 || percent > 100) {
        cout << "Percent range must be between 0 to 100" << endl;
        return;
    }

    int weight = 7; // out of 10

    int num_walls = (int)(((d_cols * d_rows * percent * weight)/ 100) / 10);
    cout << "Generating " << num_walls << " walls in the dungeon." << endl;

    // figure out how to randomly generate the positions to add the walls

    srand (time(NULL));

    for(int i = 0; i < num_walls; i++) {
        int rr = rand() % d_rows;
        int cc = rand() % d_cols;

        addWall(rr, cc);
    }


}