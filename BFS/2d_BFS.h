#include <iostream>
#include <vector>

using namespace std;

class BFS {
protected:
    // vars mst be protected so it is easier to inherit
    int d_rows;
    int d_cols;
    vector<vector<char>> matrix; // init empty char maatrix (map)
    // RxC matrix of false values to track if node has visited
    vector<vector<bool>> visited;

public:
    BFS(int r, int c);
    void setMatrix(vector<vector<char>> m);
    void setArea(int r, int c);
    void initVisited();
    void setVisited(vector<vector<bool>> v);


};