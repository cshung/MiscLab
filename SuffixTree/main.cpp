#include "SuffixTree/SuffixTreeBuilder.h"
#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
using namespace std;

int main()
{
    unsigned int length = 100000;
    char* source = new char[length + 1];
    for (unsigned int i = 0; i < length; i++)
    {
        source[i] = rand() % 26 + 'A';
    }
    source[length] = 0;
    string s(source);
    SuffixTreeBuilder builder(s);
    SuffixTree suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    // cout << suffixTree.Show(s, &builder) << endl;
    return 0;
}
