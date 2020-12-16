cmake_minimum_required(VERSION 2.6)
project(data_structure CXX)
add_definitions(-std=c++11)
set (SRCS
btree.cpp
btree_test.cpp
cartesian_tree.cpp
cartesian_tree_test.cpp
disjoint_set_union_find.cpp
disjoint_set_union_find_test.cpp
fenwick_tree.cpp
fenwick_tree_test.cpp
hash_trie.cpp
hash_trie_test.cpp
hungarian.cpp
hungarian_test.cpp
kd_tree.cpp
min_max_heap.cpp
min_max_heap_test.cpp
min_max_queue.cpp
min_max_queue_test.cpp
Program.cpp
splay_tree.cpp
suffix_tree.cpp
suffix_tree_test.cpp
)
add_executable (data_structure ${SRCS})

