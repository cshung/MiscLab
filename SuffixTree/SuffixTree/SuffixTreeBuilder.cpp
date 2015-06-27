#include "SuffixTreeBuilder.h"
#include "SuffixTree.h"
#include <iostream>
using namespace std;

SuffixTreeBuilder::SuffixTreeBuilder() : m_lastInternalNode(nullptr), m_nextStart(nullptr), m_fullStringLeaf(nullptr), m_currentEnd(0)
#ifdef _DEBUG
, m_extensionCount(0)
, m_lastInternalNodeBirthday(-1)
#endif
{

}

void SuffixTreeBuilder::BuildSuffixTree(string s, SuffixTree* suffixTree)
{
    this->m_input = s;
    this->m_nextStart = suffixTree->m_root;
    unsigned int start = 0;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        this->m_currentEnd = end;
        for (; start < end; start++)
        {
            if (suffixTree->Add(start, end, this))
            {
                break;
            }
        }
    }
}