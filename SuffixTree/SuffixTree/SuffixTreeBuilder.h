#pragma once

#include <string>
#include "SuffixTree.h"
using namespace std;

class SuffixTreeBuilder
{
public:
    SuffixTreeBuilder(string& input);
    void BuildSuffixTree(SuffixTree* suffixTree);
private:
    string& m_input;
    SuffixTree::SuffixTreeEdge* m_lastInternalNode;
    SuffixTree::SuffixTreeEdge* m_nextStart;
    unsigned int m_nextDepth;
    unsigned int m_currentEnd;
    friend class SuffixTree;
};