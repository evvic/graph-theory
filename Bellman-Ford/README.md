# Bellman-Ford Algorithm
- Single Source Shortest Path (SSSP).
  - Means it can find the shortest path from one node to any other node.
- BF is not ideal for most SSSP problems due to its time complexity **O(EV**.
  - Typically better to use Dijkstra's.
  - BF is viabe when negative edge weights can exist.
    - Therefore Dijkstra's cannot be used.
- BF works with **negative edge weights**.
  - Can determine negative cycles and their manifestation.

