#include "van_empe_boas_test.h"
#include "van_empe_boas.h"
#include <algorithm>
#include <cstdlib>
#include <stdio.h>

void van_empe_boas_test() {
  int target_bits = 12;
  int target_size = 1 << target_bits;
  int target_count = target_size / 3;
  std::vector<int> values;
  std::vector<bool> seen(target_size, false);
  for (int i = 0; i < target_count; i++) {
    int target_value = rand() % target_size;
    if (seen[target_value]) {
      i -= 1;
    } else {
      seen[target_value] = true;
      values.push_back(target_value);
    }
  }
  std::sort(values.begin(), values.end());
  veb tree;
  tree.build(target_bits);
  for (auto v : values) {
    tree.insert(v);
  }
  if (tree.enumerate() == values) {
    printf("Insertion passed\n");
  }
  int predecesor_failed = 0;
  int successor_failed = 0;
  for (int i = 0; i < values.size() - 1; i++) {
    if (tree.predecessor(values[i + 1]) != values[i]) {
      predecesor_failed += 1;
    }
    if (tree.successor(values[i]) != values[i + 1]) {
      successor_failed += 1;
    }
  }
  if (predecesor_failed == 0) {
    printf("Predecessor passed\n");
  }
  if (successor_failed == 0) {
    printf("Successor passed\n");
  }
  std::random_shuffle(values.begin(), values.end());
  for (auto v : values) {
    tree.remove(v);
  }
  if (tree.enumerate().empty()) {
    printf("Deletion passed\n");
  }
}