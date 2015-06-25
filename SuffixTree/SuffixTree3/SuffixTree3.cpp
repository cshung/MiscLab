#include "SuffixTree3.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>

SuffixTree3::SuffixTree3() : m_root(new SuffixTree3::SuffixTree3Edge())
{
    this->m_root->m_edgeLabel = "";
}

SuffixTree3::~SuffixTree3()
{
    delete this->m_root;
}

SuffixTree3::SuffixTree3Edge::SuffixTree3Edge()
{

}

SuffixTree3::SuffixTree3Edge::~SuffixTree3Edge()
{
    for (map<char, SuffixTree3::SuffixTree3Edge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

bool SuffixTree3::Add(const string key)
{
    SuffixTree3::SuffixTree3Edge* treeCursor = this->m_root;
    unsigned int treeEdgeCursor = 0;
    unsigned int keyCursor = 0;
    unsigned int searchKeyLength = key.length() - 1;

    // Step 1: Search for the string except the last character
    while (keyCursor < searchKeyLength)
    {
        char currentCharacter = key[keyCursor];
        if (treeEdgeCursor == treeCursor->m_edgeLabel.length())
        {
            map<char, SuffixTree3::SuffixTree3Edge*>::iterator probe = treeCursor->m_children.find(currentCharacter);
            if (probe == treeCursor->m_children.end())
            {
                assert(false); // The string except the last character should always in the tree"
            }
            else
            {
                SuffixTree3Edge* currentEdge = probe->second;                
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
    if (treeEdgeCursor == treeCursor->m_edgeLabel.length())
    {
        // We end up at a node
        if (treeCursor == this->m_root)
        {
            // I have no choice but to branch out for the root node
            SuffixTree3::SuffixTree3Edge* newEdge = new SuffixTree3::SuffixTree3Edge();
            newEdge->m_edgeLabel = key;
            this->m_root->m_children.insert(pair<char, SuffixTree3::SuffixTree3Edge*>(key[0], newEdge));
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
                map<char, SuffixTree3::SuffixTree3Edge*>::iterator probe = treeCursor->m_children.find(characterToExtend);
                if (probe == treeCursor->m_children.end())
                {
                    // We have reached a non-leaf node - and the tree does not extend with our character
                    // Therefore we will apply the split rule
                    SuffixTree3::SuffixTree3Edge* newEdge = new SuffixTree3::SuffixTree3Edge();
                    newEdge->m_edgeLabel = characterToExtend;
                    treeCursor->m_children.insert(pair<char, SuffixTree3::SuffixTree3Edge*>(characterToExtend, newEdge));
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
            SuffixTree3::SuffixTree3Edge* oldEdge = new SuffixTree3::SuffixTree3Edge();
            SuffixTree3::SuffixTree3Edge* newEdge = new SuffixTree3::SuffixTree3Edge();
            oldEdge->m_edgeLabel = treeCursor->m_edgeLabel.substr(treeEdgeCursor);
            treeCursor->m_edgeLabel = treeCursor->m_edgeLabel.substr(0, treeEdgeCursor);
            newEdge->m_edgeLabel = characterToExtend;

            for (map<char, SuffixTree3Edge*>::iterator ci = treeCursor->m_children.begin(); ci != treeCursor->m_children.end(); ci++)
            {
                oldEdge->m_children.insert(*ci);
            }

            treeCursor->m_children.clear();
            treeCursor->m_children.insert(pair<char, SuffixTree3::SuffixTree3Edge*>(oldEdge->m_edgeLabel[0], oldEdge));
            treeCursor->m_children.insert(pair<char, SuffixTree3::SuffixTree3Edge*>(characterToExtend, newEdge));
        }
    }
    
    return false;
}

string SuffixTree3::Show() const
{
    list<pair<int, pair<string, int>>> edges;
    queue<pair<int, SuffixTree3::SuffixTree3Edge*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, SuffixTree3::SuffixTree3Edge*>(0, this->m_root));
    while (bfsQueue.size() > 0)
    {
        pair<int, SuffixTree3::SuffixTree3Edge*> current = bfsQueue.front();
        bfsQueue.pop();

        int treeCursorId = current.first;
        SuffixTree3::SuffixTree3Edge* treeCursor = current.second;
        map<char, SuffixTree3::SuffixTree3Edge*>& treeCursorChildren = treeCursor->m_children;
        for (map<char, SuffixTree3::SuffixTree3Edge*>::iterator ci = treeCursorChildren.begin(); ci != treeCursorChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            SuffixTree3::SuffixTree3Edge* nextEdge = ci->second;
            bfsQueue.push(pair<int, SuffixTree3::SuffixTree3Edge*>(nextNodeId, nextEdge));
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
