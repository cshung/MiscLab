#include "SuffixTree6Builder.h"
#include "SuffixTree6.h"
#include <iostream>
using namespace std;

SuffixTree6Builder::SuffixTree6Builder(string& input) : m_input(input), m_lastInternalNode(nullptr), m_nextStart(nullptr)
#ifdef _DEBUG
, m_extensionCount(0)
, m_lastInternalNodeBirthday(-1)
#endif
{

}

void SuffixTree6Builder::BuildSuffixTree(SuffixTree6* suffixTree)
{
    string& s = this->m_input;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        this->m_nextStart = suffixTree->m_root;
        for (unsigned int start = 0; start < end; start++)
        {
            suffixTree->Add(start, end, this);
        }
    }
}