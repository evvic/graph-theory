#include <iostream>
#include <queue>
#include "dungeon_display.h"

using namespace std;

struct node {
    node* parent = NULL;
    pair<int, int> data;
};

// GLOBAL VARIABLES


int sr = 0, sc = 0;             // init start coordinates
queue<int> rq = {}, cq = {};    // init Row & Column queues

// Variables used to track the number of steps taken
int move_count = 0;             // init
int nodes_left_in_layer = 1;    // track curret amount of nodes left
int nodes_in_next_layer = 0;    // count amount of nodes for next layer

// Var used to track if end cell 'E' is ever reached
bool reached_end = false;



// DIRECTIONS
const int MOVE_DIRECTIONS = 4; // North, South, East, West
// North, south, east, west direction arrays
int dr[MOVE_DIRECTIONS] = { -1, 1, 0, 0 };
int dc[MOVE_DIRECTIONS] = { 0, 0, 1, -1 };



int solve(DungeonDisplay display);
vector<node*> explore_neighbors(int r, int c, vector<node*> prev);
vector<pair<int, int>> reconstruct_path(pair<int, int> e, vector<node*> prev);
//node find_node(pair<int, int> rc, vector<node> prev);
int find_node(pair<int, int> coord, vector<node*> prev);
void draw_map();
void add_coord_to_matrix(pair<int, int> p, char c);
void add_coord_to_matrix(int pr, int pc, char c);

int main() {

    int R = 5, C = 5;               // init Row & Column size
    vector<vector<char>> m(C, vector<char>(R, ' '));  // init empty char maatrix (map)

    cout << "In BFS dungeon main" << endl;

    // init where the Exit spot is
    int er = 3, ec = 3;
    m[er][ec] = 'E';

    DungeonDisplay display(R, C);
    //display.setDungeonSize(R, C);
    display.setDungeonExit(er, ec);



    // TODO: init walls
    // walls/rocks will be '#'

    // pass prev by reference

    int moves = solve(display);

    if (moves >= 0) {
        cout << "It took " << moves << " moves to leave the dungeon." << endl;
    }
    else {
        cout << "Could not find a path out of the dungeon." << endl;
    }

    return 1;
}

int solve(DungeonDisplay display) {
    // add star coordinates to the queues
    rq.push(sr);
    cq.push(sc);

    visited[sr][sc] = true;

    // INIT NODE LIST
    node* head = new node;
    head->data = pair<int, int>(sr, sc);
    head->parent = NULL;
    // tracks parent node to recreate path (R,C)
    vector<node*> prev = {head};

    while(rq.size() > 0 || cq.size() > 0) {
        int r = rq.front(); rq.pop();
        int c = cq.front(); cq.pop();

        if(display.isExit(r, c)) {
            reached_end = true;
            cout << "actual E: " << r << ", " << c << endl;
            break;
        }

        prev = explore_neighbors(r, c, prev);
        nodes_left_in_layer--;

        if(nodes_left_in_layer == 0) {
            nodes_left_in_layer = nodes_in_next_layer;
            nodes_in_next_layer = 0;
            move_count++;
        }
    }

    if(reached_end) {

        cout << "E: " << prev.back()->data.first << ", " <<  prev.back()->data.second << endl;
        // RECONSTRUCT PATH
        vector<pair<int, int>> path = reconstruct_path(display.getExit(), prev);
        // ADD PATH TO MATRIX m

        display.printDisplay();

        return move_count;
    }

    return -1;
}

vector<node*> explore_neighbors(int r, int c, vector<node*> prev) {

    // do a math thing here to figure out parent node
    node* parent = prev.at(prev.size() - nodes_left_in_layer);

    for(int i = 0; i < MOVE_DIRECTIONS; i++) {
        // get new R & C coordinates for each potential 4 directions
        int rr = r + dr[i];
        int cc = c + dc[i];

        // Skip out of bounds locations
        if( rr < 0 || cc < 0) continue;
        if( rr >= R || cc >= C) continue;

        // Skip visited locations or blocked cells
        if( visited[rr][cc]) continue;
        if( m[rr][cc] == '#') continue;

        rq.push(rr);
        cq.push(cc);

        // Create child node
        node* child = new node;
        child->data = pair<int, int>(rr, cc);
        child->parent = parent;

        prev.push_back(child);

        visited[rr][cc] = true;
        nodes_in_next_layer++;
    }

    return prev;
}

vector<pair<int, int>> reconstruct_path(pair<int, int> e, vector<node*> prev) {
    // Reconstruct path going backwards from E
    vector<pair<int, int>> path;

    int index = find_node(e, prev);
    node* curr = prev.at(index);


    while (curr->parent != NULL) {
        cout << "curr node: " << curr->data.first << ", " << curr->data.second << endl;
        path.push_back(curr->data);
        curr = curr->parent;
    }

    return path;
}

int find_node(pair<int, int> coord, vector<node*> prev) {
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

void add_coord_to_matrix(pair<int, int> p, char c) {
    // replace the char in matrix m with given char at position p
    add_coord_to_matrix(p.first, p.second, c);
}

void add_coord_to_matrix(int pr, int pc, char c) {
    //m[pr][pc] = c;
}