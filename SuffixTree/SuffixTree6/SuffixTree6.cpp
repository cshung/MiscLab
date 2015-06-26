#include "SuffixTree6.h"
#include "SuffixTree6Builder.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>

SuffixTree6::SuffixTree6() : m_root(new SuffixTree6::SuffixTree6Edge())
{
    this->m_root->m_stringDepth = 0;
    this->m_root->m_edgeLabel = "";
}

SuffixTree6::~SuffixTree6()
{
    delete this->m_root;
}

SuffixTree6::SuffixTree6Edge::SuffixTree6Edge() : m_suffixLink(nullptr), m_stringDepth(0)
{

}

SuffixTree6::SuffixTree6Edge::~SuffixTree6Edge()
{
    for (map<char, SuffixTree6::SuffixTree6Edge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

bool SuffixTree6::Add(const string key, SuffixTree6Builder* builder)
{
#ifdef _DEBUG
    builder->m_extensionCount++;
#endif
    // Step 0: Use the suffix link to speed up the search
    SuffixTree6::SuffixTree6Edge* treeCursor = builder->m_nextStart;
    unsigned int treeEdgeCursor = treeCursor->m_edgeLabel.length();
    unsigned int keyCursor = treeCursor->m_stringDepth;;
    unsigned int searchKeyLength = key.length() - 1;

    builder->m_nextStart = this->m_root;

    // Step 1: Search for the string except the last character
    while (keyCursor < searchKeyLength)
    {
        char currentCharacter = key[keyCursor];
        if (treeEdgeCursor == treeCursor->m_edgeLabel.length())
        {
            map<char, SuffixTree6::SuffixTree6Edge*>::iterator probe = treeCursor->m_children.find(currentCharacter);
            if (probe == treeCursor->m_children.end())
            {
                assert(false); // The string except the last character should always in the tree"
            }
            else
            {
                SuffixTree6Edge* currentEdge = probe->second;                
                treeCursor = currentEdge;
                treeEdgeCursor = 0;
                if (currentEdge->m_suffixLink)
                {
                    builder->m_nextStart = currentEdge->m_suffixLink;
                }
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

    SuffixTree6::SuffixTree6Edge* newInternalNode = nullptr;
    if (treeEdgeCursor == treeCursor->m_edgeLabel.length())
    {
        // We end up at a node
        if (treeCursor == this->m_root)
        {
            // I have no choice but to branch out for the root node
            SuffixTree6::SuffixTree6Edge* newEdge = new SuffixTree6::SuffixTree6Edge();
            newEdge->m_edgeLabel = key;
            newEdge->m_stringDepth = 1;
            this->m_root->m_children.insert(pair<char, SuffixTree6::SuffixTree6Edge*>(key[0], newEdge));
        }
        else
        {
            if (treeCursor->m_children.size() == 0)
            {
                // We have reached a leaf - and therefore we will apply the leaf extension rule
                treeCursor->m_edgeLabel += characterToExtend;
                treeCursor->m_stringDepth++;
            }
            else
            {
                map<char, SuffixTree6::SuffixTree6Edge*>::iterator probe = treeCursor->m_children.find(characterToExtend);
                if (probe == treeCursor->m_children.end())
                {
                    // We have reached a non-leaf node - and the tree does not extend with our character
                    // Therefore we will apply the split rule
                    SuffixTree6::SuffixTree6Edge* newEdge = new SuffixTree6::SuffixTree6Edge();
                    newEdge->m_edgeLabel = characterToExtend;
                    treeCursor->m_children.insert(pair<char, SuffixTree6::SuffixTree6Edge*>(characterToExtend, newEdge));
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
            SuffixTree6::SuffixTree6Edge* oldEdge = new SuffixTree6::SuffixTree6Edge();
            SuffixTree6::SuffixTree6Edge* newEdge = new SuffixTree6::SuffixTree6Edge();
            int originalDepth = treeCursor->m_stringDepth;
            int originalLength = treeCursor->m_edgeLabel.length();
            
            oldEdge->m_edgeLabel = treeCursor->m_edgeLabel.substr(treeEdgeCursor);
            oldEdge->m_stringDepth = originalDepth;

            treeCursor->m_edgeLabel = treeCursor->m_edgeLabel.substr(0, treeEdgeCursor);
            treeCursor->m_stringDepth = originalDepth - (originalLength - treeEdgeCursor);
            
            newEdge->m_edgeLabel = characterToExtend;
            newEdge->m_stringDepth = treeCursor->m_stringDepth + 1;

            for (map<char, SuffixTree6Edge*>::iterator ci = treeCursor->m_children.begin(); ci != treeCursor->m_children.end(); ci++)
            {
                oldEdge->m_children.insert(*ci);
            }

            newInternalNode = treeCursor;

            treeCursor->m_children.clear();
            treeCursor->m_children.insert(pair<char, SuffixTree6::SuffixTree6Edge*>(oldEdge->m_edgeLabel[0], oldEdge));
            treeCursor->m_children.insert(pair<char, SuffixTree6::SuffixTree6Edge*>(characterToExtend, newEdge));
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

string SuffixTree6::Show() const
{
    map<SuffixTree6::SuffixTree6Edge*, int> nodeIds;
    list<pair<SuffixTree6::SuffixTree6Edge*, SuffixTree6::SuffixTree6Edge*>> suffixLinks;
    map<int, int> stringDepths;
    list<pair<int, pair<string, int>>> edges;
    queue<pair<int, SuffixTree6::SuffixTree6Edge*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, SuffixTree6::SuffixTree6Edge*>(0, this->m_root));
    nodeIds.insert(pair<SuffixTree6::SuffixTree6Edge*, int>(this->m_root, 0));
    stringDepths.insert(pair<int, int>(0, this->m_root->m_stringDepth));
    while (bfsQueue.size() > 0)
    {
        pair<int, SuffixTree6::SuffixTree6Edge*> current = bfsQueue.front();
        bfsQueue.pop();
        int treeCursorId = current.first;
        SuffixTree6::SuffixTree6Edge* treeCursor = current.second;

        if (treeCursor->m_suffixLink != nullptr)
        {
            suffixLinks.push_back(pair<SuffixTree6::SuffixTree6Edge*, SuffixTree6::SuffixTree6Edge*>(treeCursor, treeCursor->m_suffixLink));
        }

        map<char, SuffixTree6::SuffixTree6Edge*>& treeCursorChildren = treeCursor->m_children;
        for (map<char, SuffixTree6::SuffixTree6Edge*>::iterator ci = treeCursorChildren.begin(); ci != treeCursorChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            SuffixTree6::SuffixTree6Edge* nextEdge = ci->second;
            bfsQueue.push(pair<int, SuffixTree6::SuffixTree6Edge*>(nextNodeId, nextEdge));
            nodeIds.insert(pair<SuffixTree6::SuffixTree6Edge*, int>(nextEdge, nextNodeId));
            stringDepths.insert(pair<int, int>(nextNodeId, nextEdge->m_stringDepth));
            edges.push_back(pair<int, pair<string, int>>(treeCursorId, pair<string, int>(nextEdge->m_edgeLabel, nextNodeId)));
        }
    }
    ostringstream stringBuilder;
    stringBuilder << "digraph {" << endl;
    for (int i = 0; i <= nodeId; i++)
    {
        stringBuilder << i << "[label = \"" << stringDepths[i] << "\"];" << endl;
    }
    for (list<pair<int, pair<string, int>>>::iterator ei = edges.begin(); ei != edges.end(); ei++)
    {
        stringBuilder << ei->first << "->" << ei->second.second << "[label = \" " << ei->second.first << " \"]" << endl;
    }
    for (list<pair<SuffixTree6::SuffixTree6Edge*, SuffixTree6::SuffixTree6Edge*>>::iterator si = suffixLinks.begin(); si != suffixLinks.end(); si++)
    {
        stringBuilder << nodeIds[si->first] << "->" << nodeIds[si->second] << "[style = dashed]" <<endl;
    }
    stringBuilder << "}" << endl;
    return stringBuilder.str();
}
