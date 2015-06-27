#pragma once

#include <string>
#include "SuffixTree7.h"
using namespace std;

class SuffixTree7Builder
{
public:
    SuffixTree7Builder();
    void BuildSuffixTree(string input, SuffixTree7* suffixTree);
private:
    SuffixTree7::SuffixTree7Edge* m_lastInternalNode;
    SuffixTree7::SuffixTree7Edge* m_nextStart;
    friend class SuffixTree7;

    string m_input;
#ifdef _DEBUG
    int m_extensionCount;
    int m_lastInternalNodeBirthday;
#endif
};