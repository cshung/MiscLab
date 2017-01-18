#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char** argv)
{
    btree index;
    index.insert(1, 2);
    index.insert(2, 2);
    index.insert(3, 2);
    index.insert(4, 2);
    index.insert(5, 2);
    index.insert(6, 2);
    index.insert(7, 2);
    index.insert(8, 2);
    index.insert(9, 2);
    index.insert(10, 2);
    index.insert(11, 2);
    index.insert(12, 2);

    index.print();

    index.remove(5);

    cout << "After remove" << endl;
    index.print();

    return 0;
}