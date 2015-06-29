#include "SuffixTree7Builder.h"
#include "SuffixTree7.h"
#include <iostream>
using namespace std;

SuffixTree7Builder::SuffixTree7Builder(string& input) : m_input(input), m_lastInternalNode(nullptr), m_nextStart(nullptr)
#ifdef _DEBUG
, m_extensionCount(0)
, m_lastInternalNodeBirthday(-1)
#endif
{

}

void SuffixTree7Builder::BuildSuffixTree(SuffixTree7* suffixTree)
{
    string& s = this->m_input;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        this->m_nextStart = suffixTree->m_root;
        for (unsigned int start = 0; start < end; start++)
        {
            if (suffixTree->Add(start, end, this))
            {
                break;
            }
        }
    }
}