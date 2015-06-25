#include "SuffixTree2Builder.h"
#include "SuffixTree2.h"
#include <iostream>
using namespace std;

void SuffixTree2Builder::BuildSuffixTree(string input, SuffixTree2* suffixTree)
{
    string s = "xabxac";
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        for (unsigned int start = 0; start < end; start++)
        {
            suffixTree->Add(s.substr(start, end - start));
        }
    }
}