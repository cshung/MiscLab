#include "SuffixTree1.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>

SuffixTree1::SuffixTree1() : m_root(new SuffixTree1::SuffixTree1Node())
{

}

SuffixTree1::~SuffixTree1()
{
    delete this->m_root;
}

SuffixTree1::SuffixTree1Node::SuffixTree1Node()
{

}

SuffixTree1::SuffixTree1Node::~SuffixTree1Node()
{
    for (map<char, SuffixTree1::SuffixTree1Edge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

SuffixTree1::SuffixTree1Edge::SuffixTree1Edge() : m_child(nullptr)
{

}

SuffixTree1::SuffixTree1Edge::~SuffixTree1Edge()
{
    if (this->m_child != nullptr)
    {
        delete this->m_child;
    }
}

bool SuffixTree1::Add(const string key)
{
    SuffixTree1::SuffixTree1Node* currentNode = this->m_root;
    unsigned int keyCursor = 0;
    while (keyCursor < key.length())
    {
        char currentCharacter = key[keyCursor];
        map<char, SuffixTree1::SuffixTree1Edge*>::iterator probe = currentNode->m_children.find(currentCharacter);
        if (probe == currentNode->m_children.end())
        {
            SuffixTree1Edge* newEdge = new SuffixTree1Edge();
            SuffixTree1Node* newLeaf = new SuffixTree1Node();
            newEdge->m_edgeLabel = key.substr(keyCursor);
            newEdge->m_child = newLeaf;
            currentNode->m_children.insert(pair<char, SuffixTree1::SuffixTree1Edge*>(currentCharacter, newEdge));
            return true;
        }
        else
        {
            SuffixTree1Edge* currentEdge = probe->second;
            int limit = min(key.length() - keyCursor, currentEdge->m_edgeLabel.length());
            for (int edgeCursor = 0; edgeCursor < limit; keyCursor++, edgeCursor++)
            {
                if (key[keyCursor] != currentEdge->m_edgeLabel[edgeCursor])
                {
                    // Break the current edge, add the data in, and be done with it
                    string edgePrefix = currentEdge->m_edgeLabel.substr(0, edgeCursor);
                    string edgeSuffix = currentEdge->m_edgeLabel.substr(edgeCursor);
                    currentEdge->m_edgeLabel = edgePrefix;

                    SuffixTree1Node* newNode = new SuffixTree1Node();
                    SuffixTree1Edge* oldEdge = new SuffixTree1Edge();
                    SuffixTree1Edge* newEdge = new SuffixTree1Edge();
                    SuffixTree1Node* newLeaf = new SuffixTree1Node();

                    oldEdge->m_edgeLabel = edgeSuffix;
                    oldEdge->m_child = currentEdge->m_child;
                    currentEdge->m_child = newNode;

                    newEdge->m_edgeLabel = key.substr(keyCursor);
                    newEdge->m_child = newLeaf;

                    newNode->m_children.insert(pair<char, SuffixTree1::SuffixTree1Edge*>(edgeSuffix[0], oldEdge));
                    newNode->m_children.insert(pair<char, SuffixTree1::SuffixTree1Edge*>(key[keyCursor], newEdge));

                    return true;
                }
            }
            currentNode = currentEdge->m_child;
        }
    }

    return false;
}

string SuffixTree1::Show() const
{
    list<pair<int, pair<string, int>>> edges;
    queue<pair<int, SuffixTree1::SuffixTree1Node*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, SuffixTree1::SuffixTree1Node*>(0, this->m_root));
    while (bfsQueue.size() > 0)
    {
        pair<int, SuffixTree1::SuffixTree1Node*> current = bfsQueue.front();
        bfsQueue.pop();

        int currentNodeId = current.first;
        SuffixTree1::SuffixTree1Node* currentNode = current.second;
        map<char, SuffixTree1::SuffixTree1Edge*>& currentNodeChildren = currentNode->m_children;
        for (map<char, SuffixTree1::SuffixTree1Edge*>::iterator ci = currentNodeChildren.begin(); ci != currentNodeChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            SuffixTree1::SuffixTree1Edge* nextEdge = ci->second;
            SuffixTree1::SuffixTree1Node* nextNode = nextEdge->m_child;
            bfsQueue.push(pair<int, SuffixTree1::SuffixTree1Node*>(nextNodeId, nextNode));
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
