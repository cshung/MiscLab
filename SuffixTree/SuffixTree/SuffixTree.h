#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTreeBuilder;

/*
 * First extension always go to the first leaf
 */
class SuffixTree
{
public:
    SuffixTree();
    ~SuffixTree();
    bool  Add(int keyBegin, int keyEnd, SuffixTreeBuilder* suffixTreeBuilder);
    string Show(string& input) const;
private:
    class SuffixTreeEdge
    {
    public:
        SuffixTreeEdge();
        ~SuffixTreeEdge();
        unsigned int end();
        unsigned int m_begin;
        unsigned int m_end;
    public:
        unsigned int length();
        SuffixTreeEdge* m_suffixLink;
        map<char, SuffixTreeEdge*> m_children;
        SuffixTreeBuilder* m_builder;
    };

    SuffixTreeEdge* m_root;

    friend class SuffixTreeBuilder;
};

