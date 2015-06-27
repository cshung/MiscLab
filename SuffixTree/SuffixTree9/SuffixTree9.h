#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree9Builder;

/*
 * First extension always go to the first leaf
 */
class SuffixTree9
{
public:
    SuffixTree9();
    ~SuffixTree9();
    bool  Add(int keyBegin, int keyEnd, SuffixTree9Builder* suffixTreeBuilder);
    string Show(string& input) const;
private:
    class SuffixTree9Edge
    {
    public:
        SuffixTree9Edge();
        ~SuffixTree9Edge();
        unsigned int m_begin;
        unsigned int m_end;
        unsigned int length();
        SuffixTree9Edge* m_suffixLink;
        map<char, SuffixTree9Edge*> m_children;
        int m_stringDepth;
    };

    SuffixTree9Edge* m_root;

    friend class SuffixTree9Builder;
};

