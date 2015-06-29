#include "SuffixTree5Builder.h"
#include "SuffixTree5.h"
#include <iostream>
using namespace std;

SuffixTree5Builder::SuffixTree5Builder(string& input) : m_input(input), m_lastInternalNode(nullptr), m_nextStart(nullptr)
#ifdef _DEBUG
, m_extensionCount(0)
, m_lastInternalNodeBirthday(-1)
#endif
{

}

void SuffixTree5Builder::BuildSuffixTree(SuffixTree5* suffixTree)
{
    string& s = this->m_input;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        this->m_nextStart = suffixTree->m_root;
        for (unsigned int start = 0; start < end; start++)
        {
            suffixTree->Add(s.substr(start, end - start), this);
        }
    }
}