#include <iostream>
#include "disjoint_set_union_find.h"

using namespace std;

void disjoint_set_union_find_test()
{
    disjoint_set_union_find dsu(10);
    dsu.union_sets(0, 1);
    dsu.union_sets(2, 3);
    dsu.union_sets(0, 3);
    for (dsu.start_set(); dsu.has_next_set(); )
    {
        int set = dsu.next_set();
        cout << set << ":";
        for (dsu.start_element(set); dsu.has_next_element();)
        {
            int element = dsu.next_element();
            cout << "  " << element;
        }
        cout << endl;
    }
}