#pragma once

#include <map>
#include <string>
using namespace std;

class CompressedTrie
{
public:
    CompressedTrie();
    ~CompressedTrie();
    bool  Add(const string key, void* value);
    void* Get(const string key) const;
    string Show() const;
private:
    class CompressedTrieEdge;

    class CompressedTrieNode
    {
    public:
        CompressedTrieNode();
        ~CompressedTrieNode();
        map<char, CompressedTrieEdge*> m_children;
        void* m_value;
    };

    class CompressedTrieEdge
    {
    public:
        CompressedTrieEdge();
        ~CompressedTrieEdge();
        string m_edgeLabel;
        CompressedTrieNode* m_child;
    };

    CompressedTrieNode* m_root;
};

