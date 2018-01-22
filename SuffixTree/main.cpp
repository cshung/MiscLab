#include "Trie.h"
#include "CompressedTrie.h"
#include "SuffixTree1\SuffixTree1Builder.h"
#include "SuffixTree2\SuffixTree2Builder.h"
#include "SuffixTree3\SuffixTree3Builder.h"
#include "SuffixTree4\SuffixTree4Builder.h"
#include "SuffixTree5\SuffixTree5Builder.h"
#include "SuffixTree6\SuffixTree6Builder.h"
#include "SuffixTree7\SuffixTree7Builder.h"
#include "SuffixTree7\SuffixTree7Builder.h"
#include "SuffixTree\SuffixTreeBuilder.h"
#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
using namespace std;

int main()
{
    /*
    TestTrie();
    TestCompressedTrie();
    TestSuffixTree1();
    TestSuffixTree2();
    TestSuffixTree3();
    TestSuffixTree4();
    TestSuffixTree5();
    TestSuffixTree6();
    TestSuffixTree7();
    TestSuffixTree();
    */
    
    // This will hit an assertion in character 86903
    unsigned int length = 100000;
    char* source = new char[length + 1];
    for (unsigned int i = 0; i < length; i++)
    {
        source[i] = rand() % 26 + 'A';
    }
    source[length] = 0;
    string s = source;
    clock_t s_time = clock();
    SuffixTreeBuilder builder(s);
    SuffixTree suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    clock_t e_time = clock();
    cout << e_time - s_time << endl;
    // cout << suffixTree.Show(s, &builder) << endl;
    return 0;
}
