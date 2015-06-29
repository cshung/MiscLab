#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree4Builder;

/*
 * Building the suffix links
 */
class SuffixTree4
{
public:
    SuffixTree4();
    ~SuffixTree4();
    void  Add(const string key, SuffixTree4Builder* suffixTreeBuilder);
    string Show() const;
private:
    class SuffixTree4Edge
    {
    public:
        SuffixTree4Edge();
        ~SuffixTree4Edge();
        string m_edgeLabel;
        SuffixTree4Edge* m_suffixLink;
        map<char, SuffixTree4Edge*> m_children;
    };

    SuffixTree4Edge* m_root;

    friend class SuffixTree4Builder;
};

