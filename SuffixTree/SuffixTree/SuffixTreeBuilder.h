#pragma once

#include <string>
#include "SuffixTree.h"
using namespace std;

class SuffixTreeBuilder
{
public:
    SuffixTreeBuilder();
    void BuildSuffixTree(string input, SuffixTree* suffixTree);
private:
    SuffixTree::SuffixTreeEdge* m_lastInternalNode;
    SuffixTree::SuffixTreeEdge* m_nextStart;
    unsigned int m_nextDepth;
    unsigned int m_currentEnd;
    friend class SuffixTree;

    string m_input;
#ifdef _DEBUG
    int m_extensionCount;
    int m_lastInternalNodeBirthday;
#endif
};