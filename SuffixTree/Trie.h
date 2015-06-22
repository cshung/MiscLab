#pragma once

#include <map>
using namespace std;

class Trie
{
public:
    Trie();
    ~Trie();
    bool  Add(const string key, void* value);
    void* Get(const string key) const;
private:

    class TrieNode
    {
    public:
        TrieNode();
        ~TrieNode();
        map<char, TrieNode*> m_children;
        void* m_value;
    };
    TrieNode* m_root;
};
