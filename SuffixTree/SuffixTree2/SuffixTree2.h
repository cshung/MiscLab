#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree2
{
public:
    SuffixTree2();
    ~SuffixTree2();
    bool  Add(const string key);
    string Show() const;
private:
    class SuffixTree2Edge;

    class SuffixTree2Node
    {
    public:
        SuffixTree2Node();
        ~SuffixTree2Node();
        map<char, SuffixTree2Edge*> m_children;
    };

    class SuffixTree2Edge
    {
    public:
        SuffixTree2Edge();
        ~SuffixTree2Edge();
        string m_edgeLabel;
        SuffixTree2Node* m_child;
    };

    SuffixTree2Node* m_root;
};
