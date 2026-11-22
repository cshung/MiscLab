#include "cartesian_tree.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void cartesian_tree_test() {
  cartesian_tree tree;
  for (int i = 0; i < 100; i++) {
    tree.insert(rand() % 1000, rand() % 1000);
  }
}