#include "SuffixTree2Builder.h"
#include "SuffixTree2.h"
#include <iostream>
using namespace std;

SuffixTree2Builder::SuffixTree2Builder(string& input) : m_input(input)
{

}

void SuffixTree2Builder::BuildSuffixTree(SuffixTree2* suffixTree)
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