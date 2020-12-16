#include "disjoint_set_union_find.h"

disjoint_set_union_find::disjoint_set_union_find(int n) : m_sets(n, -1), m_next_element(n), m_next_set(n + 2), m_prev_set(n + 2)
{
    for (int i = 0; i < n; i++)
    {
        m_next_element[i] = i;
        m_next_set[i + 1] = i + 2;
        m_prev_set[i + 1] = i;
    }
    m_next_set[0] = 1;
    m_prev_set[n + 1] = n;
}

void disjoint_set_union_find::start_set()
{
    set_cursor = 0;
}

bool disjoint_set_union_find::has_next_set()
{
    return m_next_set[set_cursor] != m_sets.size() + 1;
}

int disjoint_set_union_find::next_set()
{
    set_cursor = m_next_set[set_cursor];
    return set_cursor - 1;
}

void disjoint_set_union_find::start_element(int set)
{
    set_cursor = set + 1;
    element_cursor = -1;
}

bool disjoint_set_union_find::has_next_element()
{
    return (element_cursor == -1) || m_next_element[element_cursor] != set_cursor - 1;
}

int disjoint_set_union_find::next_element()
{
    if (element_cursor == -1)
    {
        element_cursor = set_cursor - 1;
    }
    else
    {
        element_cursor = m_next_element[element_cursor];
    }
    return element_cursor;
}

int disjoint_set_union_find::find(int v)
{
    if (m_sets[v] < 0)
    {
        return v;
    }
    else
    {
        return m_sets[v] = find(m_sets[v]);
    }
}

void disjoint_set_union_find::union_sets(int u, int v)
{
    int set1 = find(u);
    int set2 = find(v);
    if (set1 != set2)
    {
        int next_element_set1 = m_next_element[set1];
        int next_element_set2 = m_next_element[set2];
        m_next_element[set1] = next_element_set2;
        m_next_element[set2] = next_element_set1;

        int size1 = -m_sets[set1];
        int size2 = -m_sets[set2];
        if (size1 > size2)
        {
            m_next_set[m_prev_set[set2 + 1]] = m_next_set[set2 + 1];
            m_prev_set[m_next_set[set2 + 1]] = m_prev_set[set2 + 1];
            m_next_set[set2 + 1] = -1;
            m_prev_set[set2 + 1] = -1;
            m_sets[set2] = set1;
            m_sets[set1] -= size2;
        }
        else
        {
            m_next_set[m_prev_set[set1 + 1]] = m_next_set[set1 + 1];
            m_prev_set[m_next_set[set1 + 1]] = m_prev_set[set1 + 1];
            m_next_set[set1 + 1] = -1;
            m_prev_set[set1 + 1] = -1;
            m_sets[set1] = set2;
            m_sets[set2] -= size1;
        }
    }
}
