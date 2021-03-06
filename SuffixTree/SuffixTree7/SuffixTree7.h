#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree7Builder;

/*
 * First extension always go to the first leaf
 */
class SuffixTree7
{
public:
    SuffixTree7();
    ~SuffixTree7();
    bool  Add(int keyBegin, int keyEnd, SuffixTree7Builder* suffixTreeBuilder);
    string Show(string& input) const;
private:
    class SuffixTree7Edge
    {
    public:
        SuffixTree7Edge();
        ~SuffixTree7Edge();
        unsigned int m_begin;
        unsigned int m_end;
        unsigned int length();
        SuffixTree7Edge* m_suffixLink;
        map<char, SuffixTree7Edge*> m_children;
    };

    SuffixTree7Edge* m_root;

    friend class SuffixTree7Builder;

    static const bool verify;
};

