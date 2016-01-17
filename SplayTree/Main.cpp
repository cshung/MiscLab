#include <iostream>
#include <cassert>
#include <map>
#include <cstdlib>
#include <ctime>
#include "splay_tree.h"
using namespace std;

int main(int argc, char** argv)
{
    // Step 1: Generate test data
    map<int, int> test;

    srand(time(0));
    for (int i = 0; i < 3000; i++)
    {
        while (true)
        {
            int key = rand() % 10000;
            int value = rand();
            map<int, int>::iterator probe = test.find(key);
            if (probe == test.end())
            {
                test.insert(pair<int, int>(key, value));
                break;
            }
        }
    }

    // Step 2: Insert test data
    splay_tree tree;
    for (map<int, int>::iterator ti = test.begin(); ti != test.end(); ++ti)
    {
        bool succeed = tree.try_insert(ti->first, ti->second);
        assert(succeed);
    }

    // Step 3: Random probing
    for (int i = 0; i < 10000; i++)
    {
        int tree_value;
        map<int, int>::iterator test_probe = test.find(i);
        bool found_in_test = test_probe != test.end();
        bool found_in_tree = tree.try_get(i, &tree_value);
        assert(found_in_test == found_in_tree);
        if (found_in_test)
        {
            assert(test_probe->second == tree_value);
            tree.try_delete(i);
            assert(!tree.try_get(i, &tree_value));
        }
    }

    return 0;
}