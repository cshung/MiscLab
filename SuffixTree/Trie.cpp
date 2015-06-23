#include "Trie.h"

#include <map>
#include <queue>
#include <list>
#include <sstream>

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
    for (unsigned int keyCursor = 0; keyCursor < key.length(); keyCursor++)
    {
        char currentCharacter = key[keyCursor];
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
    for (unsigned int keyCursor = 0; keyCursor < key.length(); keyCursor++)
    {
        char currentCharacter = key[keyCursor];
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

string Trie::Show() const
{
    list<pair<int, pair<char, int>>> edges;
    queue<pair<int, Trie::TrieNode*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, Trie::TrieNode*>(0, this->m_root));
    while (bfsQueue.size() > 0)
    {
        pair<int, Trie::TrieNode*> current = bfsQueue.front();
        bfsQueue.pop();

        int currentNodeId = current.first;
        Trie::TrieNode* currentNode = current.second;
        map<char, Trie::TrieNode*>& currentNodeChildren = currentNode->m_children;
        for (map<char, Trie::TrieNode*>::iterator ci = currentNodeChildren.begin(); ci != currentNodeChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            char currentEdgeCharacter = ci->first;
            Trie::TrieNode* nextNode = ci->second;
            bfsQueue.push(pair<int, Trie::TrieNode*>(nextNodeId, nextNode));
            edges.push_back(pair<int, pair<char, int>>(currentNodeId, pair<char, int>(currentEdgeCharacter, nextNodeId)));
        }
    }
    ostringstream stringBuilder;
    stringBuilder << "graph {" << endl;
    for (int i = 0; i <= nodeId; i++)
    {
        stringBuilder << i << "[label = \"\"];" << endl;
    }
    for (list<pair<int, pair<char, int>>>::iterator ei = edges.begin(); ei != edges.end(); ei++)
    {
        stringBuilder << ei->first << "--" << ei->second.second << "[label = \" " << ei->second.first << " \"]" << endl;
    }
    stringBuilder << "}" << endl;
    return stringBuilder.str();
}
