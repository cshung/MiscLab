#include "SuffixTree9Builder.h"
#include "SuffixTree9.h"
#include <iostream>
using namespace std;

SuffixTree9Builder::SuffixTree9Builder() : m_lastInternalNode(nullptr), m_nextStart(nullptr), m_fullStringLeaf(nullptr)
#ifdef _DEBUG
, m_extensionCount(0)
, m_lastInternalNodeBirthday(-1)
#endif
{

}

void SuffixTree9Builder::BuildSuffixTree(string s, SuffixTree9* suffixTree)
{
    this->m_input = s;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        if (this->m_fullStringLeaf == nullptr)
        {
            this->m_nextStart = suffixTree->m_root;
        }
        else
        {
            this->m_nextStart = this->m_fullStringLeaf;
        }
        for (unsigned int start = 0; start < end; start++)
        {
            if (suffixTree->Add(start, end, this))
            {
                break;
            }
        }
    }
}