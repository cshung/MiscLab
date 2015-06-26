#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree5Builder;

/*
* Book keeping the string depths
*/
class SuffixTree5
{
public:
    SuffixTree5();
    ~SuffixTree5();
    bool  Add(const string key, SuffixTree5Builder* suffixTreeBuilder);
    string Show() const;
private:
    class SuffixTree5Edge
    {
    public:
        SuffixTree5Edge();
        ~SuffixTree5Edge();
        string m_edgeLabel;
        SuffixTree5Edge* m_suffixLink;
        map<char, SuffixTree5Edge*> m_children;
        int m_stringDepth;
    };

    SuffixTree5Edge* m_root;

    friend class SuffixTree5Builder;
};

