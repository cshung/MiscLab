#pragma once

#include <string>
#include "SuffixTree6.h"
using namespace std;

class SuffixTree6Builder
{
public:
    SuffixTree6Builder(string& input);
    void BuildSuffixTree(SuffixTree6* suffixTree);
private:
    string& m_input;
    SuffixTree6::SuffixTree6Edge* m_lastInternalNode;
    SuffixTree6::SuffixTree6Edge* m_nextStart;
    unsigned int m_nextDepth;
    friend class SuffixTree6;
#ifdef _DEBUG
    int m_extensionCount;
    int m_lastInternalNodeBirthday;
#endif
};