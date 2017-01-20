#include <iostream>
#include <map>
#include <cstdlib>
#include <cassert>
#include "btree.h"

using namespace std;

int main(int argc, char** argv)
{
    map<int, int> reference;
    btree tree;
    srand(0);
    int min_key = -30;
    int max_key = 30;
    for (int i = 0; i < 1000; i++)
    {
        //cout << "------------------------" << endl;
        //cout << "Operation #" << i << endl;

        int operation = rand() % 3;
        switch (operation)
        {
        case 0:
        {
            int key_to_insert = rand() % (max_key - min_key) + min_key;
            int val_to_insert = rand() % (max_key - min_key) + min_key;

            cout << "inserting (" << key_to_insert << ", " << val_to_insert << ")" << endl;

            bool insertion_result = tree.insert(key_to_insert, val_to_insert);

            if (reference.find(key_to_insert) == reference.end())
            {
                reference.insert(make_pair(key_to_insert, val_to_insert));
                assert(insertion_result);
            }
            else
            {
                assert(!insertion_result);
            }
        }
        break;
        case 1:
        {
            int key_to_remove = rand() % (max_key - min_key) + min_key;

            cout << "removing (" << key_to_remove << ")" << endl;

            bool remove_result = tree.remove(key_to_remove);

            if (reference.find(key_to_remove) != reference.end())
            {
                reference.erase(key_to_remove);
                assert(remove_result);
            }
            else
            {
                assert(!remove_result);
            }
        }
        break;
        case 2:
        {
            int key_to_select = rand() % (max_key - min_key) + min_key;

            cout << "selecting (" << key_to_select << ")" << endl;

            int select_answer;
            bool select_result = tree.select(key_to_select, &select_answer);
            if (reference.find(key_to_select) != reference.end())
            {
                assert(select_result);
                assert(reference[key_to_select] == select_answer);
            }
            else
            {
                assert(!select_result);
            }

            break;
        }
        }
        //tree.print();
        //cout << "------------------------" << endl;
    }

    return 0;
}