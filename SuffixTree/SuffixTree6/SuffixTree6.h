#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree6Builder;

/*
 * Implementation Trick 1 - Skip Count Trick
 */
class SuffixTree6
{
public:
    SuffixTree6();
    ~SuffixTree6();
    bool  Add(int keyBegin, int keyEnd, SuffixTree6Builder* suffixTreeBuilder);
    string Show(string& input) const;
private:
    class SuffixTree6Edge
    {
    public:
        SuffixTree6Edge();
        ~SuffixTree6Edge();
        unsigned int m_begin;
        unsigned int m_end;
        unsigned int length();
        SuffixTree6Edge* m_suffixLink;
        map<char, SuffixTree6Edge*> m_children;
    };

    SuffixTree6Edge* m_root;

    friend class SuffixTree6Builder;
};

