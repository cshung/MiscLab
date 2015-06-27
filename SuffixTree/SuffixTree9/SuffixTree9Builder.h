#pragma once

#include <string>
#include "SuffixTree9.h"
using namespace std;

class SuffixTree9Builder
{
public:
    SuffixTree9Builder();
    void BuildSuffixTree(string input, SuffixTree9* suffixTree);
private:
    SuffixTree9::SuffixTree9Edge* m_lastInternalNode;
    SuffixTree9::SuffixTree9Edge* m_nextStart;
    SuffixTree9::SuffixTree9Edge* m_fullStringLeaf;
    friend class SuffixTree9;

    string m_input;
#ifdef _DEBUG
    int m_extensionCount;
    int m_lastInternalNodeBirthday;
#endif
};