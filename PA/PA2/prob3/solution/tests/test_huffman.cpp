#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "../huffman_calculator.hpp"

using input_type = std::vector<std::pair<std::size_t, std::size_t>>;
using return_type = std::size_t;
// std::map<int, std::pair<input_type, return_type>> testcases = {
//     {1, {{{7, 1}, {3, 2}, {2, 1}, {1, 2}}, 39}},
//     {2, {{{9, 1}, {3, 2}, {1, 2}, {2, 1}}, 41}},
//     {3, {{{1, 142857}}, 2452139}},
//     {4, {{{10, 10}}, 340}},
//     {5, {{{20, 5}}, 240}},
//     {6, {{{20, 1}, {40, 2}}, 160}},
// };

std::map<int, std::pair<input_type, return_type>> testcases = {
    {1, {{{10, 1}}, 10}},
    {2, {{{20, 1}, {10, 1}, {30, 1}}, 90}},
    {3, {{{10, 1}, {20, 1}}, 30}},
    {4, {{{1, 6}, {2, 4}, {4, 1}, {8, 1}}, 84}},
    {5, {{{1, 5}, {2, 4}, {4, 1}, {8, 1}}, 78}},
    {6, {{{4, 3}, {2, 2}}, 36}},
    {7, {{{4, 4}, {2, 2}}, 52}},
};

int main() {
  for (const auto &item : testcases) {
    std::cout << "Local testcase: " << item.first
              << "  Your: " << get_huffman_length(item.second.first)
              << "  Answer: " << item.second.second << std::endl;
  }
  return 0;
}