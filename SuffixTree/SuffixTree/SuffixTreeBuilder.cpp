#include "SuffixTreeBuilder.h"
#include "SuffixTree.h"
#include <iostream>
using namespace std;

SuffixTreeBuilder::SuffixTreeBuilder(string& input) : m_input(input), m_lastInternalNode(nullptr), m_nextStart(nullptr), m_currentEnd(0)
{

}

void SuffixTreeBuilder::BuildSuffixTree(SuffixTree* suffixTree)
{
    string& s = this->m_input;
    unsigned int start = 0;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        this->m_nextStart = suffixTree->m_root;
        this->m_nextDepth = 0;
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