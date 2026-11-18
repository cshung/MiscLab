#pragma once

#include <vector>

class veb {
public:
  veb();
  ~veb();
  void build(int bits);
  bool contains(int v) const;
  void insert(int v);
  void remove(int v);
  int predecessor(int v) const;
  int successor(int v) const;
  std::vector<int> enumerate() const;

private:
  bool insert_impl(int v);
  bool remove_impl(int v);
  int low_bits() const;
  int high_bits() const;
  int child_size() const;
  int children_count() const;
  int which_child(int v) const;
  int child_index(int v) const;
  int bits;
  int min;
  int max;
  veb *children;
  veb *summary;
};
