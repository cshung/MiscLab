#pragma once

#include <vector>
using namespace std;

class disjoint_set_union_find
{
public:
    disjoint_set_union_find(int n);
    void start_set();
    bool has_next_set();
    int next_set();
    void start_element(int set);
    bool has_next_element();
    int next_element();
    int find(int v);
    void union_sets(int u, int v);
private:
    vector<int> m_sets;
    vector<int> m_next_element;
    vector<int> m_next_set;
    vector<int> m_prev_set;
    int set_cursor;
    int element_cursor;
};
