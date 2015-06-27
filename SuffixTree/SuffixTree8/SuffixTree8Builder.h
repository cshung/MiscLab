#pragma once

#include <string>
#include "SuffixTree8.h"
using namespace std;

class SuffixTree8Builder
{
public:
    SuffixTree8Builder();
    void BuildSuffixTree(string input, SuffixTree8* suffixTree);
private:
    SuffixTree8::SuffixTree8Edge* m_lastInternalNode;
    SuffixTree8::SuffixTree8Edge* m_nextStart;
    unsigned int m_nextDepth;
    SuffixTree8::SuffixTree8Edge* m_fullStringLeaf;
    friend class SuffixTree8;

    string m_input;
#ifdef _DEBUG
    int m_extensionCount;
    int m_lastInternalNodeBirthday;
#endif
};