#include "SuffixTree.h"
#include "SuffixTreeBuilder.h"
#include <queue>
#include <list>
#include <string>
#include <sstream>
#include <iostream>
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

int SuffixTree::SuffixTreeEdge::s_id = 0;

SuffixTree::SuffixTreeEdge::SuffixTreeEdge() : m_id(++s_id), m_begin(0), m_end(0), m_suffixLink(nullptr), m_parentLink(nullptr)
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
    SuffixTree::SuffixTreeEdge* searchEnd = nullptr;

    unsigned int treeEdgeCursor = treeCursor->length(this->m_root, builder);
    unsigned int keyCursor = builder->m_nextDepth;
    unsigned int searchKeyLength = keyEnd - keyBegin - 1;

    builder->m_nextStart = this->m_root;
    builder->m_nextDepth = 0;

    // Step 1: Search for the string except the last character
    while (keyCursor < searchKeyLength)
    {
        cout << "10. " << treeEdgeCursor << endl;
        cout << "13. " << treeCursor->m_id << endl;
        char currentCharacter = builder->m_input[keyBegin + keyCursor];
        unsigned int treeCursorLength = treeCursor->length(this->m_root, builder);
        if (treeEdgeCursor == treeCursorLength)
        {
            if (treeCursor->m_suffixLink != nullptr)
            {
                builder->m_nextStart = treeCursor->m_suffixLink;
                builder->m_nextDepth = keyCursor - 1;
                cout << "9. " << (keyBegin + builder->m_nextDepth + 1) << endl;
            }
            cout << "14. " << (keyBegin + keyCursor) << currentCharacter << endl;
            map<char, SuffixTree::SuffixTreeEdge*>::iterator probe = treeCursor->m_children.find(currentCharacter);
            if (probe == treeCursor->m_children.end())
            {
                assert(false); // The string except the last character should always in the tree"
            }
            else
            {
                treeCursor = probe->second;
                treeEdgeCursor = 0;
            }
        }
        else
        {
            unsigned int move = min(searchKeyLength - keyCursor, treeCursorLength);
            cout << "12. " << move << "," << (searchKeyLength - keyCursor) << "," << treeCursorLength << endl;
            keyCursor += move;
            treeEdgeCursor += move;
        }
    }
    cout << "11. " << treeEdgeCursor << endl;

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
            cout << "8. " << treeCursor->m_id << endl;
            // cout << "7. Search: ";
            for (auto pair : treeCursor->m_children)
            {
                // cout << pair.first;
            }
            // cout << endl;
            map<char, SuffixTree::SuffixTreeEdge*>::iterator probe = treeCursor->m_children.find(characterToExtend);
            if (probe == treeCursor->m_children.end())
            {
                cout << "6. Split " << characterToExtend << endl;
                // We have reached a non-leaf node - and the tree does not extend with our character
                // Therefore we will apply the split rule
                SuffixTree::SuffixTreeEdge* newEdge = new SuffixTree::SuffixTreeEdge();
                newEdge->m_begin = keyEnd - 1;
                treeCursor->m_children.insert(pair<char, SuffixTree::SuffixTreeEdge*>(characterToExtend, newEdge));
                newEdge->m_parentLink = treeCursor;
            }
            else
            {
                // We have reached a non-leaf node - and the tree extends with our character
                // Therefore we will apply the no-op rule
                cout << "4. NoOp " << characterToExtend << endl;
                noOpApplied = true;
            }
        }

        searchEnd = treeCursor;
    }
    else
    {
        // We end up at an edge
        char treeContinuation = builder->m_input[treeCursor->m_begin + treeEdgeCursor];
        if (treeContinuation == characterToExtend)
        {
            // We have reach the middle of an edge, and the edge extends with our character
            // Therefore we will apply the no-op rule
            cout << "5. NoOp" << endl;
            noOpApplied = true;
        }
        else
        {
            // We have reach the middle of an edge, and the edge does not extend with our character
            // Therefore we will apply the split rule

            SuffixTree::SuffixTreeEdge* newNode = new SuffixTree::SuffixTreeEdge();
            SuffixTree::SuffixTreeEdge* newLeaf = new SuffixTree::SuffixTreeEdge();

            newNode->m_begin = treeCursor->m_begin;
            newNode->m_end = treeCursor->m_begin + treeEdgeCursor;

            newLeaf->m_begin = keyEnd - 1;
            newLeaf->m_end = keyEnd;

            treeCursor->m_begin = treeCursor->m_begin + treeEdgeCursor;

            newNode->m_children.insert(make_pair(treeContinuation, treeCursor));
            newNode->m_children.insert(make_pair(characterToExtend, newLeaf));

            newNode->m_parentLink = treeCursor->m_parentLink;
            treeCursor->m_parentLink = newNode;
            newLeaf->m_parentLink = newNode;

            assert(newNode->m_parentLink != nullptr);
            char newNodeStart = builder->m_input[newNode->m_begin];
            newNode->m_parentLink->m_children[newNodeStart] = newNode;

            newInternalNode = newNode;
            searchEnd = newInternalNode;
        }
    }

    if (builder->m_lastInternalNode != nullptr)
    {
        assert(builder->m_lastInternalNode->m_suffixLink == nullptr);
        assert(searchEnd != nullptr);
        builder->m_lastInternalNode->m_suffixLink = searchEnd;
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
    list<pair<SuffixTree::SuffixTreeEdge*, SuffixTree::SuffixTreeEdge*> > suffixLinks;
    list<pair<int, pair<string, int> > > edges;
    queue<pair<int, SuffixTree::SuffixTreeEdge*> > bfsQueue;
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
            edges.push_back(pair<int, pair<string, int> >(treeCursorId, pair<string, int>(input.substr(nextEdge->m_begin, nextEdge->end(this->m_root, builder) - nextEdge->m_begin), nextNodeId)));
        }
    }
    ostringstream stringBuilder;
    stringBuilder << "digraph {" << endl;
    for (int i = 0; i <= nodeId; i++)
    {
        stringBuilder << i << "[label = \"\"];" << endl;
    }
    for (list<pair<int, pair<string, int> > >::iterator ei = edges.begin(); ei != edges.end(); ei++)
    {
        stringBuilder << ei->first << "->" << ei->second.second << "[label = \" " << ei->second.first << " \"]" << endl;
    }
    for (list<pair<SuffixTree::SuffixTreeEdge*, SuffixTree::SuffixTreeEdge*> >::iterator si = suffixLinks.begin(); si != suffixLinks.end(); si++)
    {
        stringBuilder << nodeIds[si->first] << "->" << nodeIds[si->second] << "[style = dashed]" <<endl;
    }
    stringBuilder << "}" << endl;
    return stringBuilder.str();
}
