#include <iostream>
#include <queue>
#include "dungeon_display.h"

using namespace std;



/*********************
 * TO-DO
 * FIX THE NODES AND MAKE THE PATH BACKWARDS ACTUALLY WORK
 * ALSO ADD PATH FROM START TO EXIT TO MATRIX WITH '#'
*/

// GLOBAL VARIABLES


// functions
int solve(DungeonDisplay display);
vector<node*> explore_neighbors(int r, int c, vector<node*> prev, DungeonDisplay display);
vector<pair<int, int>> reconstruct_path(pair<int, int> e, vector<node*> prev);
//node find_node(pair<int, int> rc, vector<node> prev);
int find_node(pair<int, int> coord, vector<node*> prev);
void draw_map();
void add_coord_to_matrix(pair<int, int> p, char c);
void add_coord_to_matrix(int pr, int pc, char c);
void print_path(vector<pair<int, int>> p);

int main() {

    int R = 5, C = 5;               // init Row & Column size
    int sr = 0, sc = 0;             // init start coordinates
    int er = 3, ec = 3;             // init where the Exit spot is

    cout << "In BFS dungeon main" << endl;

    // create dungeon object: init with size of dungeon
    DungeonDisplay display(R, C);
    //display.setDungeonSize(R, C);
    display.setDungeonExit(er, ec);
    display.setDungeonStart(sr, sc);

    // TODO: init walls
    // walls/rocks will be '#'



    int moves = display.solve();
    // RECONSTRUCT PATH
    vector<pair<int, int>> path = display.reconstruct_path();

    print_path(path);
    // ADD PATH TO MATRIX m
    display.addPath(path);
    display.printDisplay();

    if (moves >= 0) {
        cout << "It took " << moves << " moves to leave the dungeon." << endl;
    }
    else {
        cout << "Could not find a path out of the dungeon." << endl;
    }

    return 1;
}

void print_path(vector<pair<int, int>> p) {
    cout << "path: ";
    for( auto step : p) {
        cout << "(" << step.first << ',' << step.second << ") ";
    }
    cout << endl;
}