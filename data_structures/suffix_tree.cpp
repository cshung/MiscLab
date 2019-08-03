#include "suffix_tree.h"
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

/**
 * A suffix tree is a tree that represents all the suffixes of a string.
 * This implementation allows the string to be extended at the end.
 *
 * Conceptually, the edges between the nodes has some characters on it.
 * If one reads from the root node to a leaf node, one should read a
 * suffix of the string. If the last character is unique, the tree
 * contains all the suffixes.
 *
 * In any tree, except the root, every node has a parent edge, therefore
 * We just store the edge information with the root. Because we have the
 * string, and edge labels are always substrings, it is enough to just
 * store the indices.
 *
 * The parent and suffix link fields are a bit unfortunate. They are 
 * artifacts needed for the construction of the tree, but otherwise
 * often unnecessary for using the tree.
 */
class suffix_tree_node
{
public:
    suffix_tree_node();
    ~suffix_tree_node();
    int m_begin;
    int m_end;
    suffix_tree_node* m_parent;
    suffix_tree_node* m_first_child;
    suffix_tree_node* m_sibling;
    suffix_tree_node* m_suffix_link;
};

/**
 * Here is an implementation of the Ukkonen's suffix tree construction algorithm
 * The code is ported from my C++ implementation.
 *
 * The code is best understood by reading Dan Gusfield's book, because that's what
 * I read and implemented it based on the book. It is chapter 6.1
 *
 * At a high level, the Ukkonen's algorithm allows growing the suffix tree one 
 * character at a time. When the jth character is added, conceptually add all the 
 * new suffixes to the tree. 
 *
 * Now start reading the append() method to understand how it is achieved.
 */
class suffix_tree_impl
{
public:
    suffix_tree_impl();
    void append(char c);
    void show();
    suffix_tree_node* get_root();
    suffix_tree_node* get_child(suffix_tree_node* n);
    suffix_tree_node* get_sibling(suffix_tree_node* n);
    void get_edge(suffix_tree_node* n, int* pBegin, int* pEnd);
    ~suffix_tree_impl();
private:
    char first_char(suffix_tree_node* node);
    int length(suffix_tree_node* node);
    bool extension(suffix_tree_node*& next_node_cursor, int& next_text_cursor);
    void search(suffix_tree_node*& next_node_cursor, int& next_text_cursor, suffix_tree_node*& node_cursor, int& edge_cursor);
    void show(suffix_tree_node* node, int indent);
    vector<char> m_s;
    suffix_tree_node* m_root;

    suffix_tree_node* m_last_internal_node;
    int m_start;
};

suffix_tree_node::suffix_tree_node() : m_begin(0), m_end(0), m_parent(nullptr), m_first_child(nullptr), m_sibling(nullptr), m_suffix_link(nullptr)
{

}

suffix_tree_node::~suffix_tree_node()
{
    if (this->m_first_child != nullptr)
    {
        delete this->m_first_child;
    }
    if (this->m_sibling != nullptr)
    {
        delete this->m_sibling;
    }
}

suffix_tree_impl::suffix_tree_impl()
{
    this->m_root = new suffix_tree_node();
    this->m_last_internal_node = nullptr;
    this->m_start = 0;
}


/**
 * To add a character to a suffix tree, we add all the new suffixes to the tree
 * A suffix starting from _start is added to the tree by calling the extension method
 * 
 * To be consistent with the book, we will call each invocation of this method a phase.
 * 
 * Conceptually, _start should start from 1. It is not in the code because of the 
 * following rule:
 *
 * Once a leaf, always a leaf.
 * 
 * Suppose the last phase introduced a leaf node, in this phase, the only thing that we
 * need to do for this phase is to extend the edge label. All leaves must terminate at the 
 * end of the string, so if we implicity interprets the end of a leaf node to be the length
 * of the string and never explicitly store it, there is nothing we need to for them. Therefore
 * we skipped them all.
 *
 * Here begs the question, how do we know from 0 to _start - 1 were leaves in the last phase,
 * we will read more about that in the extension() method.
 *
 * Another things to notice is that if the already_in_tree returned by the extension method is True,
 * then we stop adding. There is because of the following rule:
 * 
 * Rule 3 is a show stopper
 *
 * Without referring to the book, this would be a mystery. What is rule 3 anyway? Here is an 
 * attempt to describe it succinctly. In the following, We use capital letter to represent strings
 * and lowercase character to represent characters. 
 *
 * Suppose we know that in the last extension, we were trying to insert aXb into the tree and we 
 * discovered that it is already there. That means aXb was a suffix in the last phase, so Xb must
 * also be a suffix in the last phase, so we can skip adding Xb. But not just that, any suffixes 
 * of Xb
 *
 * Another interesting piece in this code is the next_node_cursor and next_text_cursor. They can
 * be understood as the cursors where we search in the tree. We will soon see how they are used
 * in the extension method. For now, what we needed to know is that the extension() method have 
 * some way to speed up the next suffix insertion by maintaining these states.
 *
 * Now start reading the extension() method to see how a single suffix is added the tree.
 */
void suffix_tree_impl::append(char c)
{
    this->m_s.push_back(c);
    suffix_tree_node* next_node_cursor = this->m_root;
    int next_text_cursor = this->m_start;
    for (; this->m_start < this->m_s.size(); this->m_start++)
    {
        if (this->extension(/* byref */ next_node_cursor, /* byref */ next_text_cursor))
        {
            break;
        }
    }
}

/**
 * The extension method add a suffix into the tree. We knew that the whole suffix besides the last character 
 * must be already in the tree during the last phase, therefore the first thing to do is to find out where
 * to add the last character. This is done by the search() method.
 *
 * Without looking into the search() method yet, it is useful to describe how do we represent a location in
 * the tree. One could imagine the tree labels are highlighted and the highlight stop somewhere. There are two
 * cases, either the highlighting is filling every edge label, or the highlighting fill the last edge partially.
 * In both case, we can represent the location by a pair, the node that owns the edge label, and the number of 
 * character filled by the edge label. That is how node_cursor and edge_cursor are interpreted.
 * 
 * text_cursor is simply the index of the next character to be added in the tree, so there is no need for a variable
 * for it, it is simply len(self._s).
 * 
 * the next_* variables are optimization. They allows the search to be short circuited so that it doesn't always 
 * start from the beginning. For now, just assume the search method always returns node_cursor and edge_cursor at
 * the right place.
 * 
 * Now here we are a few cases. The first branch is the case where the edge label is completely filled. If the node
 * is a leaf, there is nothing to do because the implicit interpretation of the leaf node will make sure the last 
 * character is added. That said, the already_in_tree flag stays False, because conceptually we still added the 
 * character to the tree.
 *
 * In case the edge label is completely filled and yet it is not a leaf, there are still two cases. It could be the 
 * case that there is a child node that continues with the last character. In that case we know the suffix is already
 * in the tree, so we set already_in_tree to True and stop. Otherwise we create a new leaf node and stop.
 *
 * Similarly, in case the edge label is not completely filled, we check if the last character is already in the tree.
 * If it does, we set already_in_tree to True and stop. Otherwise, we have to split the edge.
 *
 * In all cases, we maintain two variables. search_end and new_internal_node. search_end represents the last node we 
 * reached in the search, and new internal node represents the new internal node we created in the split edge case.
 * Theses two variables are used to build suffix links, a tool for speeding up searches.
 * 
 * Suffix links only applies to nodes that are not leaves and not root. If such a node represents the prefix 'aX', 
 * then the suffix link of it points to an internal node that represents X. Suppose during the insertion of 'aXY' 
 * found such a link, there we could use that to speed to the search for 'XY' in the next extension. It is obvious 
 * why it could be useful. What is not so obvious is that suffix links are easy to build and always available.
 *
 * A theorem in the book shows that if an internal node is created in the current extension, the next extension
 * would have found its suffix link. Therefore, we save the new_internal_node and set its suffix link to search_end
 * in the next iteration.
 *
 * To see how the suffix link is used to speed up the search, read the search() method now.
 */
bool suffix_tree_impl::extension(suffix_tree_node*& next_node_cursor, int& next_text_cursor)
{
    suffix_tree_node* node_cursor = next_node_cursor;
    int edge_cursor = this->length(node_cursor);
    bool already_in_tree = false;

    this->search(/* byref */ next_node_cursor, /* byref */ next_text_cursor, /* byref */ node_cursor, /* byref */ edge_cursor);

    char next_text_char = this->m_s[this->m_s.size() - 1];
    suffix_tree_node* search_end = nullptr;
    suffix_tree_node* new_internal_node = nullptr;
    if (edge_cursor == length(node_cursor))
    {
        if (node_cursor != this->m_root && node_cursor->m_first_child == nullptr)
        {
        }
        else
        {
            suffix_tree_node* search = node_cursor->m_first_child;
            bool found = false;
            while (search != nullptr)
            {
                if (first_char(search) == next_text_char)
                {
                    found = true;
                    break;
                }
                else
                {
                    search = search->m_sibling;
                }
            }
            if (found)
            {
                already_in_tree = true;
            }
            else
            {
                suffix_tree_node* new_leaf = new suffix_tree_node();
                new_leaf->m_begin = this->m_s.size() - 1;
                new_leaf->m_parent = node_cursor;
                new_leaf->m_sibling = node_cursor->m_first_child;
                node_cursor->m_first_child = new_leaf;
            }
        }
        search_end = node_cursor;
    }
    else
    {
        char next_tree_char = this->m_s[node_cursor->m_begin + edge_cursor];
        if (next_text_char == next_tree_char)
        {
            already_in_tree = true;
        }
        else
        {
            suffix_tree_node* new_node = new suffix_tree_node();
            suffix_tree_node* new_leaf = new suffix_tree_node();
            new_leaf->m_begin = this->m_s.size() - 1;
            new_node->m_begin = node_cursor->m_begin;
            new_node->m_end = node_cursor->m_begin + edge_cursor;
            node_cursor->m_begin = node_cursor->m_begin + edge_cursor;

            new_node->m_parent = node_cursor->m_parent;
            new_leaf->m_parent = new_node;
            node_cursor->m_parent = new_node;

            new_node->m_sibling = new_node->m_parent->m_first_child;
            new_node->m_parent->m_first_child = new_node;

            suffix_tree_node* search = new_node;
            while (search != nullptr)
            {
                if (search->m_sibling == node_cursor)
                {
                    search->m_sibling = search->m_sibling->m_sibling;
                    break;
                }
                search = search->m_sibling;
            }

            new_node->m_first_child = new_leaf;
            new_leaf->m_sibling = node_cursor;
            node_cursor->m_sibling = nullptr;

            new_internal_node = search_end = new_node;
        }
    }

    if (this->m_last_internal_node != nullptr)
    {
        assert(this->m_last_internal_node->m_suffix_link == nullptr);
        assert(search_end != nullptr);
        this->m_last_internal_node->m_suffix_link = search_end;
        this->m_last_internal_node = nullptr;
    }

    if (new_internal_node != nullptr)
    {
        this->m_last_internal_node = new_internal_node;
    }

    if (already_in_tree)
    {
        return true;
    }

    return false;
}

/**
 * The search method does two things, it starts from the node_cursor and edge_cursor
 * and find all the way the all but last character of the string. In the process, we
 * also prepare the next_node_cursor and next_text_cursor for the next search.
 *
 * The search for the location is pretty straightforward, there are two things worth
 * notice. When we hit an edge, we just simply moved the cursor without checking the 
 * character. It is because we knew it has to be correct. 
 * 
 * In the book, this implementation optimization is called the skip/count trick.
 *
 * We also set the next_node_cursor and next_text_cursor whenever we found a suffix link.
 * This is where the suffix link serve its purpose, it make the next search much faster 
 * by starting closest to where we already knew it must reach.
 */
void suffix_tree_impl::search(suffix_tree_node*& next_node_cursor, int& next_text_cursor, suffix_tree_node*& node_cursor, int& edge_cursor)
{
    int text_cursor = next_text_cursor;
    next_node_cursor = this->m_root;
    next_text_cursor = text_cursor + 1;
    while (text_cursor < this->m_s.size() - 1)
    {
        int node_length = this->length(node_cursor);
        if (edge_cursor == node_length)
        {
            if (node_cursor->m_suffix_link != nullptr)
            {
                next_node_cursor = node_cursor->m_suffix_link;
                next_text_cursor = text_cursor;
            }

            char next_char = this->m_s[text_cursor];
            suffix_tree_node* child_cursor = node_cursor->m_first_child;
            while (true)
            {
                assert(child_cursor != nullptr);
                if (this->first_char(child_cursor) == next_char)
                {
                    node_cursor = child_cursor;
                    edge_cursor = 0;
                    break;
                }
                else
                {
                    child_cursor = child_cursor->m_sibling;
                }
            }
        }
        else
        {
            int text_move = this->m_s.size() - 1 - text_cursor;
            int edge_move = node_length - edge_cursor;
            int move = text_move > edge_move ? edge_move : text_move;
            edge_cursor += move;
            text_cursor += move;
        }
    }
}

void suffix_tree_impl::show()
{
    this->show(this->m_root, 0);
}

suffix_tree_node* suffix_tree_impl::get_root()
{
    return this->m_root;
}

suffix_tree_node* suffix_tree_impl::get_child(suffix_tree_node* n)
{
    return n->m_first_child;
}

suffix_tree_node* suffix_tree_impl::get_sibling(suffix_tree_node* n)
{
    return n->m_sibling;
}

void suffix_tree_impl::get_edge(suffix_tree_node* n, int* pBegin, int* pEnd)
{
    int length = this->length(n);
    *pBegin = n->m_begin;
    *pEnd = n->m_begin + length;
}

void suffix_tree_impl::show(suffix_tree_node* n, int indent)
{
    for (int i = 0; i< indent; i++)
    {
        cout << " ";
    }
    cout << "'";
    for (int i = 0; i < this->length(n); i++)
    {
        cout << this->m_s[n->m_begin + i];
    }
    cout << "'";
    cout << endl;

    suffix_tree_node* child = n->m_first_child;
    while (child != nullptr)
    {
        this->show(child, indent + 1);
        child = child->m_sibling;
    }
}

int suffix_tree_impl::length(suffix_tree_node* node)
{
    if (node == this->m_root)
    {
        return 0;
    }
    else if (node->m_first_child == nullptr)
    {
        return this->m_s.size() - node->m_begin;
    }
    else
    {
        return node->m_end - node->m_begin;
    }
}

char suffix_tree_impl::first_char(suffix_tree_node* node)
{
    return this->m_s[node->m_begin];
}

suffix_tree_impl::~suffix_tree_impl()
{
    delete this->m_root;
}

suffix_tree::suffix_tree() : m_impl(new suffix_tree_impl())
{
}

void suffix_tree::append(char c)
{
    this->m_impl->append(c);
}

void suffix_tree::show()
{
    this->m_impl->show();
}

suffix_tree::~suffix_tree()
{
    delete this->m_impl;
}

suffix_tree_node* suffix_tree::get_root()
{
    return this->m_impl->get_root();
}

suffix_tree_node* suffix_tree::get_child(suffix_tree_node* n)
{
    return this->m_impl->get_child(n);
}

suffix_tree_node* suffix_tree::get_sibling(suffix_tree_node* n)
{
    return this->m_impl->get_sibling(n);
}

void suffix_tree::get_edge(suffix_tree_node* n, int* pBegin, int* pEnd)
{
    this->m_impl->get_edge(n, pBegin, pEnd);
}
