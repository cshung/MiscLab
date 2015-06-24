#include "SuffixTree3.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>

SuffixTree3::SuffixTree3() : m_root(new SuffixTree3::SuffixTree3Node())
{

}

SuffixTree3::~SuffixTree3()
{
    delete this->m_root;
}

SuffixTree3::SuffixTree3Node::SuffixTree3Node()
{

}

SuffixTree3::SuffixTree3Node::~SuffixTree3Node()
{
    for (map<char, SuffixTree3::SuffixTree3Edge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

SuffixTree3::SuffixTree3Edge::SuffixTree3Edge() : m_child(nullptr)
{

}

SuffixTree3::SuffixTree3Edge::~SuffixTree3Edge()
{
    if (this->m_child != nullptr)
    {
        delete this->m_child;
    }
}

bool SuffixTree3::Add(const string key)
{
    SuffixTree3::SuffixTree3Node* currentNode = this->m_root;
    unsigned int keyCursor = 0;
    while (keyCursor < key.length())
    {
        char currentCharacter = key[keyCursor];
        map<char, SuffixTree3::SuffixTree3Edge*>::iterator probe = currentNode->m_children.find(currentCharacter);
        if (probe == currentNode->m_children.end())
        {
            SuffixTree3Edge* newEdge = new SuffixTree3Edge();
            SuffixTree3Node* newLeaf = new SuffixTree3Node();
            newEdge->m_edgeLabel = key.substr(keyCursor);
            newEdge->m_child = newLeaf;
            currentNode->m_children.insert(pair<char, SuffixTree3::SuffixTree3Edge*>(currentCharacter, newEdge));
            return true;
        }
        else
        {
            SuffixTree3Edge* currentEdge = probe->second;
            int limit = min(key.length() - keyCursor, currentEdge->m_edgeLabel.length());
            for (int edgeCursor = 0; edgeCursor < limit; keyCursor++, edgeCursor++)
            {
                if (key[keyCursor] != currentEdge->m_edgeLabel[edgeCursor])
                {
                    // Break the current edge, add the data in, and be done with it
                    string edgePrefix = currentEdge->m_edgeLabel.substr(0, edgeCursor);
                    string edgeSuffix = currentEdge->m_edgeLabel.substr(edgeCursor);
                    currentEdge->m_edgeLabel = edgePrefix;

                    SuffixTree3Node* newNode = new SuffixTree3Node();
                    SuffixTree3Edge* oldEdge = new SuffixTree3Edge();
                    SuffixTree3Edge* newEdge = new SuffixTree3Edge();
                    SuffixTree3Node* newLeaf = new SuffixTree3Node();

                    oldEdge->m_edgeLabel = edgeSuffix;
                    oldEdge->m_child = currentEdge->m_child;
                    currentEdge->m_child = newNode;

                    newEdge->m_edgeLabel = key.substr(keyCursor);
                    newEdge->m_child = newLeaf;

                    newNode->m_children.insert(pair<char, SuffixTree3::SuffixTree3Edge*>(edgeSuffix[0], oldEdge));
                    newNode->m_children.insert(pair<char, SuffixTree3::SuffixTree3Edge*>(key[keyCursor], newEdge));

                    return true;
                }
            }
            currentNode = currentEdge->m_child;
        }
    }

    return false;
}

string SuffixTree3::Show() const
{
    list<pair<int, pair<string, int>>> edges;
    queue<pair<int, SuffixTree3::SuffixTree3Node*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, SuffixTree3::SuffixTree3Node*>(0, this->m_root));
    while (bfsQueue.size() > 0)
    {
        pair<int, SuffixTree3::SuffixTree3Node*> current = bfsQueue.front();
        bfsQueue.pop();

        int currentNodeId = current.first;
        SuffixTree3::SuffixTree3Node* currentNode = current.second;
        map<char, SuffixTree3::SuffixTree3Edge*>& currentNodeChildren = currentNode->m_children;
        for (map<char, SuffixTree3::SuffixTree3Edge*>::iterator ci = currentNodeChildren.begin(); ci != currentNodeChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            SuffixTree3::SuffixTree3Edge* nextEdge = ci->second;
            SuffixTree3::SuffixTree3Node* nextNode = nextEdge->m_child;
            bfsQueue.push(pair<int, SuffixTree3::SuffixTree3Node*>(nextNodeId, nextNode));
            edges.push_back(pair<int, pair<string, int>>(currentNodeId, pair<string, int>(nextEdge->m_edgeLabel, nextNodeId)));
        }
    }
    ostringstream stringBuilder;
    stringBuilder << "graph {" << endl;
    for (int i = 0; i <= nodeId; i++)
    {
        stringBuilder << i << "[label = \"\"];" << endl;
    }
    for (list<pair<int, pair<string, int>>>::iterator ei = edges.begin(); ei != edges.end(); ei++)
    {
        stringBuilder << ei->first << "--" << ei->second.second << "[label = \" " << ei->second.first << " \"]" << endl;
    }
    stringBuilder << "}" << endl;
    return stringBuilder.str();
}
