#include "floyd-worshall.h"
#include <vector>
#include <limits> // for initializing to positive infinity
#include <iostream>

// Vars for checking bounds


using namespace std;

// N = size of the adjacency matrix
// dp = the memo table that will contain APSP solution (shortest path)
// next = matrix used to reeconstruct shortest path

FloydWorshall::FloydWorshall(vector<vector<double>> matrix) {
    cout << "FloydWorshall Constructor." << endl;
    n = matrix.size();
    dp = vector<vector<double>>(n, vector<double>(n));
    next = vector<vector<int>>(n, vector<int>(n));

    cout << "FloydWorshall Constructor beginning for loop." << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix.at(i).at(j) != POSITIVE_INFINITY) {
                next.at(i).at(j) = j;
            }
            dp.at(i).at(j) = matrix.at(i).at(j);
        }
    }
}

// Runs the APSP and calls solve()
// When APSP is called several times but is not wanted for solve()
// to also be called several times
// Returns the Solved All Pairs Shortest Path matrix
vector<vector<double>> FloydWorshall::getApspMatrix() {
    if (!solved) solve();
    return dp;
}


// Runs the Floyd-Worshall algorith,
void FloydWorshall::solve() {
    // Compute all pairs shortest paths
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)  {
                if (dp.at(i).at(k) + dp.at(k).at(j) < dp.at(i).at(j)) {
                    // Update the value of i -> j with the shortest value going through k
                    dp.at(i).at(j) = dp[i][k] + dp[k][j];
                    // Update node pointing of shortest path for path reconstruction
                    // It is now shorter to go i -> k -> j than i -> j
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    // Identify negative cycles by assigning nodes to NEGATIVE INFINITY
    // when they are in that cycle
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)  {
                // If we can improve upon the already optimal solution
                // then it is in a negative cycle
                if (dp.at(i).at(k) + dp.at(k).at(j) < dp.at(i).at(j)) {
                    dp.at(i).at(j) = NEGATIVE_INFINITY;
                    next[i][j] = REACHES_NEGATIVE_CYCLE;
                }
            }
        }
    }

    solved = true;
}

vector<int> FloydWorshall::reconstructPath(int start, int end) {
    if (!solved) solve();
    vector<int> path;

    if (dp.at(start).at(end) == POSITIVE_INFINITY) return path;

    int at = start;

    for (; at != end; at = next.at(at).at(end)) {
        // Return null if an infinite num of shortest paths
        if (at == REACHES_NEGATIVE_CYCLE) return vector<int> ();
        path.push_back(at);
    }

    // Return null if an infinite num of shortest paths
    if (next.at(at).at(end) == REACHES_NEGATIVE_CYCLE) return vector<int> ();

    path.push_back(end);
    return path;
}


