#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct node {
    node* parent = NULL;
    pair<int, int> data;
};

class BFS {
protected:
    // vars mst be protected so it is easier to inherit
    int d_rows;
    int d_cols;
    vector<vector<char>> matrix; // init empty char maatrix (map)
    // RxC matrix of false values to track if node has visited
    vector<vector<bool>> visited;
    // save path of all nodes branching
    vector<node*> prev;

    queue<int> rq, cq;    // init Row & Column queues

    pair<int, int> start;
    pair<int, int> exit;

    // Variables used to track the number of steps taken
    int move_count = 0;             // init
    int nodes_left_in_layer = 1;    // track curret amount of nodes left
    int nodes_in_next_layer = 0;    // count amount of nodes for next layer

    // Var used to track if end cell 'E' is ever reached
    bool reached_end = false;

    // DIRECTIONS
    const int MOVE_DIRECTIONS = 4; // North, South, East, West
    // North, south, east, west direction arrays
    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, 1, -1 };

    // FUNCTIONS
    void explore_neighbors(int r, int c);


public:
    BFS(int r, int c);
    void setMatrix(vector<vector<char>> m);
    void setArea(int r, int c);
    void initVisited();
    void copyVisited(vector<vector<bool>> v);
    void setVisited(pair<int, int> coord);
    void setVisited(int r, int c);

    int getRowLength();
    int getColLength();
    bool hasWall(int r, int c);


    pair<int, int> getExit();
    pair<int, int> getStart();

    bool isExit(int r, int c);

    bool visitedCell(int r, int c);

    //computational
    int solve();
    int find_node(pair<int, int> coord);

};