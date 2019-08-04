#include <iostream>
#include <cstdlib>
#include <limits.h>
#include "min_max_heap.h"
#include "min_max_heap_test.h"

using namespace std;

void min_max_heap_test()
{
    const int num_experiments = 500;
    min_max_heap my_min_max_heap(num_experiments);
    int data[num_experiments];
    for (int i = 0; i < num_experiments; i++)
    {
        data[i] = -1;
    }
    for (int i = 0; i < num_experiments; i++)
    {
        int operation = rand() % 3;
        if (my_min_max_heap.get_size() == 0)
        {
            // the only valid operation is insert a random value if the size is 0
            operation = 0;
        }

        if (operation == 0)
        {
            int value = rand() % 50;
            cout << i << ":\t Insert " << value << endl;
            // Naive implementation
            for (int i = 0; i < num_experiments; i++)
            {
                if (data[i] == -1)
                {
                    data[i] = value;
                    break;
                }
            }

            if (!my_min_max_heap.try_insert(value))
            {
                cout << "Fail" << endl;
            }
        }
        else if (operation == 1)
        {
            cout << i << ":\t Delete min" << endl;
            int min_value = INT_MAX;
            int min_index = -1;
            // Naive implementation
            for (int j = 0; j < num_experiments; j++)
            {
                if (data[j] != -1)
                {
                    if (data[j] < min_value)
                    {
                        min_value = data[j];
                        min_index = j;
                    }
                }
            }
            data[min_index] = -1;
            double test_min_value;
            if (!my_min_max_heap.try_delete_min(&test_min_value))
            {
                cout << "Fail" << endl;
            }
            if (test_min_value != min_value)
            {
                cout << "Fail" << endl;
            }
            if (!my_min_max_heap.verify_consistency())
            {
                cout << "Fail" << endl;
            }
        }
        else if (operation == 2)
        {
            cout << i << ":\t Delete max" << endl;
            int max_value = INT_MIN;
            int max_index = -1;
            // Naive implementation
            for (int j = 0; j < num_experiments; j++)
            {
                if (data[j] != -1)
                {
                    if (data[j] > max_value)
                    {
                        max_value = data[j];
                        max_index = j;
                    }
                }
            }
            data[max_index] = -1;
            double test_max_value;
            if (!my_min_max_heap.try_delete_max(&test_max_value))
            {
                cout << "Fail" << endl;
            }
            if (test_max_value != max_value)
            {
                cout << "Fail" << endl;
            }
            if (!my_min_max_heap.verify_consistency())
            {
                cout << "Fail" << endl;
            }
        }
    }
}
