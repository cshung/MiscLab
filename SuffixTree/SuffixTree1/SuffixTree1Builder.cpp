#include "SuffixTree1Builder.h"
#include "SuffixTree1.h"
#include <iostream>
using namespace std;

SuffixTree1Builder::SuffixTree1Builder(string& input) : m_input(input)
{

}

void SuffixTree1Builder::BuildSuffixTree(SuffixTree1* suffixTree)
{
    string& s = this->m_input;
    unsigned int end = s.length();
    for (unsigned int start = 0; start < end; start++)
    {
        suffixTree->Add(s.substr(start, end - start));
    }
}