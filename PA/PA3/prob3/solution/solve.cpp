#include <optional>
#include <cassert>

#include "problem.hpp"
#include "graph.hpp"

/// @brief Solve the given difference constraints problem.
/// @param problem 
/// @return The solution, or @c std::nullopt if the problem has no solutions.
std::optional<Problem::Solution> solve(const Problem &problem) {
  // TODO: Your code here.
  if (problem.hasNegativeConstant()) {
    std::size_t n = problem.getNumVars();
    Graph g(n+1);
    for (std::size_t i = 0; i < n; i++) {
      g.addEdge(n, i, (int64_t)0);
    }
    for (auto constraint : problem.getConstraints()) {
      g.addEdge(constraint.var2, constraint.var1, constraint.constant);
    }
    auto temp = g.bellmanFord(n);
    if (temp) {
      auto ans = temp.value();
      ans.pop_back();
      return ans;
    }
    return std::nullopt;
  } 
  std::vector<int64_t> ans(problem.getNumVars());
  return ans;
}

int main() {
  Problem p1(5, {{0, 1, 0},
                 {0, 4, -1},
                 {1, 4, 1},
                 {2, 0, 5},
                 {3, 0, 4},
                 {3, 2, -1},
                 {4, 2, -3},
                 {4, 3, -3}});

  // This is a possible solution.
  assert(p1.isFeasible({-5, -3, 0, -1, -4}));

  auto sol = solve(p1);
  assert(sol);                        // The problem has a solution.
  assert(p1.isFeasible(sol.value())); // The solution is correct.

  return 0;
}