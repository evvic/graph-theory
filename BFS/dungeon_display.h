#include <iostream>
#include <vector>

using namespace std;

class DungeonDisplay {
private:
    int d_rows;
    int d_cols;
    pair<int, int> exit;
    pair<int, int> start;
    vector<vector<char>> matrix; // init empty char maatrix (map)


public:
    DungeonDisplay(int r, int c);
    void setDungeonSize(int r, int c);
    void setDungeonExit(int r, int c);
    void setDungeonStart(int r, int c);
    void setMatrix(vector<vector<char>> m);

    pair<int, int> getExit();
    pair<int, int> getStart();


    bool isExit(int r, int c);

    bool addWall(int r, int c);

    void printDisplay();
};