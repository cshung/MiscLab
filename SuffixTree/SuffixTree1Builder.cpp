#include "SuffixTree1Builder.h"
#include "SuffixTree1.h"
#include <iostream>
using namespace std;

void SuffixTree1Builder::BuildSuffixTree(string input)
{
    SuffixTree1 suffixTree;
    string s = "xabxac";
    for (unsigned int end = 1; end <= s.length(); end++)
    {
        for (unsigned int start = 0; start < end; start++)
        {
            suffixTree.Add(s.substr(start));
        }
    }
    cout << suffixTree.Show() << endl;
}