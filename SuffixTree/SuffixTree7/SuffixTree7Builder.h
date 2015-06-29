#pragma once

#include <string>
#include "SuffixTree7.h"
using namespace std;

class SuffixTree7Builder
{
public:
    SuffixTree7Builder(string& input);
    void BuildSuffixTree(SuffixTree7* suffixTree);
private:
    string& m_input;
    SuffixTree7::SuffixTree7Edge* m_lastInternalNode;
    SuffixTree7::SuffixTree7Edge* m_nextStart;
    unsigned int m_nextDepth;
    friend class SuffixTree7;
#ifdef _DEBUG
    int m_extensionCount;
    int m_lastInternalNodeBirthday;
#endif
};