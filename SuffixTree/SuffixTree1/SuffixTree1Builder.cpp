#include "SuffixTree1Builder.h"
#include "SuffixTree1.h"
#include <iostream>
using namespace std;

void SuffixTree1Builder::BuildSuffixTree(string input, SuffixTree1* suffixTree)
{
    string s = "xabxac";
    unsigned int end = s.length();
    for (unsigned int start = 0; start < end; start++)
    {
        suffixTree->Add(s.substr(start, end - start));
    }
}