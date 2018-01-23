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

SuffixTree::SuffixTreeEdge::SuffixTreeEdge() : m_begin(0), m_end(0), m_suffixLink(nullptr), m_parentLink(nullptr)
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
                treeCursor = probe->second;
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
                newEdge->m_parentLink = treeCursor;
            }
            else
            {
                // We have reached a non-leaf node - and the tree extends with our character
                // Therefore we will apply the no-op rule
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