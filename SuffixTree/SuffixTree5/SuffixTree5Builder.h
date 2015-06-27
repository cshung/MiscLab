#pragma once

#include <string>
#include "SuffixTree5.h"
using namespace std;

class SuffixTree5Builder
{
public:
    SuffixTree5Builder();
    void BuildSuffixTree(string input, SuffixTree5* suffixTree);
private:
    SuffixTree5::SuffixTree5Edge* m_lastInternalNode;
    SuffixTree5::SuffixTree5Edge* m_nextStart;
    unsigned int m_nextDepth;
    friend class SuffixTree5;
#ifdef _DEBUG
    int m_extensionCount;
    int m_lastInternalNodeBirthday;
#endif
};