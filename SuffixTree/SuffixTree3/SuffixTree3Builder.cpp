#include "SuffixTree3Builder.h"
#include "SuffixTree3.h"
#include <iostream>
using namespace std;

void SuffixTree3Builder::BuildSuffixTree(string s, SuffixTree3* suffixTree)
{
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        for (unsigned int start = 0; start < end; start++)
        {
            suffixTree->Add(s.substr(start, end - start));
        }
    }
}