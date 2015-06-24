#pragma once

#include <map>
#include <string>
using namespace std;

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

