#pragma once

#include <map>
#include <string>
using namespace std;

class SuffixTree3
{
public:
    SuffixTree3();
    ~SuffixTree3();
    bool  Add(const string key);
    string Show() const;
private:
    class SuffixTree3Edge
    {
    public:
        SuffixTree3Edge();
        ~SuffixTree3Edge();
        string m_edgeLabel;
        map<char, SuffixTree3Edge*> m_children;
    };

    SuffixTree3Edge* m_root;
};

