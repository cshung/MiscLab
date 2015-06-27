#include "SuffixTree.h"
#include "SuffixTreeBuilder.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>

SuffixTree::SuffixTree() : m_root(new SuffixTree::SuffixTreeEdge())
{
    this->m_root->m_begin = 0;
    this->m_root->m_end = 0;
    this->m_root->m_builder = nullptr;
}

SuffixTree::~SuffixTree()
{
    delete this->m_root;
}

SuffixTree::SuffixTreeEdge::SuffixTreeEdge() : m_suffixLink(nullptr), m_builder(nullptr)
{

}

SuffixTree::SuffixTreeEdge::~SuffixTreeEdge()
{
    for (map<char, SuffixTree::SuffixTreeEdge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

unsigned int SuffixTree::SuffixTreeEdge::end()
{
    if (this->m_builder == nullptr)
    {
        return this->m_end;
    }
    else
    {
        return this->m_builder->m_currentEnd;
    }
}
unsigned int SuffixTree::SuffixTreeEdge::length()
{
    return this->end() - this->m_begin;
}

bool SuffixTree::Add(int keyBegin, int keyEnd, SuffixTreeBuilder* builder)
{
    bool noOpApplied = false;
#ifdef _DEBUG
    builder->m_extensionCount++;
#endif
    // Step 0: Use the suffix link to speed up the search
    SuffixTree::SuffixTreeEdge* treeCursor = builder->m_nextStart;
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
            map<char, SuffixTree::SuffixTreeEdge*>::iterator probe = treeCursor->m_children.find(currentCharacter);
            if (probe == treeCursor->m_children.end())
            {
                assert(false); // The string except the last character should always in the tree"
            }
            else
            {
                SuffixTreeEdge* currentEdge = probe->second;                
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

    
    SuffixTree::SuffixTreeEdge* newInternalNode = nullptr;
    if (treeEdgeCursor == treeCursor->length())
    {
        // We end up at a node
        if (treeCursor != this->m_root && treeCursor->m_children.size() == 0)
        {
            // A leaf becomes no-op
        }
        else
        {
            map<char, SuffixTree::SuffixTreeEdge*>::iterator probe = treeCursor->m_children.find(characterToExtend);
            if (probe == treeCursor->m_children.end())
            {
                // We have reached a non-leaf node - and the tree does not extend with our character
                // Therefore we will apply the split rule
                SuffixTree::SuffixTreeEdge* newEdge = new SuffixTree::SuffixTreeEdge();
                newEdge->m_begin = keyEnd - 1;
                newEdge->m_builder = builder;
                treeCursor->m_children.insert(pair<char, SuffixTree::SuffixTreeEdge*>(characterToExtend, newEdge));
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
            SuffixTree::SuffixTreeEdge* oldEdge = new SuffixTree::SuffixTreeEdge();
            SuffixTree::SuffixTreeEdge* newEdge = new SuffixTree::SuffixTreeEdge();
            int originalLength = treeCursor->length();
            
            oldEdge->m_begin = treeCursor->m_begin + treeEdgeCursor;
            oldEdge->m_builder = treeCursor->m_builder;
            oldEdge->m_end = treeCursor->end();

            treeCursor->m_end = treeCursor->m_begin + treeEdgeCursor;
            treeCursor->m_builder = nullptr;
            
            newEdge->m_begin = keyEnd - 1;
            newEdge->m_end = keyEnd;
            newEdge->m_builder = builder;

            for (map<char, SuffixTreeEdge*>::iterator ci = treeCursor->m_children.begin(); ci != treeCursor->m_children.end(); ci++)
            {
                oldEdge->m_children.insert(*ci);
            }

            newInternalNode = treeCursor;

            treeCursor->m_children.clear();
            treeCursor->m_children.insert(pair<char, SuffixTree::SuffixTreeEdge*>(builder->m_input[oldEdge->m_begin], oldEdge));
            treeCursor->m_children.insert(pair<char, SuffixTree::SuffixTreeEdge*>(characterToExtend, newEdge));
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

string SuffixTree::Show(string& input) const
{
    map<SuffixTree::SuffixTreeEdge*, int> nodeIds;
    list<pair<SuffixTree::SuffixTreeEdge*, SuffixTree::SuffixTreeEdge*>> suffixLinks;
    list<pair<int, pair<string, int>>> edges;
    queue<pair<int, SuffixTree::SuffixTreeEdge*>> bfsQueue;
    int nodeId = 0;
    bfsQueue.push(pair<int, SuffixTree::SuffixTreeEdge*>(0, this->m_root));
    nodeIds.insert(pair<SuffixTree::SuffixTreeEdge*, int>(this->m_root, 0));
    while (bfsQueue.size() > 0)
    {
        pair<int, SuffixTree::SuffixTreeEdge*> current = bfsQueue.front();
        bfsQueue.pop();
        int treeCursorId = current.first;
        SuffixTree::SuffixTreeEdge* treeCursor = current.second;

        if (treeCursor->m_suffixLink != nullptr)
        {
            suffixLinks.push_back(pair<SuffixTree::SuffixTreeEdge*, SuffixTree::SuffixTreeEdge*>(treeCursor, treeCursor->m_suffixLink));
        }

        map<char, SuffixTree::SuffixTreeEdge*>& treeCursorChildren = treeCursor->m_children;
        for (map<char, SuffixTree::SuffixTreeEdge*>::iterator ci = treeCursorChildren.begin(); ci != treeCursorChildren.end(); ci++)
        {
            int nextNodeId = ++nodeId;
            SuffixTree::SuffixTreeEdge* nextEdge = ci->second;
            bfsQueue.push(pair<int, SuffixTree::SuffixTreeEdge*>(nextNodeId, nextEdge));
            nodeIds.insert(pair<SuffixTree::SuffixTreeEdge*, int>(nextEdge, nextNodeId));
            edges.push_back(pair<int, pair<string, int>>(treeCursorId, pair<string, int>(input.substr(nextEdge->m_begin, nextEdge->end() - nextEdge->m_begin), nextNodeId)));
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
    for (list<pair<SuffixTree::SuffixTreeEdge*, SuffixTree::SuffixTreeEdge*>>::iterator si = suffixLinks.begin(); si != suffixLinks.end(); si++)
    {
        stringBuilder << nodeIds[si->first] << "->" << nodeIds[si->second] << "[style = dashed]" <<endl;
    }
    stringBuilder << "}" << endl;
    return stringBuilder.str();
}
