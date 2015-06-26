#pragma once

#include <string>
#include "SuffixTree6.h"
using namespace std;

class SuffixTree6Builder
{
public:
    SuffixTree6Builder();
    void BuildSuffixTree(string input, SuffixTree6* suffixTree);
private:
    SuffixTree6::SuffixTree6Edge* m_lastInternalNode;
    SuffixTree6::SuffixTree6Edge* m_nextStart;
    friend class SuffixTree6;
#ifdef _DEBUG
    int m_extensionCount;
    int m_lastInternalNodeBirthday;
#endif
};