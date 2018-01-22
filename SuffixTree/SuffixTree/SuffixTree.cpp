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
}

SuffixTree::~SuffixTree()
{
    delete this->m_root;
}

SuffixTree::SuffixTreeEdge::SuffixTreeEdge() : m_begin(0), m_end(0), m_suffixLink(nullptr)
{

}

SuffixTree::SuffixTreeEdge::~SuffixTreeEdge()
{
    for (map<char, SuffixTree::SuffixTreeEdge*>::iterator ci = this->m_children.begin(); ci != this->m_children.end(); ci++)
    {
        delete ci->second;
    }
}

unsigned int SuffixTree::SuffixTreeEdge::end(SuffixTree::SuffixTreeEdge* root, SuffixTreeBuilder* builder)
{
    if (this != root && this->m_children.size() == 0)
    {
        return builder->m_currentEnd;
    }
    else
    {
        return this->m_end;
    }
}

unsigned int SuffixTree::SuffixTreeEdge::length(SuffixTree::SuffixTreeEdge* root, SuffixTreeBuilder* builder)
{
    return this->end(root, builder) - this->m_begin;
}

bool SuffixTree::Add(int keyBegin, int keyEnd, SuffixTreeBuilder* builder)
{
    bool noOpApplied = false;

    // Step 0: Use the suffix link to speed up the search
    SuffixTree::SuffixTreeEdge* treeCursor = builder->m_nextStart;
    unsigned int treeEdgeCursor = treeCursor->length(this->m_root, builder);
    unsigned int keyCursor = builder->m_nextDepth;
    unsigned int searchKeyLength = keyEnd - keyBegin - 1;

    builder->m_nextStart = this->m_root;
    builder->m_nextDepth = 0;

    // Step 1: Search for the string except the last character
    while (keyCursor < searchKeyLength)
    {
        char currentCharacter = builder->m_input[keyBegin + keyCursor];
        unsigned int treeCursorLength = treeCursor->length(this->m_root, builder);
        if (treeEdgeCursor == treeCursorLength)
        {
            if (treeCursor->m_suffixLink != nullptr)
            {
                builder->m_nextStart = treeCursor->m_suffixLink;
                builder->m_nextDepth = keyCursor - 1;
            }
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
            }
        }
        else
        {
            unsigned int move = min(searchKeyLength - keyCursor, treeCursorLength);
            keyCursor += move;
            treeEdgeCursor += move;
        }
    }

    // Step 2: Insert the character in the tree
    char characterToExtend = builder->m_input[keyEnd - 1];

    SuffixTree::SuffixTreeEdge* newInternalNode = nullptr;
    if (treeEdgeCursor == treeCursor->length(this->m_root, builder))
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

            // BUGBUG!
            // The idea of this code is that we change the tree cursor to represent something shorter
            // But if tree cursor is currently pointed by a suffix link, that would be the wrong thing to do.

            SuffixTree::SuffixTreeEdge* oldEdge = new SuffixTree::SuffixTreeEdge();
            SuffixTree::SuffixTreeEdge* newEdge = new SuffixTree::SuffixTreeEdge();

            int originalLength = treeCursor->length(this->m_root, builder);

            oldEdge->m_begin = treeCursor->m_begin + treeEdgeCursor;
            oldEdge->m_end = treeCursor->end(this->m_root, builder);
            oldEdge->m_suffixLink = treeCursor->m_suffixLink;

            treeCursor->m_end = treeCursor->m_begin + treeEdgeCursor;
            treeCursor->m_suffixLink = nullptr;
            newInternalNode = treeCursor;

            newEdge->m_begin = keyEnd - 1;
            newEdge->m_end = keyEnd;

            for (map<char, SuffixTreeEdge*>::iterator ci = treeCursor->m_children.begin(); ci != treeCursor->m_children.end(); ci++)
            {
                oldEdge->m_children.insert(*ci);
            }

            treeCursor->m_children.clear();
            treeCursor->m_children.insert(pair<char, SuffixTree::SuffixTreeEdge*>(builder->m_input[oldEdge->m_begin], oldEdge));
            treeCursor->m_children.insert(pair<char, SuffixTree::SuffixTreeEdge*>(characterToExtend, newEdge));
        }
    }

    if (builder->m_lastInternalNode != nullptr)
    {
        assert(builder->m_lastInternalNode->m_suffixLink == nullptr);
        builder->m_lastInternalNode->m_suffixLink = treeCursor;
        builder->m_lastInternalNode = nullptr;
    }

    if (newInternalNode != nullptr)
    {
        builder->m_lastInternalNode = newInternalNode;
    }

    return noOpApplied;
}

string SuffixTree::Show(string& input, SuffixTreeBuilder* builder) const
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
            edges.push_back(pair<int, pair<string, int>>(treeCursorId, pair<string, int>(input.substr(nextEdge->m_begin, nextEdge->end(this->m_root, builder) - nextEdge->m_begin), nextNodeId)));
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
        stringBuilder << nodeIds[si->first] << "->" << nodeIds[si->second] << "[style = dashed]" << endl;
    }
    stringBuilder << "}" << endl;
    return stringBuilder.str();
}
