#include <iostream>

#include "../graph.hpp"

int main() {
  // Graph g(5);
  // g.addEdge(0, 1, 5);
  // g.addEdge(0, 4, 3);
  // g.addEdge(1, 2, 6);
  // g.addEdge(1, 3, 4);
  // g.addEdge(2, 3, 7);
  // g.addEdge(4, 3, 6);
  // g.addEdge(3, 2, 2);

  // g.addEdge(0, 1, 6);
  // g.addEdge(0, 2, 7);
  // g.addEdge(1, 2, 8);
  // g.addEdge(1, 3, -4);
  // g.addEdge(1, 4, 5);
  // g.addEdge(2, 3, 9);
  // g.addEdge(2, 4, -3);
  // g.addEdge(3, 0, 2);
  // g.addEdge(3, 4, 7);
  // g.addEdge(4, 1, -2);

  Graph g(6);
  g.addEdge(1, 0, 0);
  g.addEdge(4, 0, -1);
  g.addEdge(4, 1, 1);
  g.addEdge(0, 2, 5);
  g.addEdge(0, 3, 4);
  g.addEdge(2, 3, -1);
  g.addEdge(2, 4, -3);
  g.addEdge(3, 4, -3);
  g.addEdge(5, 0, 0);
  g.addEdge(5, 1, 0);
  g.addEdge(5, 2, 0);
  g.addEdge(5, 3, 0);
  g.addEdge(5, 4, 0);

  g.bfs(0, [](auto x) { std::cout << x << ' '; });
  std::cout << std::endl;

  auto dist = g.dijkstra(0);
  for (auto x : dist)
    std::cout << x << ' ';
  std::cout << std::endl;

  auto dist1 = g.bellmanFord(5);
  // auto dist1 = g.bellmanFord(0);
  if (dist1) {
    auto ans = dist1.value();
    for (auto x : ans)
      std::cout << x << ' ';
    std::cout << std::endl;
  } else {
    std::cout << "Negative circle!" << std::endl;
  }
  

  return 0;
}