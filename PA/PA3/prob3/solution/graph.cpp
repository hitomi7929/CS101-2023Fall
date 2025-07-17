#include "graph.hpp"
#include <cmath>


using VertexID = std::size_t;
using Weight = std::int64_t;

struct HeapEntry {
  VertexID vertex;
  Weight dist;

  auto operator<=>(const HeapEntry &rhs) const { return dist <=> rhs.dist; }
};

template <typename T>
using MinHeap = std::priority_queue<T, std::vector<T>, std::greater<>>;

auto Graph::dijkstra(VertexID source) const -> std::vector<Weight> {
  // FIXME: This is a naive O(V^2) implementation. Overwrite it to make it run
  // within O(ElogV) time, which is more efficient when E = o(V^2/logV).
  std::vector<Weight> dist(numVertices(), infinity);
  std::vector<bool> done(numVertices(), false);
  MinHeap<HeapEntry> minHeap;
  dist[source] = 0;
  minHeap.push({.vertex = source, .dist = dist[source]});
  while (!minHeap.empty()) {
    auto [current, minDist] = minHeap.top();
    minHeap.pop();
    done[current] = true;
    for (auto [to, weight] : mAdjacent[current]) {
      if (!done[to] && dist[current] + weight < dist[to]) {
        dist[to] = dist[current] + weight;
        minHeap.push({.vertex = to, .dist = dist[to]});
      }
    }
  }
  return dist;
}

auto Graph::bellmanFord(VertexID source) const
    -> std::optional<std::vector<Weight>> {
  // TODO: Implement this.
  std::vector<Weight> dist(numVertices(), infinity);
  dist[source] = 0;
  bool to_continue = true;
  for (VertexID i = 0; to_continue && i != numVertices(); i++) {
    to_continue = false;
    for (VertexID from = 0; from != numVertices(); from++) {
      for (auto [to, weight] : mAdjacent[from]) {
        if (dist[from] != infinity && dist[to] > weight + dist[from]) {
          dist[to] = weight + dist[from];
          to_continue = true;
        }
      }
    }
  }
  for (VertexID from = 0; from != numVertices(); from++) {
    for (auto [to, weight] : mAdjacent[from]) {
      if (dist[from] != infinity && dist[to] > weight + dist[from]) {
        return std::nullopt;
      }
    }
  }
  return dist;
}