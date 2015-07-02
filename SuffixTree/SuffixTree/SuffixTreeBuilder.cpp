#include "SuffixTreeBuilder.h"
#include "SuffixTree.h"
#include <iostream>
using namespace std;

SuffixTreeBuilder::SuffixTreeBuilder(string& input) : m_input(input), m_lastInternalNode(nullptr), m_nextStart(nullptr), m_currentEnd(0)
#ifdef _DEBUG
, m_extensionCount(0)
, m_lastInternalNodeBirthday(-1)
#endif
{

}

void SuffixTreeBuilder::BuildSuffixTree(SuffixTree* suffixTree)
{
    string& s = this->m_input;
    this->m_nextStart = suffixTree->m_root;
    this->m_nextDepth = 0;
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