cmake_minimum_required(VERSION 2.6)
project(SuffixTree CXX)
add_definitions(-std=c++11)
set (SRCS
CompressedTrie.cpp
main.cpp
Trie.cpp
SuffixTree/SuffixTree.cpp
SuffixTree/SuffixTreeBuilder.cpp
SuffixTree1/SuffixTree1.cpp
SuffixTree1/SuffixTree1Builder.cpp
SuffixTree2/SuffixTree2.cpp
SuffixTree2/SuffixTree2Builder.cpp
SuffixTree3/SuffixTree3.cpp
SuffixTree3/SuffixTree3Builder.cpp
SuffixTree4/SuffixTree4.cpp
SuffixTree4/SuffixTree4Builder.cpp
SuffixTree5/SuffixTree5.cpp
SuffixTree5/SuffixTree5Builder.cpp
SuffixTree6/SuffixTree6.cpp
SuffixTree6/SuffixTree6Builder.cpp
SuffixTree7/SuffixTree7.cpp
SuffixTree7/SuffixTree7Builder.cpp
)
add_executable (SuffixTree ${SRCS})
