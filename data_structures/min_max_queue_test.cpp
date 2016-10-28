#include <iostream>
#include "min_max_queue.h"

using namespace std;

void min_max_queue_test()
{
    min_max_queue queue;
    int min;
    int max;
    queue.enqueue(3);    queue.min(&min);    queue.max(&max);    cout << min << endl;    cout << max << endl; cout << endl;
    queue.enqueue(0);    queue.min(&min);    queue.max(&max);    cout << min << endl;    cout << max << endl; cout << endl;
    queue.enqueue(6);    queue.min(&min);    queue.max(&max);    cout << min << endl;    cout << max << endl; cout << endl;
    queue.enqueue(2);    queue.min(&min);    queue.max(&max);    cout << min << endl;    cout << max << endl; cout << endl;
    queue.enqueue(4);    queue.min(&min);    queue.max(&max);    cout << min << endl;    cout << max << endl; cout << endl;
    queue.enqueue(7);    queue.min(&min);    queue.max(&max);    cout << min << endl;    cout << max << endl; cout << endl;
    queue.enqueue(0);    queue.min(&min);    queue.max(&max);    cout << min << endl;    cout << max << endl; cout << endl;
    queue.enqueue(0);    queue.min(&min);    queue.max(&max);    cout << min << endl;    cout << max << endl; cout << endl;

    cout << "--------------------------" << endl;

    int val;
    queue.dequeue(&val);     queue.min(&min);    queue.max(&max);    cout << val << endl;    cout << min << endl;    cout << max << endl; cout << endl;
    queue.dequeue(&val);     queue.min(&min);    queue.max(&max);    cout << val << endl;   cout << min << endl;    cout << max << endl; cout << endl;
    queue.dequeue(&val);     queue.min(&min);    queue.max(&max);    cout << val << endl;   cout << min << endl;    cout << max << endl; cout << endl;
    queue.dequeue(&val);     queue.min(&min);    queue.max(&max);    cout << val << endl;   cout << min << endl;    cout << max << endl; cout << endl;
    queue.dequeue(&val);     queue.min(&min);    queue.max(&max);    cout << val << endl;   cout << min << endl;    cout << max << endl; cout << endl;
    queue.dequeue(&val);     queue.min(&min);    queue.max(&max);    cout << val << endl;   cout << min << endl;    cout << max << endl; cout << endl;
    queue.dequeue(&val);     queue.min(&min);    queue.max(&max);    cout << val << endl;   cout << min << endl;    cout << max << endl; cout << endl;
    queue.dequeue(&val);    cout << val << endl;
}