# Dijkstra's Algorithm

#### Single Source Shortest Path (SSSP) algorithm for graphs with **non-negative edge weights**.

## Constraint

- The (non-negative edge weight) contraint ensures that once a node has been visited, its optimal distance can not be improved (it only grows).
  - Allows Dijkstra's Algorithm to try selecting the next most promising node.

## Time Complexity

Typically the time complexity when implementing this algortihm is `O(E*log(V))`
- Competitive amongst other shortest path algorithms.

# Visualization

- For creating graphs of these nodes in a machine-readable manner this project will be using **graphviz**.
  - [Documentation](https://graphviz.org/)
  - For installing onto Linux: `sudo apt install graphviz`
## Graphviz
- Rendering a `.gv` (graphviz) file into an image (i.e. png) is very simple
  - Example: `dot -Tpng test.gv -o test.png`
    - Where the `dot` command is rendering the `test.gv` into a png through the ` -Tpng` option
    - The output ` -o` option is allowing to pass the name of the outputted png file, `test.png`