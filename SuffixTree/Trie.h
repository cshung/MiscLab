#pragma once

#include <map>
using namespace std;

class Trie
{
public:
    Trie();
    ~Trie();
    bool  Add(string key, void* value);
    void* Get(string key);
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

