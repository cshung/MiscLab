#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char** argv)
{
    {
        btree index;
        for (int i = 0; i < 8; i++)
        {
            index.insert(8 - i, i + 1);
            index.print();
            cout << "====================" << endl;
        }

        index.remove(5);

        cout << "After remove" << endl;
        index.print();

    }

    return 0;
}