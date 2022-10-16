#include <iostream>
#include <vector>
#include "2d_BFS.h"

using namespace std;

class DungeonDisplay : public BFS {
private:

    pair<int, int> exit;
    pair<int, int> start;

public:
    DungeonDisplay(int r, int c);
    void setDungeonSize(int r, int c);
    void setDungeonExit(int r, int c);
    void setDungeonStart(int r, int c);
    //void setMatrix(vector<vector<char>> m);

    pair<int, int> getExit();
    pair<int, int> getStart();


    bool isExit(int r, int c);

    bool addWall(int r, int c);

    void printDisplay();
};