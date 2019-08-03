#include "suffix_tree.h"
#include "suffix_tree_test.h"
#include <iostream>
#include <string>
using namespace std;

void longest_common_substring_search(string& s, suffix_tree& t, suffix_tree_node* node, int length, int& max_length, int& max_end)
{
    int begin;
    int end;
    t.get_edge(node, &begin, &end);
    int children_count = 0;
    suffix_tree_node* child = t.get_child(node);
    while (child != nullptr)
    {
        longest_common_substring_search(s, t, child, length + end - begin, max_length, max_end);
        child = t.get_sibling(child);
        children_count++;
    }
    if (children_count > 0)
    {
        if (max_length == -1)
        {
            max_length = length + end - begin;
            max_end = end;
        }
        else if (length + end - begin > max_length)
        {
            max_length = length + end - begin;
            max_end = end;
        }
    }
}

string longest_common_substring(string& s)
{
    suffix_tree t;
    for (int i = 0; i < s.size(); i++)
    {
        t.append(s[i]);
    }
    t.append('$');
    int max_length = -1;
    int max_end;
    longest_common_substring_search(s, t, t.get_root(), 0, max_length, max_end);
    int begin = max_end - max_length;
    return s.substr(begin, max_length);
}

int longest_common_substring_test()
{
    string s = "banana$";
    cout << longest_common_substring(s) << endl;
    return 0;
}

int suffix_tree_test()
{
    unsigned int length = 100000;
    suffix_tree s;
    for (unsigned int i = 0; i < length; i++)
    {
        s.append(rand() % 26 + 'A');
    }
    return 0;
}