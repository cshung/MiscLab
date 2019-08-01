#include "SuffixTree/SuffixTreeBuilder.h"
#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
using namespace std;

int main()
{
    string s = "GATTACA0TAGACCA1ATACA2";
    SuffixTreeBuilder builder(s);
    SuffixTree suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    // cout << suffixTree.Show(s, &builder) << endl;
    return 0;
}
