#include "SuffixTree2.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>

SuffixTree2::SuffixTree2() : m_root(new SuffixTree2::SuffixTree2Node())
{

}

SuffixTree2::~SuffixTree2()
{
    delete this->m_root;
}

SuffixTree2::SuffixTree2Node::SuffixTree2Node()
{

}

SuffixTree2::SuffixTree2Node::~SuffixTree2Node()
{
    for (map<char, SuffixTree2::SuffixTree2Edge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

SuffixTree2::SuffixTree2Edge::SuffixTree2Edge() : m_child(nullptr)
{

}

SuffixTree2::SuffixTree2Edge::~SuffixTree2Edge()
{
    if (this->m_child != nullptr)
    {
        delete this->m_child;
    }
}

bool SuffixTree2::Add(const string key)
{
    SuffixTree2::SuffixTree2Node* currentNode = this->m_root;
    unsigned int keyCursor = 0;
    while (keyCursor < key.length())
    {
        char currentCharacter = key[keyCursor];
        map<char, SuffixTree2::SuffixTree2Edge*>::iterator probe = currentNode->m_children.find(currentCharacter);
        if (probe == currentNode->m_children.end())
        {
            SuffixTree2Edge* newEdge = new SuffixTree2Edge();
            SuffixTree2Node* newLeaf = new SuffixTree2Node();
            newEdge->m_edgeLabel = key.substr(keyCursor);
            newEdge->m_child = newLeaf;
            currentNode->m_children.insert(pair<char, SuffixTree2::SuffixTree2Edge*>(currentCharacter, newEdge));
            return true;
        }
        else
        {
            SuffixTree2Edge* currentEdge = probe->second;
            int limit = min(key.length() - keyCursor, currentEdge->m_edgeLabel.length());
            for (int edgeCursor = 0; edgeCursor < limit; keyCursor++, edgeCursor++)
            {
                if (key[keyCursor] != currentEdge->m_edgeLabel[edgeCursor])
                {
                    // Break the current edge, add the data in, and be done with it
                    string edgePrefix = currentEdge->m_edgeLabel.substr(0, edgeCursor);
                    string edgeSuffix = currentEdge->m_edgeLabel.substr(edgeCursor);
                    currentEdge->m_edgeLabel = edgePrefix;

                    SuffixTree2Node* newNode = new SuffixTree2Node();
                    SuffixTree2Edge* oldEdge = new SuffixTree2Edge();
                    SuffixTree2Edge* newEdge = new SuffixTree2Edge();
                    SuffixTree2Node* newLeaf = new SuffixTree2Node();

                    oldEdge->m_edgeLabel = edgeSuffix;
                    oldEdge->m_child = currentEdge->m_child;
                    currentEdge->m_child = newNode;

                    newEdge->m_edgeLabel = key.substr(keyCursor);
                    newEdge->m_child = newLeaf;

                    newNode->m_children.insert(pair<char, SuffixTree2::SuffixTree2Edge*>(edgeSuffix[0], oldEdge));
                    newNode->m_children.insert(pair<char, SuffixTree2::SuffixTree2Edge*>(key[keyCursor], newEdge));

                    return true;
                }
            }
            currentNode = currentEdge->m_child;
        }
    }

    return false;
}

string SuffixTree2::Show() const
{
    list<pair<int, pair<string, int>>> edges;
    queue<pair<int, SuffixTree2::SuffixTree2Node*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, SuffixTree2::SuffixTree2Node*>(0, this->m_root));
    while (bfsQueue.size() > 0)
    {
        pair<int, SuffixTree2::SuffixTree2Node*> current = bfsQueue.front();
        bfsQueue.pop();

        int currentNodeId = current.first;
        SuffixTree2::SuffixTree2Node* currentNode = current.second;
        map<char, SuffixTree2::SuffixTree2Edge*>& currentNodeChildren = currentNode->m_children;
        for (map<char, SuffixTree2::SuffixTree2Edge*>::iterator ci = currentNodeChildren.begin(); ci != currentNodeChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            SuffixTree2::SuffixTree2Edge* nextEdge = ci->second;
            SuffixTree2::SuffixTree2Node* nextNode = nextEdge->m_child;
            bfsQueue.push(pair<int, SuffixTree2::SuffixTree2Node*>(nextNodeId, nextNode));
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
