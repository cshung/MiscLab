#pragma once

#include <map>
#include <string>
using namespace std;

/*
 * Fixing the issue that the a node is created for all
 * characters, merging an edge and its outgoing node to 
 * save memory as well. Significant change from the 
 * previous version.
 */
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

