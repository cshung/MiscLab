#include "SuffixTree8.h"
#include "SuffixTree8Builder.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>

SuffixTree8::SuffixTree8() : m_root(new SuffixTree8::SuffixTree8Edge())
{
    this->m_root->m_begin = 0;
    this->m_root->m_end = 0;
}

SuffixTree8::~SuffixTree8()
{
    delete this->m_root;
}

SuffixTree8::SuffixTree8Edge::SuffixTree8Edge() : m_suffixLink(nullptr)
{

}

SuffixTree8::SuffixTree8Edge::~SuffixTree8Edge()
{
    for (map<char, SuffixTree8::SuffixTree8Edge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

unsigned int SuffixTree8::SuffixTree8Edge::length()
{
    return this->m_end - this->m_begin;
}

bool SuffixTree8::Add(int keyBegin, int keyEnd, SuffixTree8Builder* builder)
{
    bool noOpApplied = false;
#ifdef _DEBUG
    builder->m_extensionCount++;
#endif
    // Step 0: Use the suffix link to speed up the search
    SuffixTree8::SuffixTree8Edge* treeCursor = builder->m_nextStart;
    unsigned int treeEdgeCursor = treeCursor->length();
    unsigned int keyCursor = builder->m_nextDepth;
    unsigned int searchKeyLength = keyEnd - keyBegin - 1;

    builder->m_nextStart = this->m_root;
    builder->m_nextDepth = 0;

    // Step 1: Search for the string except the last character
    while (keyCursor < searchKeyLength)
    {
        char currentCharacter = builder->m_input[keyBegin + keyCursor];
        if (treeEdgeCursor == treeCursor->length())
        {
            map<char, SuffixTree8::SuffixTree8Edge*>::iterator probe = treeCursor->m_children.find(currentCharacter);
            if (probe == treeCursor->m_children.end())
            {
                assert(false); // The string except the last character should always in the tree"
            }
            else
            {
                SuffixTree8Edge* currentEdge = probe->second;                
                treeCursor = currentEdge;
                treeEdgeCursor = 0;
                if (currentEdge->m_suffixLink)
                {
                    builder->m_nextStart = currentEdge->m_suffixLink;
                    builder->m_nextDepth = keyCursor;
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

    SuffixTree8::SuffixTree8Edge* newInternalNode = nullptr;
    if (treeEdgeCursor == treeCursor->length())
    {
        // We end up at a node
        if (treeCursor != this->m_root && treeCursor->m_children.size() == 0)
        {
            // We have reached a leaf - and therefore we will apply the leaf extension rule
            treeCursor->m_end++;
        }
        else
        {
            map<char, SuffixTree8::SuffixTree8Edge*>::iterator probe = treeCursor->m_children.find(characterToExtend);
            if (probe == treeCursor->m_children.end())
            {
                // We have reached a non-leaf node - and the tree does not extend with our character
                // Therefore we will apply the split rule
                SuffixTree8::SuffixTree8Edge* newEdge = new SuffixTree8::SuffixTree8Edge();
                newEdge->m_begin = keyEnd - 1;
                newEdge->m_end = keyEnd;
                treeCursor->m_children.insert(pair<char, SuffixTree8::SuffixTree8Edge*>(characterToExtend, newEdge));
            }
            else
            {
                // We have reached a non-leaf node - and the tree extends with our character
                // Therefore we will apply the no-op rule
                noOpApplied = true;
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
            SuffixTree8::SuffixTree8Edge* oldEdge = new SuffixTree8::SuffixTree8Edge();
            SuffixTree8::SuffixTree8Edge* newEdge = new SuffixTree8::SuffixTree8Edge();
            int originalLength = treeCursor->length();
            
            oldEdge->m_begin = treeCursor->m_begin + treeEdgeCursor;
            oldEdge->m_end = treeCursor->m_end;

            treeCursor->m_end = treeCursor->m_begin + treeEdgeCursor;
            
            newEdge->m_begin = keyEnd - 1;
            newEdge->m_end = keyEnd;

            for (map<char, SuffixTree8Edge*>::iterator ci = treeCursor->m_children.begin(); ci != treeCursor->m_children.end(); ci++)
            {
                oldEdge->m_children.insert(*ci);
            }

            newInternalNode = treeCursor;

            treeCursor->m_children.clear();
            treeCursor->m_children.insert(pair<char, SuffixTree8::SuffixTree8Edge*>(builder->m_input[oldEdge->m_begin], oldEdge));
            treeCursor->m_children.insert(pair<char, SuffixTree8::SuffixTree8Edge*>(characterToExtend, newEdge));
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

string SuffixTree8::Show(string& input) const
{
    map<SuffixTree8::SuffixTree8Edge*, int> nodeIds;
    list<pair<SuffixTree8::SuffixTree8Edge*, SuffixTree8::SuffixTree8Edge*>> suffixLinks;
    list<pair<int, pair<string, int>>> edges;
    queue<pair<int, SuffixTree8::SuffixTree8Edge*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, SuffixTree8::SuffixTree8Edge*>(0, this->m_root));
    nodeIds.insert(pair<SuffixTree8::SuffixTree8Edge*, int>(this->m_root, 0));
    while (bfsQueue.size() > 0)
    {
        pair<int, SuffixTree8::SuffixTree8Edge*> current = bfsQueue.front();
        bfsQueue.pop();
        int treeCursorId = current.first;
        SuffixTree8::SuffixTree8Edge* treeCursor = current.second;

        if (treeCursor->m_suffixLink != nullptr)
        {
            suffixLinks.push_back(pair<SuffixTree8::SuffixTree8Edge*, SuffixTree8::SuffixTree8Edge*>(treeCursor, treeCursor->m_suffixLink));
        }

        map<char, SuffixTree8::SuffixTree8Edge*>& treeCursorChildren = treeCursor->m_children;
        for (map<char, SuffixTree8::SuffixTree8Edge*>::iterator ci = treeCursorChildren.begin(); ci != treeCursorChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            SuffixTree8::SuffixTree8Edge* nextEdge = ci->second;
            bfsQueue.push(pair<int, SuffixTree8::SuffixTree8Edge*>(nextNodeId, nextEdge));
            nodeIds.insert(pair<SuffixTree8::SuffixTree8Edge*, int>(nextEdge, nextNodeId));
            edges.push_back(pair<int, pair<string, int>>(treeCursorId, pair<string, int>(input.substr(nextEdge->m_begin, nextEdge->m_end - nextEdge->m_begin), nextNodeId)));
        }
    }
    ostringstream stringBuilder;
    stringBuilder << "digraph {" << endl;
    for (int i = 0; i <= nodeId; i++)
    {
        stringBuilder << i << "[label = \"\"];" << endl;
    }
    for (list<pair<int, pair<string, int>>>::iterator ei = edges.begin(); ei != edges.end(); ei++)
    {
        stringBuilder << ei->first << "->" << ei->second.second << "[label = \" " << ei->second.first << " \"]" << endl;
    }
    for (list<pair<SuffixTree8::SuffixTree8Edge*, SuffixTree8::SuffixTree8Edge*>>::iterator si = suffixLinks.begin(); si != suffixLinks.end(); si++)
    {
        stringBuilder << nodeIds[si->first] << "->" << nodeIds[si->second] << "[style = dashed]" <<endl;
    }
    stringBuilder << "}" << endl;
    return stringBuilder.str();
}
