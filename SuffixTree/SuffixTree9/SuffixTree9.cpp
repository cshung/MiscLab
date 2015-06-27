#include "SuffixTree9.h"
#include "SuffixTree9Builder.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>

SuffixTree9::SuffixTree9() : m_root(new SuffixTree9::SuffixTree9Edge())
{
    this->m_root->m_stringDepth = 0;
    this->m_root->m_begin = 0;
    this->m_root->m_end = 0;
}

SuffixTree9::~SuffixTree9()
{
    delete this->m_root;
}

SuffixTree9::SuffixTree9Edge::SuffixTree9Edge() : m_suffixLink(nullptr), m_stringDepth(0)
{

}

SuffixTree9::SuffixTree9Edge::~SuffixTree9Edge()
{
    for (map<char, SuffixTree9::SuffixTree9Edge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

unsigned int SuffixTree9::SuffixTree9Edge::length()
{
    return this->m_end - this->m_begin;
}

bool SuffixTree9::Add(int keyBegin, int keyEnd, SuffixTree9Builder* builder)
{
    bool noOpApplied = false;
#ifdef _DEBUG
    builder->m_extensionCount++;
#endif
    // Step 0: Use the suffix link to speed up the search
    SuffixTree9::SuffixTree9Edge* treeCursor = builder->m_nextStart;
    unsigned int treeEdgeCursor = treeCursor->length();
    unsigned int keyCursor = treeCursor->m_stringDepth;;
    unsigned int searchKeyLength = keyEnd - keyBegin - 1;

    builder->m_nextStart = this->m_root;

    // Step 1: Search for the string except the last character
    while (keyCursor < searchKeyLength)
    {
        char currentCharacter = builder->m_input[keyBegin + keyCursor];
        if (treeEdgeCursor == treeCursor->length())
        {
            map<char, SuffixTree9::SuffixTree9Edge*>::iterator probe = treeCursor->m_children.find(currentCharacter);
            if (probe == treeCursor->m_children.end())
            {
                assert(false); // The string except the last character should always in the tree"
            }
            else
            {
                SuffixTree9Edge* currentEdge = probe->second;                
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
            unsigned int move = min(searchKeyLength - keyCursor, treeCursor->length());
#ifdef _DEBUG
            unsigned int myTreeEdgeCursor = treeEdgeCursor;
            unsigned int myKeyCursor = keyCursor;
            for (myTreeEdgeCursor = 0; myTreeEdgeCursor < move; myKeyCursor++, myTreeEdgeCursor++)
            {
                if (builder->m_input[keyBegin + myKeyCursor] != builder->m_input[treeCursor->m_begin + myTreeEdgeCursor])
                {
                    assert(false); // The string except the last character should always in the tree"
                }
            }
#endif
            keyCursor += move;
            treeEdgeCursor += move;
        }
    }

    // Step 2: Insert the character in the tree
    char characterToExtend = builder->m_input[keyEnd - 1];

    SuffixTree9::SuffixTree9Edge* newInternalNode = nullptr;
    if (treeEdgeCursor == treeCursor->length())
    {
        // We end up at a node
        if (treeCursor == this->m_root)
        {
            // I have no choice but to branch out for the root node
            SuffixTree9::SuffixTree9Edge* newEdge = new SuffixTree9::SuffixTree9Edge();
            newEdge->m_begin = keyBegin;
            newEdge->m_end = keyEnd;
            newEdge->m_stringDepth = 1;
            this->m_root->m_children.insert(pair<char, SuffixTree9::SuffixTree9Edge*>(builder->m_input[keyBegin], newEdge));
            if (builder->m_fullStringLeaf == nullptr)
            {
                builder->m_fullStringLeaf = newEdge;
            }
        }
        else
        {
            if (treeCursor->m_children.size() == 0)
            {
                // We have reached a leaf - and therefore we will apply the leaf extension rule
                treeCursor->m_end++;
                treeCursor->m_stringDepth++;
            }
            else
            {
                map<char, SuffixTree9::SuffixTree9Edge*>::iterator probe = treeCursor->m_children.find(characterToExtend);
                if (probe == treeCursor->m_children.end())
                {
                    // We have reached a non-leaf node - and the tree does not extend with our character
                    // Therefore we will apply the split rule
                    SuffixTree9::SuffixTree9Edge* newEdge = new SuffixTree9::SuffixTree9Edge();
                    newEdge->m_begin = keyEnd - 1;
                    newEdge->m_end = keyEnd;
                    treeCursor->m_children.insert(pair<char, SuffixTree9::SuffixTree9Edge*>(characterToExtend, newEdge));
                }
                else
                {
                    // We have reached a non-leaf node - and the tree extends with our character
                    // Therefore we will apply the no-op rule
                    noOpApplied = true;
                }
            }
        }
    }
    else
    {
        // We end up at an edge
        if (builder->m_input[treeCursor->m_begin + treeEdgeCursor] == characterToExtend)
        {
            // We have reach the middle of an edge, and the edge extends with our character
            // Therefore we will apply the no-op rule
            noOpApplied = true;
        }
        else
        {
            // We have reach the middle of an edge, and the edge does not extend with our character
            // Therefore we will apply the split rule
            SuffixTree9::SuffixTree9Edge* oldEdge = new SuffixTree9::SuffixTree9Edge();
            SuffixTree9::SuffixTree9Edge* newEdge = new SuffixTree9::SuffixTree9Edge();
            int originalDepth = treeCursor->m_stringDepth;
            int originalLength = treeCursor->length();
            
            oldEdge->m_begin = treeCursor->m_begin + treeEdgeCursor;
            oldEdge->m_end = treeCursor->m_end;
            oldEdge->m_stringDepth = originalDepth;

            treeCursor->m_end = treeCursor->m_begin + treeEdgeCursor;
            treeCursor->m_stringDepth = originalDepth - (originalLength - treeEdgeCursor);
            
            newEdge->m_begin = keyEnd - 1;
            newEdge->m_end = keyEnd;
            newEdge->m_stringDepth = treeCursor->m_stringDepth + 1;

            for (map<char, SuffixTree9Edge*>::iterator ci = treeCursor->m_children.begin(); ci != treeCursor->m_children.end(); ci++)
            {
                oldEdge->m_children.insert(*ci);
            }

            newInternalNode = treeCursor;

            treeCursor->m_children.clear();
            treeCursor->m_children.insert(pair<char, SuffixTree9::SuffixTree9Edge*>(builder->m_input[oldEdge->m_begin], oldEdge));
            treeCursor->m_children.insert(pair<char, SuffixTree9::SuffixTree9Edge*>(characterToExtend, newEdge));
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
    
    return noOpApplied;
}

string SuffixTree9::Show(string& input) const
{
    map<SuffixTree9::SuffixTree9Edge*, int> nodeIds;
    list<pair<SuffixTree9::SuffixTree9Edge*, SuffixTree9::SuffixTree9Edge*>> suffixLinks;
    map<int, int> stringDepths;
    list<pair<int, pair<string, int>>> edges;
    queue<pair<int, SuffixTree9::SuffixTree9Edge*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, SuffixTree9::SuffixTree9Edge*>(0, this->m_root));
    nodeIds.insert(pair<SuffixTree9::SuffixTree9Edge*, int>(this->m_root, 0));
    stringDepths.insert(pair<int, int>(0, this->m_root->m_stringDepth));
    while (bfsQueue.size() > 0)
    {
        pair<int, SuffixTree9::SuffixTree9Edge*> current = bfsQueue.front();
        bfsQueue.pop();
        int treeCursorId = current.first;
        SuffixTree9::SuffixTree9Edge* treeCursor = current.second;

        if (treeCursor->m_suffixLink != nullptr)
        {
            suffixLinks.push_back(pair<SuffixTree9::SuffixTree9Edge*, SuffixTree9::SuffixTree9Edge*>(treeCursor, treeCursor->m_suffixLink));
        }

        map<char, SuffixTree9::SuffixTree9Edge*>& treeCursorChildren = treeCursor->m_children;
        for (map<char, SuffixTree9::SuffixTree9Edge*>::iterator ci = treeCursorChildren.begin(); ci != treeCursorChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            SuffixTree9::SuffixTree9Edge* nextEdge = ci->second;
            bfsQueue.push(pair<int, SuffixTree9::SuffixTree9Edge*>(nextNodeId, nextEdge));
            nodeIds.insert(pair<SuffixTree9::SuffixTree9Edge*, int>(nextEdge, nextNodeId));
            stringDepths.insert(pair<int, int>(nextNodeId, nextEdge->m_stringDepth));
            edges.push_back(pair<int, pair<string, int>>(treeCursorId, pair<string, int>(input.substr(nextEdge->m_begin, nextEdge->m_end - nextEdge->m_begin), nextNodeId)));
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
    for (list<pair<SuffixTree9::SuffixTree9Edge*, SuffixTree9::SuffixTree9Edge*>>::iterator si = suffixLinks.begin(); si != suffixLinks.end(); si++)
    {
        stringBuilder << nodeIds[si->first] << "->" << nodeIds[si->second] << "[style = dashed]" <<endl;
    }
    stringBuilder << "}" << endl;
    return stringBuilder.str();
}
