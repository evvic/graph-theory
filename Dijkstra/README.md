# Dijkstra's Algorithm

#### Single Source Shortest Path (SSSP) algorithm for graphs with **non-negative edge weights**.

## Constraint

- The (non-negative edge weight) contraint ensures that once a node has been visited, its optimal distance can not be improved (it only grows).
  - Allows Dijkstra's Algorithm to try selecting the next most promising node.

## Time Complexity

Typically the time complexity when implementing this algortihm is `O(E*log(V))`
- Competitive amongst other shortest path algorithms.