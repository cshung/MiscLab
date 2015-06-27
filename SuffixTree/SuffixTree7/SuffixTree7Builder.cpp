#include "SuffixTree7Builder.h"
#include "SuffixTree7.h"
#include <iostream>
using namespace std;

SuffixTree7Builder::SuffixTree7Builder() : m_lastInternalNode(nullptr), m_nextStart(nullptr), m_fullStringLeaf(nullptr)
#ifdef _DEBUG
, m_extensionCount(0)
, m_lastInternalNodeBirthday(-1)
#endif
{

}

void SuffixTree7Builder::BuildSuffixTree(string s, SuffixTree7* suffixTree)
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
            suffixTree->Add(start, end, this);
        }
    }
}