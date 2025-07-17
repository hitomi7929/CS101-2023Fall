#ifndef CS10123F_PA1_HASHTABLE
#define CS10123F_PA1_HASHTABLE

#include <cstdint>
#include <cstdio>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include "hash_types.hpp"

template <> struct std::hash<Website> {
  static const uint32_t base = 1009u; // 1009 is a prime number.
  uint32_t operator()(const Website &S) const noexcept {
    // ------------------------START YOUR CODE----------------
    if ((S.name).empty()) return 0;
    uint32_t hash = static_cast<uint32_t>((S.name)[0]);
    for (auto iter = (S.name).begin()+1; iter != (S.name).end(); iter++) {
      hash *= base;
      hash += static_cast<uint32_t>(*iter);
    }

    return hash;
    // ------------------------END OF YOUR CODE----------------
  }
};

template <class Key = Website, class T = IPv4> // template for key type class
                                               // Key and value type class T
class HashTable {
private:
  enum entrystate { Unused, Used, Erased };
  const size_t Mod = 1000037u; // 1000037 is a prime number
  std::vector<T> Table;        // main table stores value
  std::vector<Key> Stored_Key; // used for storing original key
  std::vector<entrystate>
      State; // keeps the state of every table entry : {Unused, Used or Erased};
  std::vector<size_t> Used_pos; // hint: used for function clear()

public:
  static const std::size_t npos = -1;
  HashTable() : Table(Mod), Stored_Key(Mod), State(Mod, entrystate::Unused) {}

  const T &operator[](const Key &x) const {
    std::size_t pos = find(x);
    if (pos == npos)
      throw std::invalid_argument("Key does not exist");
    return Table[pos];
  }

  // ------------------------START YOUR CODE----------------

  std::size_t Search(const Key &x) const {
    std::size_t pos = std::hash<Key>{}(x) % Mod;
    std::size_t p = pos;
    auto iter = Used_pos.begin();
    for (std::size_t i = 0; iter != Used_pos.end() && i < Mod; i++) {
      if (State[pos] == entrystate::Used) {
        if (Stored_Key[pos] == x) {
          return pos;
        } else {
          pos = (pos + 1) % Mod;
          iter++;
        }
      } else {
        if (Stored_Key[pos] == x) {
          while (State[p] == entrystate::Used) {
            p = (p + 1) % Mod;
          }
          return p;
        } else if (State[pos] == entrystate::Unused) {
          return pos;
        }
        pos = (pos + 1) % Mod;
      }
    }
    while (State[p] == entrystate::Used) {
      p = (p + 1) % Mod;
    }
    return p;
  }

  std::size_t find(const Key &x) const {
    std::size_t pos = std::hash<Key>{}(x) % Mod;
    auto iter = Used_pos.begin();
    for (std::size_t i = 0; iter != Used_pos.end() && i < Mod; i++) {
      if (State[pos] == entrystate::Used) {
        if (Stored_Key[pos] == x) {
          return pos;
        } else {
          pos = (pos + 1) % Mod;
          iter++;
        }
      } else {
        if (State[pos] == entrystate::Unused || Stored_Key[pos] == x) return npos;
        pos = (pos + 1) % Mod;
      }
    }
    return npos;
  }

  void insert(const Key &x, const T& value) {
    std::size_t pos = Search(x);
    if (State[pos] == entrystate::Used) {
      Table[pos] = value;
    } else {
      Used_pos.push_back(pos);
      Stored_Key[pos] = x;
      State[pos] = entrystate::Used;
      Table[pos] = value;
    }
  }

  bool erase(const Key &x) {
    std::size_t pos = find(x);
    if (pos == npos) {
      return false;
    }
    State[pos] = entrystate::Erased;
    auto iter = Used_pos.begin();
    while (*iter != pos) {
      iter++;
    }
    Used_pos.erase(iter);
    return true;
  }

  void clear() {
    for (std::size_t pos : Used_pos) {
      State[pos] = entrystate::Erased;
    }
    Used_pos.clear();
  }
};

#endif // CS10123F_PA1_HASHTABLE