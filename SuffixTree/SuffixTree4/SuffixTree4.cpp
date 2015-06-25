#include "SuffixTree4.h"
#include "SuffixTree4Builder.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>

SuffixTree4::SuffixTree4() : m_root(new SuffixTree4::SuffixTree4Edge())
{
    this->m_root->m_edgeLabel = "";
}

SuffixTree4::~SuffixTree4()
{
    delete this->m_root;
}

SuffixTree4::SuffixTree4Edge::SuffixTree4Edge() : m_suffixLink(nullptr)
{

}

SuffixTree4::SuffixTree4Edge::~SuffixTree4Edge()
{
    for (map<char, SuffixTree4::SuffixTree4Edge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

bool SuffixTree4::Add(const string key, SuffixTree4Builder* builder)
{
#ifdef _DEBUG
    builder->m_extensionCount++;
#endif
    SuffixTree4::SuffixTree4Edge* treeCursor = this->m_root;
    unsigned int treeEdgeCursor = 0;
    unsigned int keyCursor = 0;
    unsigned int searchKeyLength = key.length() - 1;

    // Step 1: Search for the string except the last character
    while (keyCursor < searchKeyLength)
    {
        char currentCharacter = key[keyCursor];
        if (treeEdgeCursor == treeCursor->m_edgeLabel.length())
        {
            map<char, SuffixTree4::SuffixTree4Edge*>::iterator probe = treeCursor->m_children.find(currentCharacter);
            if (probe == treeCursor->m_children.end())
            {
                assert(false); // The string except the last character should always in the tree"
            }
            else
            {
                SuffixTree4Edge* currentEdge = probe->second;                
                treeCursor = currentEdge;
                treeEdgeCursor = 0;
            }
        }
        else
        {
            if (key[keyCursor] != treeCursor->m_edgeLabel[treeEdgeCursor])
            {
                assert(false); // The string except the last character should always in the tree"
            }
            keyCursor++;
            treeEdgeCursor++;
        }
    }

    // Step 2: Insert the character in the tree
    char characterToExtend = key[key.length() - 1];

    SuffixTree4::SuffixTree4Edge* newInternalNode = nullptr;
    if (treeEdgeCursor == treeCursor->m_edgeLabel.length())
    {
        // We end up at a node
        if (treeCursor == this->m_root)
        {
            // I have no choice but to branch out for the root node
            SuffixTree4::SuffixTree4Edge* newEdge = new SuffixTree4::SuffixTree4Edge();
            newEdge->m_edgeLabel = key;
            this->m_root->m_children.insert(pair<char, SuffixTree4::SuffixTree4Edge*>(key[0], newEdge));
        }
        else
        {
            if (treeCursor->m_children.size() == 0)
            {
                // We have reached a leaf - and therefore we will apply the leaf extension rule
                treeCursor->m_edgeLabel += characterToExtend;
            }
            else
            {
                map<char, SuffixTree4::SuffixTree4Edge*>::iterator probe = treeCursor->m_children.find(characterToExtend);
                if (probe == treeCursor->m_children.end())
                {
                    // We have reached a non-leaf node - and the tree does not extend with our character
                    // Therefore we will apply the split rule
                    SuffixTree4::SuffixTree4Edge* newEdge = new SuffixTree4::SuffixTree4Edge();
                    newEdge->m_edgeLabel = characterToExtend;
                    treeCursor->m_children.insert(pair<char, SuffixTree4::SuffixTree4Edge*>(characterToExtend, newEdge));
                }
                else
                {
                    // We have reached a non-leaf node - and the tree extends with our character
                    // Therefore we will apply the no-op rule
                }
            }
        }
    }
    else
    {
        // We end up at an edge
        if (treeCursor->m_edgeLabel[treeEdgeCursor] == characterToExtend)
        {
            // We have reach the middle of an edge, and the edge extends with our character
            // Therefore we will apply the no-op rule
        }
        else
        {
            // We have reach the middle of an edge, and the edge does not extend with our character
            // Therefore we will apply the split rule
            SuffixTree4::SuffixTree4Edge* oldEdge = new SuffixTree4::SuffixTree4Edge();
            SuffixTree4::SuffixTree4Edge* newEdge = new SuffixTree4::SuffixTree4Edge();
            oldEdge->m_edgeLabel = treeCursor->m_edgeLabel.substr(treeEdgeCursor);
            treeCursor->m_edgeLabel = treeCursor->m_edgeLabel.substr(0, treeEdgeCursor);
            newEdge->m_edgeLabel = characterToExtend;

            for (map<char, SuffixTree4Edge*>::iterator ci = treeCursor->m_children.begin(); ci != treeCursor->m_children.end(); ci++)
            {
                oldEdge->m_children.insert(*ci);
            }

            newInternalNode = treeCursor;

            treeCursor->m_children.clear();
            treeCursor->m_children.insert(pair<char, SuffixTree4::SuffixTree4Edge*>(oldEdge->m_edgeLabel[0], oldEdge));
            treeCursor->m_children.insert(pair<char, SuffixTree4::SuffixTree4Edge*>(characterToExtend, newEdge));
        }
    }

    if (builder->m_lastInternalNode != nullptr)
    {
        builder->m_lastInternalNode->m_suffixLink = treeCursor;
        builder->m_lastInternalNode = nullptr;
    }

    if (newInternalNode != nullptr)
    {
#ifdef _DEBUG
        builder->m_lastInternalNodeBirthday = builder->m_extensionCount;
#endif
        builder->m_lastInternalNode = treeCursor;
    }
    
    return false;
}

string SuffixTree4::Show() const
{
    list<pair<int, pair<string, int>>> edges;
    queue<pair<int, SuffixTree4::SuffixTree4Edge*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, SuffixTree4::SuffixTree4Edge*>(0, this->m_root));
    while (bfsQueue.size() > 0)
    {
        pair<int, SuffixTree4::SuffixTree4Edge*> current = bfsQueue.front();
        bfsQueue.pop();

        int treeCursorId = current.first;
        SuffixTree4::SuffixTree4Edge* treeCursor = current.second;
        map<char, SuffixTree4::SuffixTree4Edge*>& treeCursorChildren = treeCursor->m_children;
        for (map<char, SuffixTree4::SuffixTree4Edge*>::iterator ci = treeCursorChildren.begin(); ci != treeCursorChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            SuffixTree4::SuffixTree4Edge* nextEdge = ci->second;
            bfsQueue.push(pair<int, SuffixTree4::SuffixTree4Edge*>(nextNodeId, nextEdge));
            edges.push_back(pair<int, pair<string, int>>(treeCursorId, pair<string, int>(nextEdge->m_edgeLabel, nextNodeId)));
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
