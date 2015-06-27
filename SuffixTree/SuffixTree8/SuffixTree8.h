#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree8Builder;

/*
 * First extension always go to the first leaf
 */
class SuffixTree8
{
public:
    SuffixTree8();
    ~SuffixTree8();
    bool  Add(int keyBegin, int keyEnd, SuffixTree8Builder* suffixTreeBuilder);
    string Show(string& input) const;
private:
    class SuffixTree8Edge
    {
    public:
        SuffixTree8Edge();
        ~SuffixTree8Edge();
        unsigned int m_begin;
        unsigned int m_end;
        unsigned int length();
        SuffixTree8Edge* m_suffixLink;
        map<char, SuffixTree8Edge*> m_children;
        int m_stringDepth;
    };

    SuffixTree8Edge* m_root;

    friend class SuffixTree8Builder;
};

