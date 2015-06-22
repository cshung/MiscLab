#include "CompressedTrie.h"

CompressedTrie::CompressedTrie() : m_root(new CompressedTrie::CompressedTrieNode())
{
    
}

CompressedTrie::~CompressedTrie()
{
    delete this->m_root;
}

CompressedTrie::CompressedTrieNode::CompressedTrieNode() : m_value(nullptr)
{

}

CompressedTrie::CompressedTrieNode::~CompressedTrieNode()
{
    for (map<char, CompressedTrie::CompressedTrieEdge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

CompressedTrie::CompressedTrieEdge::CompressedTrieEdge() : m_child(nullptr)
{

}

CompressedTrie::CompressedTrieEdge::~CompressedTrieEdge()
{
    if (this->m_child != nullptr)
    {
        delete this->m_child;
    }
}

bool CompressedTrie::Add(const string key, void* value)
{
    return false;
}

void* CompressedTrie::Get(const string key) const
{
    return nullptr;
}