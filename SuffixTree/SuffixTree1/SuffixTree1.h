#pragma once

#include <map>
#include <string>
using namespace std;

/*
 * A very primitive suffix tree that works.
 * This version just insert all the suffix in a compressed trie
 * It runs in quadratic time, and it serves as a baseline for us 
 * to check correctness for advanced algorithms.
 */
class SuffixTree1
{
public:
    SuffixTree1();
    ~SuffixTree1();
    bool  Add(const string key);
    string Show() const;
private:
    class SuffixTree1Edge;

    class SuffixTree1Node
    {
    public:
        SuffixTree1Node();
        ~SuffixTree1Node();
        map<char, SuffixTree1Edge*> m_children;
    };

    class SuffixTree1Edge
    {
    public:
        SuffixTree1Edge();
        ~SuffixTree1Edge();
        string m_edgeLabel;
        SuffixTree1Node* m_child;
    };

    SuffixTree1Node* m_root;
};

