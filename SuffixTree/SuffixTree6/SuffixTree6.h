#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree6Builder;

/*
 * Using the suffix links to reduce time
 */
class SuffixTree6
{
public:
    SuffixTree6();
    ~SuffixTree6();
    bool  Add(const string key, SuffixTree6Builder* suffixTreeBuilder);
    string Show() const;
private:
    class SuffixTree6Edge
    {
    public:
        SuffixTree6Edge();
        ~SuffixTree6Edge();
        string m_edgeLabel;
        SuffixTree6Edge* m_suffixLink;
        map<char, SuffixTree6Edge*> m_children;
        int m_stringDepth;
    };

    SuffixTree6Edge* m_root;

    friend class SuffixTree6Builder;
};

