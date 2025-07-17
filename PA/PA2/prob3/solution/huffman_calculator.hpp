#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <tuple>
#include <vector>


void swap(std::vector<std::pair<size_t, size_t>> &data, size_t x, size_t y) {
  std::pair<size_t, size_t> temp = data[y];
  data[y] = std::move(data[x]);
  data[x] = std::move(temp);
}


void pop(std::vector<std::pair<size_t, size_t>> &data) {
  if (data.size() < 2) { return; }
  if (data.size() == 2) {
    data.pop_back();
    return;
  }
  data[1] = data[data.size() - 1];
  data.pop_back();
  size_t index = 1;
  size_t left = 2*index;
  size_t right = 2*index+1;
  while (left < data.size()) {
    if (data[left].first < data[index].first) {
      if (right < data.size()) {
        if (data[left].first > data[right].first) {
          swap(data, index, right);
          index = right;
        } else if (data[left].first == data[right].first) {
          data[right].second += data[left].second;
          data[left] = {0, 0};
          swap(data, index, left);
          index = left;
        } else {
          swap(data, index, left);
          index = left;
        }
      } else {
        swap(data, index, left);
        index = left;
      }
    } else if (right < data.size() && data[right].first < data[index].first) {
      swap(data, index, right);
      index = right;
    } else {
      break;
    }
    left = 2*index;
    right = 2*index+1;
  }
  if (data.size() > 1 && (data[1].second == 0 || data[1].second == 0)) {
    pop(data);
  }
}


void insert(std::vector<std::pair<size_t, size_t>> &data, std::pair<size_t, size_t> node) {
  data.push_back(node);
  size_t index = data.size() - 1;
  size_t parent = index/2;
  while (parent > 0) {
    if (data[index] < data[parent]) {
      swap(data, index, parent);
      index = parent;
    } else if (data[index] == data[parent]) {
      data[parent].second += data[index].second;
      data[index] = {0, 0};
      break;
    } else {
      break;
    }
    parent = index / 2;
  }
}


void percolatDown(std::vector<std::pair<size_t, size_t>> &data, size_t index) {
  size_t left = 2*index;
  size_t right = 2*index+1;
  while (left < data.size()) {
    if (data[index].first > data[left].first) {
      if (right < data.size() && data[right].first < data[left].first) {
        swap(data, index, right);
        index = right;
      } else {
        swap(data, index, left);
        index = left;
      }
    } else if (right < data.size() && data[index].first > data[right].first) {
      swap(data, index, right);
      index = right;
    } else {
      break;
    }
    left = 2*index;
    right = 2*index+1;
  }
}


size_t get_huffman_length(const std::vector<std::pair<size_t, size_t>> &data) {
  // TODO: Implement this.
  if (data.empty()) { return 0; }
  if (data.size() == 1 && data[0].second == 1) { return data[0].first; }
  size_t len = 0;
  size_t sum = 0;
  std::vector<std::pair<size_t, size_t>> sorted_data(data.size() + 1);

  for (size_t i = 0; i < data.size(); i++) {
    sum += data[i].first * data[i].second;
    sorted_data[i + 1] = data[i];
  }

  for (size_t i = (sorted_data.size() - 1) / 2; i > 0; i--) {
    percolatDown(sorted_data, i);
  }
  
  while (sorted_data.size() > 1) {
    if (sorted_data[1].first == 0 || sorted_data[1].second == 0) {
      pop(sorted_data);
    } else if (sorted_data[1].second == 1) {
      if (sorted_data.size() == 2) {
        len += sorted_data[1].first;
        break;
      }
      std::pair<size_t, size_t> temp = sorted_data[1];
      pop(sorted_data);
      if (sorted_data[1].first == 0 || sorted_data[1].second < 1) { pop(sorted_data); }
      std::pair<size_t, size_t> node(temp.first + sorted_data[1].first, 1);
      len += node.first;
      sorted_data[1].second -= temp.second;
      if (sorted_data[1].second < 1) { pop(sorted_data); }     
      insert(sorted_data, node);
    } else {
      std::pair<size_t, size_t> node(sorted_data[1].first * 2, sorted_data[1].second / 2);
      len += node.first * node.second;
      sorted_data[1].second -= node.second * 2;
      insert(sorted_data, node);
    }
  }

  return len - sum;
}
#endif // HUFFMAN_CALCULATOR_HPP