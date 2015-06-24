#include "Trie.h"
#include "CompressedTrie.h"
#include "SuffixTree1Builder.h"
#include <iostream>
#include <string>
#include <cassert>
using namespace std;

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

void TestSuffixTree0()
{
    CompressedTrie compressedTrie;
    string s = "xabxac";
    unsigned int end = s.length();
    for (unsigned int start = 0; start < end; start++)
    {
        compressedTrie.Add(s.substr(start), &s);
    }
    cout << compressedTrie.Show() << endl;
}

void TestSuffixTree1()
{
    SuffixTree1Builder builder;
    builder.BuildSuffixTree("xabxac");
}

int main()
{
    //TestTrie();
    //TestCompressedTrie();
    //TestSuffixTree0();
    TestSuffixTree1();
    return 0;
}