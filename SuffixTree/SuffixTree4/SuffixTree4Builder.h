#pragma once

#include <string>
#include "SuffixTree4.h"
using namespace std;

class SuffixTree4Builder
{
public:
    SuffixTree4Builder();
    void BuildSuffixTree(string input, SuffixTree4* suffixTree);
private:
    SuffixTree4::SuffixTree4Edge* m_lastInternalNode;
    friend class SuffixTree4;
#ifdef _DEBUG
    int m_extensionCount;
    int m_lastInternalNodeBirthday;
#endif
};