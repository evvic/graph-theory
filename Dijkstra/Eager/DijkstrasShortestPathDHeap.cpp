#include <iostream>
#include <vector>
#include <limits> // for initializing to positive infinity
#include "DijkstrasShortestPathDHeap.h"
// Graph_Theory/Data_Structures/IndexedPriorityQueue.h
//#include "../../Data_Structures/IndexedPriorityQueue.h"

using namespace std;

// Edge class constructor within DijkstrasShortestPathDHeap
DijkstrasShortestPathDHeap::Edge::Edge(int to, double cost) {
    this->to = to;
    this->cost = cost;
}

//DijkstrasShortestPathDHeap::Edge::Edge() { this->to = 0; this->cost = 0.0; }

// private member function to create empty graph
void DijkstrasShortestPathDHeap::createEmptyGraph() {
    // Init only 1 dimension of the graph
    graph = vector<vector<Edge>>(n);
}

// CONSTRUCTOR
DijkstrasShortestPathDHeap::DijkstrasShortestPathDHeap(int n) {
    this->n = n;
    createEmptyGraph();
}

void DijkstrasShortestPathDHeap::addEdge(int from, int to, double cost) {
    edgeCount++;
    graph.at(from).push_back(Edge(to, cost));
}

double DijkstrasShortestPathDHeap::dijkstra(int start, int end) {

    // Keep an Indexed Priority Queue (ipq) of best node to visit
    int degree = edgeCount / n;

    // First parameter is key datatype
    // and it should be hashable
    // Second parameter is value datatype comparator
    // function (by default it implements maxheap)
    indexed_priority_queue<int, double> ipq;
    ipq.push(start, 0.0);

    // Maintain an array of min distance to each node
    vector<double> dist(n, std::numeric_limits<double>::max());
    dist.at(start) = 0.0;

    // Maintain an array of nodes if visited
    vector<bool> visited(n, false);

    // Maintain an array of previous visited nodes (reconstruct path)
    prev = vector<int>(edgeCount, NULL);

    while(!ipq.empty()) {
        cout << " queue: ";
        display(ipq);
        int nodeID = ipq.bottom().first;

        visited.at(nodeID)= true;
        double minVal = ipq.top().second;
        ipq.pop();

        // Ignore if already found a better path
        if (minVal > dist.at(nodeID)) {
            cout << "  Skipping " << nodeID << ", better path not using it." << endl;
            continue;
        }

        for(Edge edge : graph.at(nodeID)) {
            cout << "  node " << nodeID << " --" << edge.cost << "--> " << edge.to << endl;

            // Cannot get a shorter path from a previosuly visited node
            if (visited.at(edge.to)) {
                cout << "  Already visited " << edge.to << " from " << nodeID << ", skipping." << endl;
                continue;
            }

            // Relax edge through updating min cost
            double newDist = dist.at(nodeID) + edge.cost;

            if (newDist < dist.at(edge.to)) {
                prev.at(edge.to) = nodeID;
                dist.at(edge.to) = newDist;

                //  Add new node to queue if not there already
                if(!ipq.contains(edge.to)) {
                    cout << "   Add new node " << edge.to << " to queue with dist " << newDist << '.' << endl;
                    ipq.push(edge.to, newDist);
                }
                // ELSE Update cost of node traversal to existing node
                else {
                    cout << "   Update cost of node " << edge.to << " to " << newDist << endl;
                    ipq.changeAtKey(edge.to, newDist);
                }
            } else {
                cout << "   To node " << edge.to << " with cost " << newDist << " is not cheaper." << endl;
            }
        }
        // If the end node has been processed then it can return early.
        // No need to visit the whole graph because Dijkstra's algorithm
        // is greedy and there are no negative edge weights.
        //if (nodeID == end) return dist.at(end);
    }

    // End node is unreachable
    return std::numeric_limits<double>::max();
}

//
vector<int> DijkstrasShortestPathDHeap::reconstructPath(int start, int end) {
    vector<int> path;

    if (end < 0 || end >= n) {
        throw std::invalid_argument("Invalid end node index");
    }
    if (start < 0 || start >= n) {
        throw std::invalid_argument("Invalid start node index");
    }

    double dist = dijkstra(start, end);

    // If dist == positive infinity, return empty path.
    if (dist == std::numeric_limits<double>::max()) return path;

    for (int at = end; at != NULL; at = prev[at]) {
        path.push_back(at);
    }

    // Path is in reversed direction (end -> start)
    return path;
}

////
void DijkstrasShortestPathDHeap::display(indexed_priority_queue<int, double> IPQ) {

    indexed_priority_queue<int, double> temp = IPQ;
    while (!temp.empty()) {
        pair<int, double> tmp;
        tmp = temp.top();
        temp.pop();
        cout << "( " << tmp.first << ", "
             << tmp.second << " ) ";
    }
    cout << '\n';
}