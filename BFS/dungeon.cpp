#include <iostream>
#include <queue>
#include "dungeon_display.h"

using namespace std;

void print_path(vector<pair<int, int>> p);

int main()
{
    // INITIAL VARIABLES
    int R = 15, C = 30;   // init Row & Column size
    int sr = 2, sc = 2;   // init start coordinates
    int er = 12, ec = 19; // init where the Exit spot is

    cout << "In BFS dungeon main" << endl;

    // create dungeon object: init with size of dungeon
    DungeonDisplay display(R, C);
    // display.setDungeonSize(R, C);
    display.setDungeonExit(er, ec);
    display.setDungeonStart(sr, sc);

    // percentage (0-100)
    display.generateWalls(70);

    display.printDisplay();

    int moves = display.solve();
    // RECONSTRUCT PATH
    vector<pair<int, int>> path = display.reconstruct_path();

    // ADD PATH TO MATRIX m
    display.addPath(path);
    display.printDisplay();

    if (moves >= 0)
    {
        cout << "It took " << moves << " moves to leave the dungeon." << endl;
    }
    else
    {
        cout << "Could not find a path out of the dungeon." << endl;
    }

    return 1;
}

void print_path(vector<pair<int, int>> p)
{
    cout << "path: ";
    for (auto step : p)
    {
        cout << "(" << step.first << ',' << step.second << ") ";
    }
    cout << endl;
}