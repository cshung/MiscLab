#include "SuffixTree3Builder.h"
#include "SuffixTree3.h"
#include <iostream>
using namespace std;

SuffixTree3Builder::SuffixTree3Builder(string& input) : m_input(input)
{

}

void SuffixTree3Builder::BuildSuffixTree(SuffixTree3* suffixTree)
{
    string& s = this->m_input;
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        for (unsigned int start = 0; start < end; start++)
        {
            suffixTree->Add(s.substr(start, end - start));
        }
    }
}