#include <iostream>
#include <vector>

auto random_choice(auto begin, auto end) {
  // This is only a possible implementation of random_choice.
  // You can use this for your convenience or design a better one.
  std::size_t length = end - begin;
  std::size_t idx = std::rand() % length;
  return begin + idx;
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end, auto compare) {
  // TODO: IMPLEMENT THIS.
  if (begin >= end - 1) { return; }
  std::random_access_iterator auto left = begin;
  std::random_access_iterator auto right = end - 1;
  std::random_access_iterator auto pivot = random_choice(begin, end);
  auto base = std::move(*pivot);
  while (left < right) {
    while(left < pivot && !compare(base, *left)) { left++; }
    while (pivot < right && compare(base, *right)) { right--; }
    auto temp = std::move(*left);
    *left = std::move(*right);
    *right = temp;
    if (left < pivot && pivot < right) {
      left++;
    } else {
      if (pivot == left) {
        pivot = std::move(right);
        left++;
      } else if (pivot == right) {
        pivot = std::move(left);
        right--;
      }
    }
  }
  quick_sort(begin, pivot, compare);
  quick_sort(pivot, end, compare);
}

void quick_sort(std::random_access_iterator auto begin,
                std::random_access_iterator auto end) {
  quick_sort(begin, end, std::less<>{});
}
