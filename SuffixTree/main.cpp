#include "Trie.h"
#include <iostream>
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
}

int main()
{
    TestTrie();
    return 0;
}