#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree3
{
public:
    SuffixTree3();
    ~SuffixTree3();
    bool  Add(const string key);
    string Show() const;
private:
    class SuffixTree3Edge;

    class SuffixTree3Node
    {
    public:
        SuffixTree3Node();
        ~SuffixTree3Node();
        map<char, SuffixTree3Edge*> m_children;
    };

    class SuffixTree3Edge
    {
    public:
        SuffixTree3Edge();
        ~SuffixTree3Edge();
        string m_edgeLabel;
        SuffixTree3Node* m_child;
    };

    SuffixTree3Node* m_root;
};

