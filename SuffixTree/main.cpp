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
    string s = "Hello_world_to_suffix_tree";
    SuffixTree1Builder builder(s);
    SuffixTree1 suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    cout << suffixTree.Show() << endl;
}

// For Post 5
void TestSuffixTree2()
{
    string s = "Hello_world_to_suffix_tree";
    SuffixTree2Builder builder(s);
    SuffixTree2 suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    cout << suffixTree.Show() << endl;
}

// For Post 7
void TestSuffixTree3()
{
    string s = "Hello_world_to_suffix_tree";
    SuffixTree3Builder builder(s);
    SuffixTree3 suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    cout << suffixTree.Show() << endl;
}

// For next post
void TestSuffixTree4()
{
    string s = "Hello_world_to_suffix_tree";
    SuffixTree4Builder builder(s);
    SuffixTree4 suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    cout << suffixTree.Show() << endl;
}

void TestSuffixTree5()
{
    string s = "Hello_world_to_suffix_tree";
    SuffixTree5Builder builder(s);
    SuffixTree5 suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    cout << suffixTree.Show() << endl;
}

// For next next next post
void TestSuffixTree6()
{
    string s = "Hello_world_to_suffix_tree";
    SuffixTree6Builder builder(s);
    SuffixTree6 suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    cout << suffixTree.Show(s) << endl;
}

// For next next next post
void TestSuffixTree7()
{
    string s = "Hello_world_to_suffix_tree";
    SuffixTree7Builder builder(s);
    SuffixTree7 suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    cout << suffixTree.Show(s) << endl;
}

void TestSuffixTree()
{
    string s = "Hello_world_to_suffix_tree";
    SuffixTreeBuilder builder(s);
    SuffixTree suffixTree;
    builder.BuildSuffixTree(&suffixTree);
    cout << suffixTree.Show(s, &builder) << endl;
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
    TestSuffixTree7();
    TestSuffixTree();
    */
    
    cout << "Enter the length of the string: ";
    unsigned int length;
    cin >> length;
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
