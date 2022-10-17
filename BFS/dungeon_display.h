#include <iostream>
#include <vector>
#include "2d_BFS.h"

using namespace std;

class DungeonDisplay : public BFS {
private:


public:
    DungeonDisplay(int r, int c);
    void setDungeonSize(int r, int c);
    void setDungeonExit(int r, int c);
    void setDungeonStart(int r, int c);
    //void setMatrix(vector<vector<char>> m);

    void addPath(vector<pair<int, int>>);
    void addStep(pair<int, int> wall);
    bool addStep(int r, int c);
    void addWall(pair<int, int> wall);
    bool addWall(int r, int c);
    void generateWalls(int percent);

    // COMPUTATIONAL
    vector<pair<int, int>> reconstruct_path();

    void printDisplay();
};