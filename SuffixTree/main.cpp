#include "Trie.h"
#include "CompressedTrie.h"
#include "SuffixTree1\SuffixTree1Builder.h"
#include "SuffixTree2\SuffixTree2Builder.h"
#include "SuffixTree3\SuffixTree3Builder.h"
#include "SuffixTree4\SuffixTree4Builder.h"
#include "SuffixTree5\SuffixTree5Builder.h"
#include "SuffixTree6\SuffixTree6Builder.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// For Post 1
void TestTrie()
{
    int a = 1;
    int b = 2;
    int c = 3;
    Trie trie;
    trie.Add("Hello", &a);
    trie.Add("Happy", &b);
    trie.Add("Halloween", &c);
    cout << (&a == trie.Get("Hello")) << endl;
    cout << (&b == trie.Get("Happy")) << endl;
    cout << (&c == trie.Get("Halloween")) << endl;
    cout << trie.Show() << endl;
}

// For Post 2
void TestCompressedTrie()
{
    int a = 1;
    int b = 2;
    int c = 3;
    CompressedTrie compressedTrie;
    compressedTrie.Add("Hello", &a);
    compressedTrie.Add("Happy", &b);
    compressedTrie.Add("Halloween", &c);
    cout << (&a == compressedTrie.Get("Hello")) << endl;
    cout << (&b == compressedTrie.Get("Happy")) << endl;
    cout << (&c == compressedTrie.Get("Halloween")) << endl;
    cout << compressedTrie.Show() << endl;
}

// For Post 4
void TestSuffixTree1()
{
    SuffixTree1Builder builder;
    SuffixTree1 suffixTree;
    builder.BuildSuffixTree("Hello_world_to_suffix_tree", &suffixTree);
    cout << suffixTree.Show() << endl;
}

// For Post 5
void TestSuffixTree2()
{
    SuffixTree2Builder builder;
    SuffixTree2 suffixTree;
    builder.BuildSuffixTree("Hello_world_to_suffix_tree", &suffixTree);
    cout << suffixTree.Show() << endl;
}

// For Post 7
void TestSuffixTree3()
{
    SuffixTree3Builder builder;
    SuffixTree3 suffixTree;
    builder.BuildSuffixTree("Hello_world_to_suffix_tree", &suffixTree);
    cout << suffixTree.Show() << endl;
}

// For next post
void TestSuffixTree4()
{
    SuffixTree4Builder builder;
    SuffixTree4 suffixTree;
    builder.BuildSuffixTree("Hello_world_to_suffix_tree", &suffixTree);
    cout << suffixTree.Show() << endl;
}

// For next next post
void TestSuffixTree5()
{
    SuffixTree5Builder builder;
    SuffixTree5 suffixTree;
    builder.BuildSuffixTree("Hello_world_to_suffix_tree", &suffixTree);
    cout << suffixTree.Show() << endl;
}

// For next next next post
void TestSuffixTree6()
{
    SuffixTree6Builder builder;
    SuffixTree6 suffixTree;
    builder.BuildSuffixTree("Hello_world_to_suffix_tree", &suffixTree);
    cout << suffixTree.Show() << endl;
}

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
    */
    TestSuffixTree6();
    return 0;
}