#include "Trie.h"

#include <map>
using namespace std;

Trie::Trie() : m_root(new Trie::TrieNode())
{
}

Trie::~Trie()
{
    delete this->m_root;
}

bool Trie::Add(const string key, void* value)
{
    Trie::TrieNode* current = this->m_root;
    for (unsigned int i = 0; i < key.length(); i++)
    {
        char currentCharacter = key[i];
        map<char, Trie::TrieNode*>::iterator probe = current->m_children.find(currentCharacter);
        if (probe == current->m_children.end())
        {
            Trie::TrieNode* newNode = new Trie::TrieNode();
            current->m_children.insert(pair<char, Trie::TrieNode*>(currentCharacter, newNode));
            current = newNode;
        }
        else
        {
            current = probe->second;
        }
    }
    if (current->m_value == nullptr)
    {
        current->m_value = value;
        return true;
    }
    else
    {
        return false;
    }
}

void* Trie::Get(const string key) const
{
    Trie::TrieNode* current = this->m_root;
    for (unsigned int i = 0; i < key.length(); i++)
    {
        char currentCharacter = key[i];
        map<char, Trie::TrieNode*>::iterator probe = current->m_children.find(currentCharacter);
        if (probe == current->m_children.end())
        {
            return nullptr;
        }
        else
        {
            current = probe->second;
        }
    }
    return current->m_value;
}

Trie::TrieNode::TrieNode() : m_value(nullptr)
{

}

Trie::TrieNode::~TrieNode()
{
    for (map<char, Trie::TrieNode*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}