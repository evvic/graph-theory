#include <iostream>
#include "2d_BFS.h"

using namespace std;

BFS::BFS(int r, int c) {
    setArea(r, c);
    // init matrix with all ' '
    setMatrix(vector<vector<char>>(d_cols, vector<char>(d_rows, ' ')));
    // init have visited matrix all false
    initVisited();

    // init queues
    rq = {};
    cq = {};

    prev = {};
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

pair<int, int> BFS::getExit() {
    return exit;
}

pair<int, int> BFS::getStart() {
    return start;
}

bool BFS::isExit(int r, int c) {
    if(exit.first == r && exit.second == c) return true;
    else return false;
}

bool BFS::hasWall(int r, int c) {
    // do a bounds checking
    // check wall is in bouds
    if(r < 0 || r > d_rows) {
        cout << "Wall row pos is out of bounds." << endl;
        return true;
    }
    if(c < 0 || c > d_cols) {
        cout << "Wall col pos is out of bounds." << endl;
        return true;
    }

    if(matrix[r][c] == '#') return true;
    else return false;
}

// COMPUTATIONAL
int BFS::solve() {
    // add star coordinates to the queues
    rq.push(start.first);
    cq.push(start.second);

    setVisited(start);

    // INIT NODE LIST
    node* head = new node;
    head->data = start;
    head->parent = NULL;
    // tracks parent node to recreate path (R,C)
    prev = {head};

    //cout << prev.size() << endl;

    cout << "HEAD NODE: " << prev.at(0)->data.first << ", " << prev.at(0)->data.second << endl;

    while(rq.size() > 0 || cq.size() > 0) {
        int r = rq.front(); rq.pop();
        int c = cq.front(); cq.pop();

        if(isExit(r, c)) {
            reached_end = true;
            cout << "actual E: " << r << ", " << c << endl;
            break;
        }

        cout << " looping with " << r << ", " << c << endl;

        cout << "  prev size before neighbors: " << prev.size() << endl;
        explore_neighbors(r, c);
        nodes_left_in_layer--;

        if(nodes_left_in_layer == 0) {
            nodes_left_in_layer = nodes_in_next_layer;
            nodes_in_next_layer = 0;
            move_count++;
        }
    }

    if(reached_end) {
        cout << "E: " << prev.back()->data.first << ", " <<  prev.back()->data.second << endl;
        return move_count;
    }

    return -1;
}

void BFS::explore_neighbors(int r, int c) {
    // do a math thing here to figure out parent node
    cout << "  BFS::explore_neighbors. prev size: " << prev.size() << endl;
    node* parent = prev.at(prev.size() - nodes_left_in_layer);

    for(int i = 0; i < MOVE_DIRECTIONS; i++) {
        // get new R & C coordinates for each potential 4 directions
        int rr = r + dr[i];
        int cc = c + dc[i];

        // Skip out of bounds locations
        if( rr < 0 || cc < 0) continue;
        if( rr >= getRowLength() || cc >= getColLength()) continue;

        // Skip visited locations or blocked cells
        if(visitedCell(rr, cc)) continue;
        if(hasWall(rr, cc)) continue;

        rq.push(rr);
        cq.push(cc);

        // Create child node
        node* child = new node;
        child->data = pair<int, int>(rr, cc);
        child->parent = parent;

        prev.push_back(child);

        setVisited(rr, cc);
        nodes_in_next_layer++;
    }
}

int BFS::find_node(pair<int, int> coord) {
    cout << "find node" <<endl;

    for(int i = prev.size() -1; prev.at(i)->parent != NULL && i > 0; --i) {
        if( prev.at(i)->data.first == coord.first && prev.at(i)->data.second == coord.second) {
            cout << "found node: " << prev.at(i)->data.first << ", " << prev.at(i)->data.second << " at: " << i <<endl;
            return i;
        }
    }

    cout << "find node done" <<endl;

    return -1;
}