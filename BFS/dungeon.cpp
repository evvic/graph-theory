#include <iostream>
#include <queue>

using namespace std;

struct node {
    node* parent;
    pair<int, int> data;
};

// GLOBAL VARIABLES
int R = 5, C = 5;               // init Row & Column size
vector<vector<char>> m(C, vector<char>(R, ' '));  // init empty char maatrix (map)
int sr = 0, sc = 0;             // init start coordinates
queue<int> rq = {}, cq = {};    // init Row & Column queues

// Variables used to track the number of steps taken
int move_count = 0;             // init
int nodes_left_in_layer = 1;    // track curret amount of nodes left
int nodes_in_next_layer = 0;    // count amount of nodes for next layer

// Var used to track if end cell 'E' is ever reached
bool reached_end = false;

// RxC matrix of false values to track if node has visited
vector<vector<bool>> visited(C, vector<bool> (R, false));

// DIRECTIONS
const int MOVE_DIRECTIONS = 4; // North, South, East, West
// North, south, east, west direction arrays
int dr[MOVE_DIRECTIONS] = { -1, 1, 0, 0 };
int dc[MOVE_DIRECTIONS] = { 0, 0, 1, -1 };



int solve();
vector<node> explore_neighbors(int r, int c);
void reconstruct_path(pair<int, int> s, pair<int, int> e, vector<node> prev);

int main() {

    cout << "In BFS dungeon main" << endl;

    // init where the Exit spot it=s
    m[3][3] = 'E';

    // TODO: init walls
    // walls/rocks will be '#'

    // pass prev by reference

    int moves = solve();

    if (moves >= 0) {
        cout << "It took " << moves << " moves to leave the dungeon." << endl;
    }
    else {
        cout << "Could not find a path out of the dungeon." << endl;
    }

    return 1;
}

int solve() {
    // add star coordinates to the queues
    rq.push(sr);
    cq.push(sc);

    visited[sr][sc] = true;

    // INIT NODE LIST
    node head;
    head.data = pair<int, int>(sr, sc);
    // tracks parent node to recreate path (R,C)
    vector<node> prev = {head};

    while(rq.size() > 0 || cq.size() > 0) {
        int r = rq.front(); rq.pop();
        int c = cq.front(); cq.pop();

        if(m[r][c] == 'E') {
            reached_end = true;
            break;
        }

        prev.push_back(explore_neighbors(r, c, prev.back()));
        nodes_left_in_layer--;

        if(nodes_left_in_layer == 0) {
            nodes_left_in_layer = nodes_in_next_layer;
            nodes_in_next_layer = 0;
            move_count++;
        }
    }

    if(reached_end) {
        return move_count;
    }

    return -1;
}

vector<node> explore_neighbors(int r, int c, node parent) {
    vector<node> ret;

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

        node child;

        visited[rr][cc] = true;
        nodes_in_next_layer++;
    }

    if(ret.size() == 0) {
        return null;
    }
    else return ret;
}

void reconstruct_path(pair<int, int> s, pair<int, int> e, vector<node> prev) {
    // Reconstruct path going backwards from E
    vector<pair<int, int>> path;

    for(pair<int, int> at = e; at != null; at = prev[at]) {

    }
}