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
    string Show(string& input, SuffixTreeBuilder* suffixTreeBuilder) const;
private:
    class SuffixTreeEdge
    {
    public:
        SuffixTreeEdge();
        ~SuffixTreeEdge();
        unsigned int end(SuffixTree::SuffixTreeEdge* root, SuffixTreeBuilder* suffixTreeBuilder);
        unsigned int m_begin;
        unsigned int m_end;
    public:
        unsigned int length(SuffixTree::SuffixTreeEdge* root, SuffixTreeBuilder* suffixTreeBuilder);
        SuffixTreeEdge* m_suffixLink;
        map<char, SuffixTreeEdge*> m_children;
    };

    SuffixTreeEdge* m_root;

    friend class SuffixTreeBuilder;
};

