#include "CompressedTrie.h"
#include <algorithm>

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
    CompressedTrie::CompressedTrieNode* currentNode = this->m_root;
    unsigned int keyCursor = 0;
    while (keyCursor < key.length())
    {
        char currentCharacter = key[keyCursor];
        map<char, CompressedTrie::CompressedTrieEdge*>::iterator probe = currentNode->m_children.find(currentCharacter);
        if (probe == currentNode->m_children.end())
        {
            CompressedTrieEdge* newEdge = new CompressedTrieEdge();
            CompressedTrieNode* newLeaf = new CompressedTrieNode();
            newEdge->m_edgeLabel = key.substr(keyCursor);
            newEdge->m_child = newLeaf;
            newLeaf->m_value = value;
            currentNode->m_children.insert(pair<char, CompressedTrie::CompressedTrieEdge*>(currentCharacter, newEdge));
            return true;
        }
        else
        {
            CompressedTrieEdge* currentEdge = probe->second;
            int limit = min(key.length() - keyCursor, currentEdge->m_edgeLabel.length());
            for (int edgeCursor = 0; edgeCursor < limit; keyCursor++, edgeCursor++)
            {
                if (key[keyCursor] != currentEdge->m_edgeLabel[edgeCursor])
                {
                    // Break the current edge, add the data in, and be done with it
                    string edgePrefix = currentEdge->m_edgeLabel.substr(0, edgeCursor);
                    string edgeSuffix = currentEdge->m_edgeLabel.substr(edgeCursor);
                    currentEdge->m_edgeLabel = edgePrefix;

                    CompressedTrieNode* newNode = new CompressedTrieNode();
                    CompressedTrieEdge* oldEdge = new CompressedTrieEdge();
                    CompressedTrieEdge* newEdge = new CompressedTrieEdge();
                    CompressedTrieNode* newLeaf = new CompressedTrieNode();

                    oldEdge->m_edgeLabel = edgeSuffix;
                    oldEdge->m_child = currentEdge->m_child;
                    currentEdge->m_child = newNode;

                    newEdge->m_edgeLabel = key.substr(keyCursor);
                    newEdge->m_child = newLeaf;

                    newNode->m_children.insert(pair<char, CompressedTrie::CompressedTrieEdge*>(edgeSuffix[0], oldEdge));
                    newNode->m_children.insert(pair<char, CompressedTrie::CompressedTrieEdge*>(key[keyCursor], newEdge));

                    newLeaf->m_value = value;
                    return true;
                }
            }
            currentNode = currentEdge->m_child;
        }
    }

    return false;
}

void* CompressedTrie::Get(const string key) const
{
    CompressedTrie::CompressedTrieNode* currentNode = this->m_root;
    unsigned int keyCursor = 0;
    while (keyCursor < key.length())
    {
        char currentCharacter = key[keyCursor];
        map<char, CompressedTrie::CompressedTrieEdge*>::iterator probe = currentNode->m_children.find(currentCharacter);
        if (probe == currentNode->m_children.end())
        {
            return nullptr;
        }
        else
        {
            CompressedTrieEdge* currentEdge = probe->second;
            int limit = min(key.length() - keyCursor, currentEdge->m_edgeLabel.length());
            for (int edgeCursor = 0; edgeCursor < limit; keyCursor++, edgeCursor++)
            {
                if (key[keyCursor] != currentEdge->m_edgeLabel[edgeCursor])
                {
                    return nullptr;
                }
            }
            currentNode = currentEdge->m_child;
        }
    }
    return currentNode->m_value;
}