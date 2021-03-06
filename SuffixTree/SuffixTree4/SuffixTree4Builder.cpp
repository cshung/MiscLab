#include "SuffixTree4Builder.h"
#include "SuffixTree4.h"
#include <iostream>
using namespace std;

SuffixTree4Builder::SuffixTree4Builder(string& input) : m_input(input), m_lastInternalNode(nullptr)
#ifdef _DEBUG
, m_extensionCount(0)
, m_lastInternalNodeBirthday(-1)
#endif
{

}

void SuffixTree4Builder::BuildSuffixTree(SuffixTree4* suffixTree)
{
    string& s = this->m_input;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        for (unsigned int start = 0; start < end; start++)
        {
            suffixTree->Add(s.substr(start, end - start), this);
        }
    }
}