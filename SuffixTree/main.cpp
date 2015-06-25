#include "Trie.h"
#include "CompressedTrie.h"
#include "SuffixTree1.h"
#include "SuffixTree1Builder.h"
#include "SuffixTree2.h"
#include "SuffixTree2Builder.h"
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
    builder.BuildSuffixTree("xabxac", &suffixTree);
    cout << suffixTree.Show() << endl;
}

// For Post 5
void TestSuffixTree2()
{
    SuffixTree2Builder builder;
    SuffixTree2 suffixTree;
    builder.BuildSuffixTree("xabxac", &suffixTree);
    cout << suffixTree.Show() << endl;
}

int main()
{
    TestTrie();
    TestCompressedTrie();
    TestSuffixTree1();
    TestSuffixTree2();
    return 0;
}